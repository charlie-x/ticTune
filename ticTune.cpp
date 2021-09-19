// ticTune.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// base range of device movement
#define UPPER_RANGE		( 200 )
#define LOWER_RANGE		( -5500 )

#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>
#include <filesystem>
#include <iostream>
#include <math.h>
#include <windows.h>

#include "tic/tic.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx12.h"

#include "imgui/implot.h"

#ifndef M_PI
#   define M_PI    3.14159265358979323846
#endif

#pragma comment(lib,"tic/libtic.lib")
#pragma comment(lib,"tic/usbp-1.lib")

#pragma comment(lib,"setupapi.lib")
#pragma comment(lib,"winusb.lib")


// step mode

static constexpr char stepModes[][20] = {
	"Full step",
	"1/2 step",
	"1/4 step",
	"1/8 step",
	"1/16 step",
	"1/32 step",

	"1/2 step 100%",
	"1/64 step",
	"1/128 step",
	"1/256 step"
};

enum class Modes {
	mNONE,
	mSIN,
	mSIN2,
	mSIN3,
	mPINGPONG,
};

Modes mMode;

// T825
static constexpr char decayModes[][20] = {
	"slow",
	"mixed(d)",
	"fast"
};

bool bBorderless = false;
bool bVSync = true;

static tic::handle handle;
static tic::variables vars;
static tic::settings settings;

static bool bInvertMotor = false;
static bool bAutoUpdate = false;
static bool bChanged = false;
static bool bOneAccel = true;

static int iMaxSpeed			= 28400000;
static int iStartingSpeed		= 0;
static int iMaxAcceleration		= 280000;
static int iMaxDeceleration		= 280000;
static int step_mode			= 0;
static int decay_mode			= 0;
static int current_limit = 0;

int RenderGUI();
bool SetupImgui();
void  RenderLoop();
bool CleanupImgui();

// utility structure for realtime plot
struct ScrollingBuffer {

	int MaxSize;
	int Offset;

	ImVector<ImVec2> Data;
	ImVec2 DataAvg;

	double min = DBL_MAX, max = DBL_MIN;

	ScrollingBuffer(int max_size = 1500) {
		MaxSize = max_size;
		Offset = 0;

		Data.reserve(MaxSize);

		DataAvg.x = 0;
		DataAvg.y = 0;
	}

	void AddPoint(float x, float y) {
		if (Data.size() < MaxSize) {
			Data.push_back(ImVec2(x, y));
		}
		else {
			Data[Offset] = ImVec2(x, y);
			Offset = (Offset + 1) % MaxSize;
		}

		DataAvg.x = 0;
		DataAvg.y = 0;

		min = DBL_MAX;
		max = DBL_MIN;

		for (int i = 0; i < Data.size(); i++) {
			DataAvg.x += Data[i].x;
			DataAvg.y += Data[i].y;

			min = min(Data[i].y, min);
			max = max(Data[i].y, max);
		}

		DataAvg.x /= Data.size();
		DataAvg.y /= Data.size();
	}

	void Erase() {
		if (Data.size() > 0) {
			Data.shrink(0);
			Offset = 0;
		}
	}
};

// tic lib needs this

extern "C" void usleep(__int64 usec)
{
	HANDLE timer;
	LARGE_INTEGER ft;

	ft.QuadPart = -(10 * usec);										// Convert to 100 nanosecond interval, negative value indicates relative time

	timer = CreateWaitableTimer(NULL, TRUE, NULL);

	if (timer) {
		SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
		WaitForSingleObject(timer, INFINITE);
		CloseHandle(timer);
	}
}
std::vector<tic::device> list;

// Opens a handle to a Tic that can be used for communication.
//
// To open a handle to any Tic:
//   tic_handle * handle = open_handle();
// To open a handle to the Tic with serial number 01234567:
//   tic_handle * handle = open_handle("01234567");
tic::handle open_handle(const char* desired_serial_number = nullptr)
{
	// Get a list of Tic devices connected via USB.
	// moved to init

	// Iterate through the list and select one device.
	for (const tic::device& device : list) {
		if (desired_serial_number &&
			device.get_serial_number() != desired_serial_number) {
			// Found a device with the wrong serial number, so continue on to
			// the next device in the list.
			continue;
		}

		// Open a handle to this device and return it.
		return tic::handle(device);
	}

	throw std::runtime_error("No device found.");
}

int main()
{
	list = tic::list_connected_devices();

	SetupImgui();

	// Handle the TIC
	try {

		handle = open_handle();

		vars = handle.get_variables();
		settings = handle.get_settings();

		step_mode =
			tic_settings_get_step_mode(settings.get_pointer());

		decay_mode =
			tic_settings_get_decay_mode(settings.get_pointer());

		current_limit =
			tic_settings_get_current_limit(settings.get_pointer());

		bInvertMotor = tic_settings_get_input_invert(settings.get_pointer());

		iMaxSpeed = tic_settings_get_max_speed(settings.get_pointer());
		iStartingSpeed = tic_settings_get_starting_speed(settings.get_pointer());
		iMaxAcceleration = tic_settings_get_max_accel(settings.get_pointer());
		iMaxDeceleration = tic_settings_get_max_decel(settings.get_pointer());
	}
	catch (const std::exception& error) {
		std::cerr << "Error: " << error.what() << std::endl;
		return 1;
	}

	// turn TIC off
	handle.deenergize();

	RenderLoop();

	CleanupImgui();

	return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	main();
}

int GetRange(int step_mode, const int base = UPPER_RANGE)
{
	switch (step_mode) {
		case 0:
			return base;

		case 1:
			return base * 2;

		case 2:
			return base * 4;

		case 3:
			return base * 8;

		case 4:
			return base * 16;

		case 5:
			return base * 32;

		default:
			return base;
	}
}


bool DrawSButton(const std::string text, const std::string name, int& variable, int value)
{
	bool ret = false;

	std::string label;

	label = (text + "##" + name);

	if (ImGui::Button(label.c_str())) {

		if (variable > value) {
			variable -= value;
		}

		ret = true;
	}

	ImGui::SameLine();

	return ret;
}

bool DrawAButton(const std::string text, const std::string name, int& variable, int value)
{
	bool ret = false;

	std::string label;

	label = (text + "##" + name);

	if (ImGui::Button(label.c_str())) {

		variable += value;
		ret = true;
	}

	ImGui::SameLine();

	return ret;
}

bool DrawSlider(const std::string name, int& variable, int lower, int upper)
{
	int  ret = 0;
	std::string label;;

	ret += DrawSButton("-1K",   name, variable, 1000);
	ret += DrawSButton("-10K",  name, variable, 10000);
	ret += DrawSButton("-100K", name, variable, 100000);

	label = ("##int" + name);

	if (ImGui::SliderInt(label.c_str(), &variable, lower, upper)) {
		ret = true;
	}

	ImGui::SameLine();

	ret += DrawAButton("+100K", name, variable, 100000);
	ret += DrawAButton("+10K",  name, variable, 10000);
	ret += DrawAButton("+1K",   name, variable, 1000);

	ImGui::Text(name.c_str());

	return (ret ? true : false);
}

bool DrawButton(const char* const label, Modes mode, bool bSameLine = true)
{
	bool ret = false;
	bool bStylePushed = false;

	if (mMode != mode) {
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		bStylePushed = true;
	}

	if (ImGui::Button(label)) {
		mMode = mode;
		ret = true;
	}

	if (bSameLine) {
		ImGui::SameLine();
	}

	if (bStylePushed) {
		ImGui::PopStyleVar();
	}

	return ret;
}

int RenderGUI()
{
	static bool _showMTTuning = true;
	static bool _bEnableTIC = false;
	static float elapsedTime = 0;

	static bool bPaused = false;

	ImVec2 winSize;
	static ScrollingBuffer positionHistory[3];
	static ScrollingBuffer vinHistory[6];   // vin, min, max

	ImGui::SetNextWindowSize(ImVec2(1000, 640), ImGuiCond_FirstUseEver);

	static int32_t new_target = 0;

	ImGui::Begin("Motor Controls##ticTune", &_showMTTuning);
	{
		if (ImGui::Button("ENERGISE")) {
			handle.energize();
			_bEnableTIC = true;
		}

		ImGui::SameLine();

		if (ImGui::Button("DEENERGISE")) {
			_bEnableTIC = false;
			handle.deenergize();
		}

		if (_bEnableTIC) {
			ImGui::SameLine();
			ImGui::Text("Enabled");
		}

		for (const tic::device& device : list) {
			ImGui::Text("%s %lu", 
				device.get_short_name().c_str(), 
				device.get_serial_number() 
			);
		}

		ImGui::Checkbox("Pause", &bPaused);
		ImGui::SameLine();
		ImGui::Checkbox("AA", &ImPlot::GetStyle().AntiAliasedLines);
		ImGui::SameLine();
		ImGui::Checkbox("vSync", &bVSync);
		ImGui::NewLine();

		DrawButton("NONE", Modes::mNONE);
		DrawButton("SIN", Modes::mSIN);
		DrawButton("SIN 2x", Modes::mSIN2);
		DrawButton("SIN 3", Modes::mSIN3);
		DrawButton("PING PONG", Modes::mPINGPONG, false);

		try {
			bool bUpdate = false;

			// this would override what we have
			// settings = handle.get_settings();

			vars = handle.get_variables();

			int32_t current_position = vars.get_current_position();

			static double request = 0;

			if (mMode == Modes::mSIN) {
				request = sin(elapsedTime);
				bUpdate = true;
			}

			if (mMode == Modes::mSIN2) {
				request = ((sin(elapsedTime * 2.0)));
				bUpdate = true;
			}

			//sin(2 * pi * x) + cos(x / 2 * pi)

			if (mMode == Modes::mSIN3) {

				request = ((sin(2.0 * M_PI * elapsedTime) + cos(elapsedTime / 2.0 * M_PI)) / 2.0);
				bUpdate = true;
			}

			static float lastChange = 1;

			if (mMode == Modes::mPINGPONG) {

				if (elapsedTime > lastChange) {

					if (request == 1) {

						request = -1;
					}
					else {
						request = 1;
					}

					lastChange = elapsedTime + 1;
				}

				bUpdate = true;
			}

			ImGui::End();

			if (bUpdate) {

				double temp = (((double)request + 1.0) / 2.0) * ( ( GetRange(step_mode,LOWER_RANGE) - GetRange(step_mode) ) ) + GetRange(step_mode);

				new_target = (int32_t)temp;

				handle.set_target_position(new_target);
			}

			if ( !bPaused ) {

				elapsedTime += ImGui::GetIO().DeltaTime;

				positionHistory[0].AddPoint(elapsedTime, (float)new_target);

				positionHistory[1].AddPoint(elapsedTime, (float)current_position);

				positionHistory[2].AddPoint(elapsedTime, (float)(vars.get_current_velocity() / 10000) - (vars.get_max_speed() / 10000.0f));

			}

			double vin = (vars.get_vin_voltage() / 1000.0);

			static double vinMin;
			static double vinMax;

			if (vinHistory[0].Data.size() == 0) {

				vinMin = vin;
				vinMax = vin;
			}

			vinMin = min(vin, vinMin);
			vinMax = max(vin, vinMax);

			if (!bPaused) {
				vinHistory[0].AddPoint(elapsedTime, (float)vin);
				vinHistory[1].AddPoint(elapsedTime, (float)vinMin);
				vinHistory[2].AddPoint(elapsedTime, (float)vinMax);
			}

			if (!bPaused) {
				vinHistory[3].AddPoint(elapsedTime, (float)((vinHistory[0].DataAvg.y) / vin) * 100.0f);
				vinHistory[4].AddPoint(elapsedTime, (float)(vinHistory[0].min / vin) * 100.0f);
				vinHistory[5].AddPoint(elapsedTime, (float)(vinHistory[0].max / vin) * 100.0f);
			}

			if (ImGui::Begin("Information")) {

				static bool showWarning = sizeof(ImDrawIdx) * 8 == 16 && (ImGui::GetIO().BackendFlags & ImGuiBackendFlags_RendererHasVtxOffset) == false;

				if (showWarning) {
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 0, 1));
					ImGui::TextWrapped("WARNING: ImDrawIdx is 16-bit and ImGuiBackendFlags_RendererHasVtxOffset is false. Expect visual glitches and artifacts!");
					ImGui::PopStyleColor();
				}

				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, .5, 1, 1)); {
					ImGui::Text("Current Tine      [%f]", elapsedTime);
					ImGui::Text("Current Position  [%d]", current_position);
					ImGui::Text("Request Position  [%d]", new_target);
					ImGui::Text("Current Velocity  [%d]", vars.get_current_velocity() / 10000);
				}ImGui::PopStyleColor();

				ImGui::Separator();

				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(.5, 1, 1, 1)); {

					ImGui::Text("Max Speed         %f", vars.get_max_speed() / 10000.0);
					ImGui::Text("Starting Speed    %f", vars.get_starting_speed() / 10000.0);

					ImGui::Text("Max Acceleration  %f", vars.get_max_accel() / 100.0);
					ImGui::Text("Max Deceleration  %f", vars.get_max_decel() / 100.0);

					ImGui::Text("Current Limit     %d mA", vars.get_current_limit());
				}ImGui::PopStyleColor();

				ImGui::Separator();

				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 0, 1)); {

					ImGui::Text("VIN               %f", vin);
					ImGui::Text("VIN Avg           %f", vinHistory[0].DataAvg.y);
					ImGui::Text("VIN Min           %f", vinHistory[0].min);
					ImGui::Text("VIN Max           %f", vinHistory[0].max);

					ImGui::Text("VINAvg Max Diff   %f", (vinHistory[0].max - (vinHistory[0].DataAvg.y)));
					ImGui::Text("VIN Max Diff      %f", (vinHistory[0].max - vin));
					ImGui::Text("VINAvg Min Diff   %f", fabs(vinHistory[0].min - (vinHistory[0].DataAvg.y)));
					ImGui::Text("VIN Min Diff      %f", fabs(vinHistory[0].min - vin));

					ImGui::Text("VIN Avg %%         %f", (vinHistory[0].DataAvg.y / vin) * 100.0);
					ImGui::Text("VIN Avg %%         %f", (vinHistory[0].min / vin) * 100.0);
					ImGui::Text("VIN Avg %%         %f", (vinHistory[0].max / vin) * 100.0);
					ImGui::Text("VIN Max %%         %f", fabs(vinHistory[0].max - vin));

				} ImGui::PopStyleColor();

			}

			handle.exit_safe_start();

			if (current_position != new_target) {
				handle.set_target_position(new_target);
			}
		}
		catch (const std::exception& error) {
			std::cerr << "Error: " << error.what() << std::endl;
			return 1;
		}
	}
	ImGui::End();

	if (ImGui::Begin("Trainer")) {

		/// <summary>
		///  each of the trainer modes
		/// </summary>
		enum class TrainMode {
			NONE,
			IDLE,
			IDLE_COLLECT,
			ENERGISED,
			ENERGISED_COLLECT,
			SLOW,
			SLOW_COLLECT,
			FASTER,
			FASTER_COLLECT,
			LOAD,
			LOAD_COLLECT,
			DONE
		};

		static TrainMode mTrainMode = TrainMode::NONE;

		static bool bTraining = false;

		static float iTrainTimer = 0;
		static bool bIdleDataValid = false, bEnergisedDataValid = false, bSlowDataValid = false, bFasterDataValid = false, bLoadDataValid = false;

		static double idleMax = DBL_MIN, idleMin = DBL_MAX;
		static double energisedMax = DBL_MIN, energisedMin = DBL_MAX;
		static double slowMax = DBL_MIN, slowMin = DBL_MAX;
		static double fasterMax = DBL_MIN, fasterMin = DBL_MAX;
		static double loadMax = DBL_MIN, loadMin = DBL_MAX;

		if (ImGui::Button("Train")) {

			bTraining = true;
			mTrainMode = TrainMode::IDLE;
			bIdleDataValid = false;
			bEnergisedDataValid = false;
			bSlowDataValid = false;
			bFasterDataValid = false;
			bLoadDataValid = false;
		}

		static int iTorqueBias = 0;
		static int iSpeedBias = 0;
		static int iAccelBias = 0;

		if (ImGui::SliderInt("Torque##torque", &iTorqueBias, -100, 100)) {
		}

		if (ImGui::SliderInt("Speed##speed", &iSpeedBias, -100, 100)) {
		}

		if (ImGui::SliderInt("Accel##accel", &iAccelBias, -100, 100)) {
		}

		if (bTraining) {

			switch (mTrainMode) {

			case TrainMode::NONE:
				break;

			case TrainMode::IDLE:
				// turn off motors
				handle.deenergize();

				// 30 seconds training time
				iTrainTimer = (30 + elapsedTime);
				mTrainMode = TrainMode::IDLE_COLLECT;
				break;

			case TrainMode::IDLE_COLLECT:

				// next mode
				if (elapsedTime > iTrainTimer) {

					idleMax = vinHistory[0].max;
					idleMin = vinHistory[0].min;

					mTrainMode = TrainMode::ENERGISED;
					iTrainTimer = (30 + elapsedTime);

					bIdleDataValid = true;

					break;
				}

				ImGui::Text("Collecting idle data");

				break;

			case TrainMode::ENERGISED:
				// turn on motors
				handle.energize();
				_bEnableTIC = true;

				// 30 seconds training time
				iTrainTimer = (30 + elapsedTime);
				mTrainMode = TrainMode::ENERGISED_COLLECT;
				break;

			case TrainMode::ENERGISED_COLLECT:

				// next mode
				if (elapsedTime > iTrainTimer) {

					energisedMax = vinHistory[0].max;
					energisedMin = vinHistory[0].min;

					mTrainMode = TrainMode::SLOW;
					iTrainTimer = (30 + elapsedTime);

					bEnergisedDataValid = true;

					break;
				}

				ImGui::Text("Collecting energised data");

				break;

			case TrainMode::SLOW:

				// turn on motors
				handle.energize();
				_bEnableTIC = true;

				// 30 seconds training time
				iTrainTimer = (30 + elapsedTime);
				mTrainMode = TrainMode::SLOW_COLLECT;
				mMode = Modes::mSIN;

				break;

			case TrainMode::SLOW_COLLECT:

				// next mode
				if (elapsedTime > iTrainTimer) {

					slowMax = vinHistory[0].max;
					slowMin = vinHistory[0].min;

					mTrainMode = TrainMode::FASTER;
					iTrainTimer = (30 + elapsedTime);

					bSlowDataValid = true;

					break;
				}

				ImGui::Text("Collecting slow data");

				break;

			case TrainMode::FASTER:

				// turn on motors
				handle.energize();
				_bEnableTIC = true;

				// 30 seconds training time
				iTrainTimer = (30 + elapsedTime);
				mTrainMode = TrainMode::FASTER_COLLECT;

				mMode = Modes::mSIN3;

				break;

			case TrainMode::FASTER_COLLECT:

				// next mode
				if (elapsedTime > iTrainTimer) {

					fasterMax = vinHistory[0].max;
					fasterMin = vinHistory[0].min;

					mTrainMode = TrainMode::LOAD;
					iTrainTimer = (30 + elapsedTime);

					bFasterDataValid = true;

					break;
				}

				ImGui::Text("Collecting faster data");

				break;

			case TrainMode::LOAD:

				// turn on motors
				handle.energize();
				_bEnableTIC = true;

				// 30 seconds training time
				iTrainTimer = (30 + elapsedTime);
				mTrainMode = TrainMode::LOAD_COLLECT;

				mMode = Modes::mPINGPONG;

				break;

			case TrainMode::LOAD_COLLECT:

				// next mode
				if (elapsedTime > iTrainTimer) {

					loadMax = vinHistory[0].max;
					loadMin = vinHistory[0].min;

					mTrainMode = TrainMode::DONE;
					iTrainTimer = 0;

					mMode = Modes::mNONE;

					bLoadDataValid = true;

					// motors off
					handle.deenergize();
					_bEnableTIC = false;
					break;
				}

				ImGui::Text("Collecting load data");

				break;
			}
		}

		int iCount = (int)( iTrainTimer - elapsedTime );

		if (ImGui::SliderInt("Time##time", &iCount, 0,(int)( iTrainTimer - elapsedTime ) )) {
		}

		if (bIdleDataValid) {
			ImGui::Text("Idle Min      %f", idleMin);
			ImGui::Text("Idle Max      %f", idleMax);
		}

		if (bEnergisedDataValid) {
			ImGui::Text("Energised Min %f", energisedMin);
			ImGui::Text("Energised Max %f", energisedMax);
		}

		if (bSlowDataValid) {
			ImGui::Text("Slow Min      %f", slowMin);
			ImGui::Text("Slow Max      %f", slowMax);
		}

		if (bFasterDataValid) {
			ImGui::Text("Faster Min    %f", fasterMin);
			ImGui::Text("Faster Max    %f", fasterMax);
		}

		if (bLoadDataValid) {
			ImGui::Text("Loaded Min    %f", loadMin);
			ImGui::Text("Loaded Max    %f", loadMax);
		}
	}

	ImGui::End();

	if (ImGui::Begin("Data")) {

		if (ImGui::SliderInt("Target Position##ticTune1", &new_target, GetRange(step_mode , LOWER_RANGE ), GetRange(step_mode))) {
			handle.set_target_position(new_target);
		}
		ImGui::SameLine();

		static ImPlotAxisFlags xflags = ImPlotAxisFlags_None;
		static ImPlotAxisFlags yflags = ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_RangeFit;

		ImGui::CheckboxFlags("Auto Fit##Y", (unsigned int*)&yflags, ImPlotAxisFlags_AutoFit);
		ImGui::SameLine();

		ImGui::CheckboxFlags("Range Fit##Y", (unsigned int*)&yflags, ImPlotAxisFlags_RangeFit);

		ImPlot::SetNextPlotLimitsY(-1610, 210);

		ImPlot::SetNextPlotLimitsX(elapsedTime - 10.0, elapsedTime, bPaused ? ImGuiCond_Once : ImGuiCond_Always);

		if (ImPlot::BeginPlot("Motor Position", "time", "pos", ImVec2(-1, 0), 0, xflags, yflags)) {

			for (int i = 0; i < 3; i++) {

				if (positionHistory[i].Data.size()) {

					switch (i) {

					case 0:
						ImPlot::SetNextLineStyle(ImVec4(1, 1, 1, 1));
						break;

					case  1:
						ImPlot::SetNextLineStyle(ImVec4(0, 1, 1, 1));
						break;

					case  2:
						ImPlot::SetNextLineStyle(ImVec4(1, 0, 1, 1));
						break;
					}

					const char names[][20] = { "Target", "Current", "Velocity" };

					ImPlot::PlotLine(names[i], &positionHistory[i].Data[0].x, &positionHistory[i].Data[0].y, positionHistory[i].Data.size(), positionHistory[i].Offset, 2 * sizeof(float));
				}
			}

			ImPlot::EndPlot();
		}

		ImPlot::SetNextPlotLimitsY(-1610, 210);

		ImPlot::SetNextPlotLimitsX(elapsedTime - 10.0, elapsedTime, bPaused ? ImGuiCond_Once : ImGuiCond_Always);

		if (ImPlot::BeginPlot("VIN History##vinHistory", "time", "VIN", ImVec2(-1, 0), 0, xflags, yflags)) {

			if (vinHistory[0].Data.size()) {

				ImPlot::SetNextLineStyle(ImVec4(1, 1, .5, 1));
				ImPlot::PlotLine("VIN##vin", &vinHistory[0].Data[0].x, &vinHistory[0].Data[0].y, vinHistory[0].Data.size(), vinHistory[0].Offset, 2 * sizeof(float));
				ImPlot::SetNextLineStyle(ImVec4(.5, 1, 1, 1));
				ImPlot::PlotLine("min##vinmin", &vinHistory[1].Data[0].x, &vinHistory[1].Data[0].y, vinHistory[1].Data.size(), vinHistory[1].Offset, 2 * sizeof(float));
				ImPlot::SetNextLineStyle(ImVec4(1, 0, 1, 1));
				ImPlot::PlotLine("max##vinmax", &vinHistory[2].Data[0].x, &vinHistory[2].Data[0].y, vinHistory[2].Data.size(), vinHistory[2].Offset, 2 * sizeof(float));
			}

			ImPlot::EndPlot();
		}

		ImPlot::SetNextPlotLimitsY(-1610, 210);

		ImPlot::SetNextPlotLimitsX(elapsedTime - 10.0, elapsedTime, bPaused ? ImGuiCond_Once : ImGuiCond_Always);
		if (ImPlot::BeginPlot("VIN History##vinAvgHistory", "time", "%", ImVec2(-1, 0), 0, xflags, yflags)) {

			ImPlot::PushColormap(ImPlotColormap_Plasma); {

				if (vinHistory[3].Data.size()) {
					ImPlot::SetNextLineStyle(ImVec4(1, 1, 1, 1));
					ImPlot::PlotLine("VinAvg##vin", &vinHistory[3].Data[0].x, &vinHistory[3].Data[0].y, vinHistory[3].Data.size(), vinHistory[3].Offset, 2 * sizeof(float));
					ImPlot::SetNextLineStyle(ImVec4(1, 0, 1, 1));
					ImPlot::PlotLine("VinMin##vinmin", &vinHistory[4].Data[0].x, &vinHistory[4].Data[0].y, vinHistory[4].Data.size(), vinHistory[4].Offset, 2 * sizeof(float));
					ImPlot::SetNextLineStyle(ImVec4(0, 1, 0, 1));
					ImPlot::PlotLine("VinMax##vinmax", &vinHistory[5].Data[0].x, &vinHistory[5].Data[0].y, vinHistory[5].Data.size(), vinHistory[5].Offset, 2 * sizeof(float));
				}

				ImPlot::EndPlot();
			} ImPlot::PopColormap();
		}
	}

	ImGui::End();

	if (ImGui::Begin("Motor Config")) {
		{
			ImGui::Checkbox("Invert", &bInvertMotor);
			ImGui::SameLine();
			ImGui::Checkbox("Auto Update", &bAutoUpdate);
			ImGui::SameLine();
			ImGui::Checkbox("One Accel", &bOneAccel);

			if (DrawSlider("Max Speed", iMaxSpeed, 0, 500000000)) {
				tic_settings_set_max_speed(settings.get_pointer(), iMaxSpeed);
				bChanged = true;
			}

			if (DrawSlider("Starting Speed", iStartingSpeed, 0, 500000000)) {
				tic_settings_set_starting_speed(settings.get_pointer(), iStartingSpeed);
				bChanged = true;
			}

			if (DrawSlider("Max Acceleration", iMaxAcceleration, 100, 2147483647 / 20)) {
				tic_settings_set_max_accel(settings.get_pointer(), iMaxAcceleration);

				if (bOneAccel == true) {
					tic_settings_set_max_decel(settings.get_pointer(), iMaxDeceleration);
				}

				bChanged = true;
			}
		}

		if (bOneAccel == false) {
			if (DrawSlider("Max Deceleration", iMaxDeceleration, 100, 2147483647 / 20)) {
				tic_settings_set_max_decel(settings.get_pointer(), iMaxDeceleration);
				bChanged = true;
			}
		}

		if (ImGui::SliderInt("Step Mode##stepMode", &step_mode, 0, 5)) {
			tic_settings_set_step_mode(settings.get_pointer(), step_mode);
			bChanged = true;
		}

		ImGui::SameLine();
		ImGui::Text("%s", stepModes[step_mode]);

		// current limit
		if (ImGui::SliderInt("Current Limit##currentLimit", &current_limit, 0, 1024)) {
			tic_settings_set_current_limit(settings.get_pointer(), current_limit);
			current_limit = tic_settings_get_current_limit(settings.get_pointer());
			bChanged = true;
		}

		// decay mode
		if (ImGui::SliderInt("Decay Mode##decayMode", &decay_mode, 0, 2)) {
			tic_settings_set_decay_mode(settings.get_pointer(), decay_mode);
			bChanged = true;
		}

		ImGui::SameLine();

		ImGui::Text("%s", decayModes[decay_mode]);

		if (ImGui::Button("Update") || (bChanged && bAutoUpdate)) {

			handle.set_settings(settings);
			handle.reinitialize();
			settings = handle.get_settings();

			bChanged = false;
		}

		ImGui::SameLine();

		if (ImGui::Button("Refresh")) {
			settings = handle.get_settings();
		}
	}

	ImGui::End();

#ifndef _NDEBUG
	static bool show_imgui_metrics       = false;
	static bool show_implot_metrics      = false;
	static bool show_imgui_style_editor  = false;
	static bool show_implot_style_editor = false;
	static bool show_implot_benchmark    = false;
	if (show_imgui_metrics) {
		ImGui::ShowMetricsWindow(&show_imgui_metrics);
	}
	if (show_implot_metrics) {
		ImPlot::ShowMetricsWindow(&show_implot_metrics);
	}
	if (show_imgui_style_editor) {
		ImGui::Begin("Style Editor (ImGui)", &show_imgui_style_editor);
		ImGui::ShowStyleEditor();
		ImGui::End();
	}
	if (show_implot_style_editor) {
		ImGui::SetNextWindowSize(ImVec2(415,762), ImGuiCond_Appearing);
		ImGui::Begin("Style Editor (ImPlot)", &show_implot_style_editor);
		ImPlot::ShowStyleEditor();
		ImGui::End();
	}

	ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(600, 750), ImGuiCond_FirstUseEver);
	ImGui::Begin("Tools", 0, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Tools")) {
			ImGui::MenuItem("Metrics (ImGui)",       NULL, &show_imgui_metrics);
			ImGui::MenuItem("Metrics (ImPlot)",      NULL, &show_implot_metrics);
			ImGui::MenuItem("Style Editor (ImGui)",  NULL, &show_imgui_style_editor);
			ImGui::MenuItem("Style Editor (ImPlot)", NULL, &show_implot_style_editor);
	
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::End();
#endif

	return 0;
}