// This header file contains constants needed to use the USB, serial, and I2C
// protocols for the Tic USB Stepper Motor Controller from Pololu.
//
// Note: Many of these definitions are not considered to be part of the public
// API of libpololu-tic, so they could change or be removed in future versions
// of the library.  The only definitions that are part of the public API are the
// ones referred to in the comments in tic.h.

#ifndef _TIC_PROTOCOL_H
#define _TIC_PROTOCOL_H

#include "config.h"

#define TIC_VENDOR_ID 0x1FFB
#define TIC_PRODUCT_ID_T825 0x00B3
#define TIC_PRODUCT_ID_T834 0x00B5
#define TIC_PRODUCT_ID_T500 0x00BD
#define TIC_PRODUCT_ID_N825 0x00C3
#define TIC_PRODUCT_ID_T249 0x00C9
#define TIC_PRODUCT_ID_36V4 0x00CB

#define TIC_FIRMWARE_MODIFICATION_STRING_INDEX 4

#define TIC_OPERATION_STATE_RESET 0
#define TIC_OPERATION_STATE_DEENERGIZED 2
#define TIC_OPERATION_STATE_SOFT_ERROR 4
#define TIC_OPERATION_STATE_WAITING_FOR_ERR_LINE 6
#define TIC_OPERATION_STATE_STARTING_UP 8
#define TIC_OPERATION_STATE_NORMAL 10

#define TIC_MISC_FLAGS1_ENERGIZED 0
#define TIC_MISC_FLAGS1_POSITION_UNCERTAIN 1
#define TIC_MISC_FLAGS1_FORWARD_LIMIT_ACTIVE 2
#define TIC_MISC_FLAGS1_REVERSE_LIMIT_ACTIVE 3
#define TIC_MISC_FLAGS1_HOMING_ACTIVE 4

#define TIC_ERROR_INTENTIONALLY_DEENERGIZED 0
#define TIC_ERROR_MOTOR_DRIVER_ERROR 1
#define TIC_ERROR_LOW_VIN 2
#define TIC_ERROR_KILL_SWITCH 3
#define TIC_ERROR_REQUIRED_INPUT_INVALID 4
#define TIC_ERROR_SERIAL_ERROR 5
#define TIC_ERROR_COMMAND_TIMEOUT 6
#define TIC_ERROR_SAFE_START_VIOLATION 7
#define TIC_ERROR_ERR_LINE_HIGH 8
#define TIC_ERROR_SERIAL_FRAMING 16
#define TIC_ERROR_SERIAL_RX_OVERRUN 17
#define TIC_ERROR_SERIAL_FORMAT 18
#define TIC_ERROR_SERIAL_CRC 19
#define TIC_ERROR_ENCODER_SKIP 20

#define TIC_INPUT_STATE_NOT_READY 0
#define TIC_INPUT_STATE_INVALID 1
#define TIC_INPUT_STATE_HALT 2
#define TIC_INPUT_STATE_POSITION 3
#define TIC_INPUT_STATE_VELOCITY 4

#define TIC_RESPONSE_DEENERGIZE 0
#define TIC_RESPONSE_HALT_AND_HOLD 1
#define TIC_RESPONSE_DECEL_TO_HOLD 2
#define TIC_RESPONSE_GO_TO_POSITION 3

#define TIC_PIN_NUM_SCL 0
#define TIC_PIN_NUM_SDA 1
#define TIC_PIN_NUM_TX 2
#define TIC_PIN_NUM_RX 3
#define TIC_PIN_NUM_RC 4

#define TIC_PLANNING_MODE_OFF 0
#define TIC_PLANNING_MODE_TARGET_POSITION 1
#define TIC_PLANNING_MODE_TARGET_VELOCITY 2

#define TIC_RESET_POWER_UP 0
#define TIC_RESET_BROWNOUT 1
#define TIC_RESET_RESET_LINE 2
#define TIC_RESET_WATCHDOG 4
#define TIC_RESET_SOFTWARE 8
#define TIC_RESET_STACK_OVERFLOW 16
#define TIC_RESET_STACK_UNDERFLOW 32

#define TIC_PIN_STATE_HIGH_IMPEDANCE 0
#define TIC_PIN_STATE_PULLED_UP 1
#define TIC_PIN_STATE_OUTPUT_LOW 2
#define TIC_PIN_STATE_OUTPUT_HIGH 3

#define TIC_MIN_ALLOWED_BAUD_RATE 200
#define TIC_MAX_ALLOWED_BAUD_RATE 115385

#define TIC_DEFAULT_COMMAND_TIMEOUT 1000
#define TIC_MAX_ALLOWED_COMMAND_TIMEOUT 60000

#define TIC_MAX_ALLOWED_CURRENT 3968
#define TIC_MAX_ALLOWED_CURRENT_T825 3968
#define TIC_MAX_ALLOWED_CURRENT_T834 3456
#define TIC_MAX_ALLOWED_CURRENT_T500 3093
#define TIC_MAX_ALLOWED_CURRENT_CODE_T500 32
#define TIC_MAX_ALLOWED_CURRENT_T249 4480
#define TIC_MAX_ALLOWED_CURRENT_36V4 9095

// Only applies to the T825 and T834.
#define TIC_CURRENT_LIMIT_UNITS_MA 32

#define TIC_CURRENT_LIMIT_UNITS_MA_T249 40

#define TIC_MIN_ALLOWED_ACCEL 100
#define TIC_MAX_ALLOWED_ACCEL 0x7FFFFFFF
#define TIC_MAX_ALLOWED_SPEED 500000000  // 50 kHz

#define TIC_MAX_ALLOWED_ENCODER_PRESCALER 0x7FFFFFFF
#define TIC_MAX_ALLOWED_ENCODER_POSTSCALER 0x7FFFFFFF

#define TIC_SPEED_UNITS_PER_HZ 10000
#define TIC_ACCEL_UNITS_PER_HZ2 100

#define TIC_CONTROL_MODE_SERIAL 0
#define TIC_CONTROL_MODE_STEP_DIR 1
#define TIC_CONTROL_MODE_RC_POSITION 2
#define TIC_CONTROL_MODE_RC_SPEED 3
#define TIC_CONTROL_MODE_ANALOG_POSITION 4
#define TIC_CONTROL_MODE_ANALOG_SPEED 5
#define TIC_CONTROL_MODE_ENCODER_POSITION 6
#define TIC_CONTROL_MODE_ENCODER_SPEED 7

#define TIC_SCALING_DEGREE_LINEAR 0
#define TIC_SCALING_DEGREE_QUADRATIC 1
#define TIC_SCALING_DEGREE_CUBIC 2

#define TIC_STEP_MODE_FULL 0
#define TIC_STEP_MODE_HALF 1
#define TIC_STEP_MODE_MICROSTEP1 0
#define TIC_STEP_MODE_MICROSTEP2 1
#define TIC_STEP_MODE_MICROSTEP4 2
#define TIC_STEP_MODE_MICROSTEP8 3
#define TIC_STEP_MODE_MICROSTEP16 4
#define TIC_STEP_MODE_MICROSTEP32 5
#define TIC_STEP_MODE_MICROSTEP2_100P 6
#define TIC_STEP_MODE_MICROSTEP64 7
#define TIC_STEP_MODE_MICROSTEP128 8
#define TIC_STEP_MODE_MICROSTEP256 9

#define TIC_DECAY_MODE_MIXED 0
#define TIC_DECAY_MODE_SLOW 1
#define TIC_DECAY_MODE_FAST 2

#define TIC_DECAY_MODE_T825_MIXED 0
#define TIC_DECAY_MODE_T825_SLOW 1
#define TIC_DECAY_MODE_T825_FAST 2

#define TIC_DECAY_MODE_T834_MIXED50 0
#define TIC_DECAY_MODE_T834_SLOW 1
#define TIC_DECAY_MODE_T834_FAST 2
#define TIC_DECAY_MODE_T834_MIXED25 3
#define TIC_DECAY_MODE_T834_MIXED75 4

#define TIC_DECAY_MODE_T500_AUTO 0

#define TIC_DECAY_MODE_T249_MIXED 0

#define TIC_AGC_MODE_OFF 0
#define TIC_AGC_MODE_ON 1
#define TIC_AGC_MODE_ACTIVE_OFF 2

#define TIC_AGC_BOTTOM_CURRENT_LIMIT_45 0
#define TIC_AGC_BOTTOM_CURRENT_LIMIT_50 1
#define TIC_AGC_BOTTOM_CURRENT_LIMIT_55 2
#define TIC_AGC_BOTTOM_CURRENT_LIMIT_60 3
#define TIC_AGC_BOTTOM_CURRENT_LIMIT_65 4
#define TIC_AGC_BOTTOM_CURRENT_LIMIT_70 5
#define TIC_AGC_BOTTOM_CURRENT_LIMIT_75 6
#define TIC_AGC_BOTTOM_CURRENT_LIMIT_80 7

#define TIC_AGC_CURRENT_BOOST_STEPS_5 0
#define TIC_AGC_CURRENT_BOOST_STEPS_7 1
#define TIC_AGC_CURRENT_BOOST_STEPS_9 2
#define TIC_AGC_CURRENT_BOOST_STEPS_11 3

#define TIC_AGC_FREQUENCY_LIMIT_OFF 0
#define TIC_AGC_FREQUENCY_LIMIT_225 1
#define TIC_AGC_FREQUENCY_LIMIT_450 2
#define TIC_AGC_FREQUENCY_LIMIT_675 3

#define TIC_AGC_OPTION_MODE 0
#define TIC_AGC_OPTION_BOTTOM_CURRENT_LIMIT 1
#define TIC_AGC_OPTION_CURRENT_BOOST_STEPS 2
#define TIC_AGC_OPTION_FREQUENCY_LIMIT 3

#define TIC_MOTOR_DRIVER_ERROR_NONE 0
#define TIC_MOTOR_DRIVER_ERROR_OVERCURRENT 1
#define TIC_MOTOR_DRIVER_ERROR_OVERTEMPERATURE 2

#define TIC_HP_DECMOD_SLOW 0
#define TIC_HP_DECMOD_SLOW_MIXED 1
#define TIC_HP_DECMOD_FAST 2
#define TIC_HP_DECMOD_MIXED 3
#define TIC_HP_DECMOD_SLOW_AUTO_MIXED 4
#define TIC_HP_DECMOD_AUTO_MIXED 5

#define TIC_HP_DRIVER_ERROR_OTS 0
#define TIC_HP_DRIVER_ERROR_AOCP 1
#define TIC_HP_DRIVER_ERROR_BOCP 2
#define TIC_HP_DRIVER_ERROR_APDF 3
#define TIC_HP_DRIVER_ERROR_BPDF 4
#define TIC_HP_DRIVER_ERROR_UVLO 5
#define TIC_HP_DRIVER_ERROR_VERIFY 7

#define TIC_PIN_PULLUP 7
#define TIC_PIN_ANALOG 6
#define TIC_PIN_FUNC_POSN 0
#define TIC_PIN_FUNC_MASK 0x0F
#define TIC_PIN_FUNC_DEFAULT 0
#define TIC_PIN_FUNC_USER_IO 1
#define TIC_PIN_FUNC_USER_INPUT 2
#define TIC_PIN_FUNC_POT_POWER 3
#define TIC_PIN_FUNC_SERIAL 4
#define TIC_PIN_FUNC_RC 5
#define TIC_PIN_FUNC_ENCODER 6
#define TIC_PIN_FUNC_KILL_SWITCH 7
#define TIC_PIN_FUNC_LIMIT_SWITCH_FORWARD 8
#define TIC_PIN_FUNC_LIMIT_SWITCH_REVERSE 9

#define TIC_CMD_SET_TARGET_POSITION 0xE0
#define TIC_CMD_SET_TARGET_VELOCITY 0xE3
#define TIC_CMD_HALT_AND_SET_POSITION 0xEC
#define TIC_CMD_HALT_AND_HOLD 0x89
#define TIC_CMD_GO_HOME 0x97
#define TIC_CMD_RESET_COMMAND_TIMEOUT 0x8C
#define TIC_CMD_DEENERGIZE 0x86
#define TIC_CMD_ENERGIZE 0x85
#define TIC_CMD_EXIT_SAFE_START 0x83
#define TIC_CMD_ENTER_SAFE_START 0x8F
#define TIC_CMD_RESET 0xB0
#define TIC_CMD_CLEAR_DRIVER_ERROR 0x8A
#define TIC_CMD_SET_MAX_SPEED 0xE6
#define TIC_CMD_SET_STARTING_SPEED 0xE5
#define TIC_CMD_SET_MAX_ACCEL 0xEA
#define TIC_CMD_SET_MAX_DECEL 0xE9
#define TIC_CMD_SET_STEP_MODE 0x94
#define TIC_CMD_SET_CURRENT_LIMIT 0x91
#define TIC_CMD_SET_DECAY_MODE 0x92
#define TIC_CMD_SET_AGC_OPTION 0x98
#define TIC_CMD_GET_VARIABLE 0xA1
#define TIC_CMD_GET_VARIABLE_AND_CLEAR_ERRORS_OCCURRED 0xA2
#define TIC_CMD_GET_SETTING 0xA8
#define TIC_CMD_SET_SETTING 0x13
#define TIC_CMD_REINITIALIZE 0x10
#define TIC_CMD_START_BOOTLOADER 0xFF
#define TIC_CMD_GET_DEBUG_DATA 0x20

#define TIC_VAR_OPERATION_STATE 0x00
#define TIC_VAR_MISC_FLAGS1 0x01
#define TIC_VAR_ERROR_STATUS 0x02
#define TIC_VAR_ERRORS_OCCURRED 0x04
#define TIC_VAR_PLANNING_MODE 0x09
#define TIC_VAR_TARGET_POSITION 0x0A
#define TIC_VAR_TARGET_VELOCITY 0x0E
#define TIC_VAR_STARTING_SPEED 0x12
#define TIC_VAR_MAX_SPEED 0x16
#define TIC_VAR_MAX_DECEL 0x1A
#define TIC_VAR_MAX_ACCEL 0x1E
#define TIC_VAR_CURRENT_POSITION 0x22
#define TIC_VAR_CURRENT_VELOCITY 0x26
#define TIC_VAR_ACTING_TARGET_POSITION 0x2A
#define TIC_VAR_TIME_SINCE_LAST_STEP 0x2E
#define TIC_VAR_DEVICE_RESET 0x32
#define TIC_VAR_VIN_VOLTAGE 0x33
#define TIC_VAR_UP_TIME 0x35
#define TIC_VAR_ENCODER_POSITION 0x39
#define TIC_VAR_RC_PULSE_WIDTH 0x3D
#define TIC_VAR_ANALOG_READING_SCL 0x3F
#define TIC_VAR_ANALOG_READING_SDA 0x41
#define TIC_VAR_ANALOG_READING_TX 0x43
#define TIC_VAR_ANALOG_READING_RX 0x45
#define TIC_VAR_DIGITAL_READINGS 0x47
#define TIC_VAR_PIN_STATES 0x48
#define TIC_VAR_STEP_MODE 0x49
#define TIC_VAR_CURRENT_LIMIT 0x4A
#define TIC_VAR_DECAY_MODE 0x4B
#define TIC_VAR_INPUT_STATE 0x4C
#define TIC_VAR_INPUT_AFTER_AVERAGING 0x4D
#define TIC_VAR_INPUT_AFTER_HYSTERESIS 0x4F
#define TIC_VAR_INPUT_AFTER_SCALING 0x51

#define TIC_VAR_LAST_MOTOR_DRIVER_ERROR 0x55
#define TIC_VAR_AGC_MODE 0x56
#define TIC_VAR_AGC_BOTTOM_CURRENT_LIMIT 0x57
#define TIC_VAR_AGC_CURRENT_BOOST_STEPS 0x58
#define TIC_VAR_AGC_FREQUENCY_LIMIT 0x59

#define TIC_VAR_LAST_HP_DRIVER_ERRORS 0xFF

#define TIC_SETTING_NOT_INITIALIZED 0x00
#define TIC_SETTING_CONTROL_MODE 0x01
#define TIC_SETTING_OPTIONS_BYTE1 0x02
#define TIC_SETTING_DISABLE_SAFE_START 0x03
#define TIC_SETTING_IGNORE_ERR_LINE_HIGH 0x04
#define TIC_SETTING_SERIAL_BAUD_RATE_GENERATOR 0x05
#define TIC_SETTING_SERIAL_DEVICE_NUMBER_LOW 0x07
#define TIC_SETTING_AUTO_CLEAR_DRIVER_ERROR 0x08
#define TIC_SETTING_COMMAND_TIMEOUT 0x09
#define TIC_SETTING_SERIAL_OPTIONS_BYTE 0x0B
#define TIC_SETTING_LOW_VIN_TIMEOUT 0x0C
#define TIC_SETTING_LOW_VIN_SHUTOFF_VOLTAGE 0x0E
#define TIC_SETTING_LOW_VIN_STARTUP_VOLTAGE 0x10
#define TIC_SETTING_HIGH_VIN_SHUTOFF_VOLTAGE 0x12
#define TIC_SETTING_VIN_CALIBRATION 0x14
#define TIC_SETTING_RC_MAX_PULSE_PERIOD 0x16
#define TIC_SETTING_RC_BAD_SIGNAL_TIMEOUT 0x18
#define TIC_SETTING_RC_CONSECUTIVE_GOOD_PULSES 0x1A
#define TIC_SETTING_INVERT_MOTOR_DIRECTION 0x1B
#define TIC_SETTING_INPUT_ERROR_MIN 0x1C
#define TIC_SETTING_INPUT_ERROR_MAX 0x1E
#define TIC_SETTING_INPUT_SCALING_DEGREE 0x20
#define TIC_SETTING_INPUT_INVERT 0x21
#define TIC_SETTING_INPUT_MIN 0x22
#define TIC_SETTING_INPUT_NEUTRAL_MIN 0x24
#define TIC_SETTING_INPUT_NEUTRAL_MAX 0x26
#define TIC_SETTING_INPUT_MAX 0x28
#define TIC_SETTING_OUTPUT_MIN 0x2A
#define TIC_SETTING_INPUT_AVERAGING_ENABLED 0x2E
#define TIC_SETTING_INPUT_HYSTERESIS 0x2F
#define TIC_SETTING_CURRENT_LIMIT_DURING_ERROR 0x31
#define TIC_SETTING_OUTPUT_MAX 0x32
#define TIC_SETTING_SWITCH_POLARITY_MAP 0x36
#define TIC_SETTING_ENCODER_POSTSCALER 0x37
#define TIC_SETTING_SCL_CONFIG 0x3B
#define TIC_SETTING_SDA_CONFIG 0x3C
#define TIC_SETTING_TX_CONFIG 0x3D
#define TIC_SETTING_RX_CONFIG 0x3E
#define TIC_SETTING_RC_CONFIG 0x3F
#define TIC_SETTING_CURRENT_LIMIT 0x40
#define TIC_SETTING_STEP_MODE 0x41
#define TIC_SETTING_DECAY_MODE 0x42
#define TIC_SETTING_STARTING_SPEED 0x43
#define TIC_SETTING_MAX_SPEED 0x47
#define TIC_SETTING_MAX_DECEL 0x4B
#define TIC_SETTING_MAX_ACCEL 0x4F
#define TIC_SETTING_SOFT_ERROR_RESPONSE 0x53
#define TIC_SETTING_SOFT_ERROR_POSITION 0x54
#define TIC_SETTING_ENCODER_PRESCALER 0x58
#define TIC_SETTING_ENCODER_UNLIMITED 0x5C
#define TIC_SETTING_KILL_SWITCH_MAP 0x5D
#define TIC_SETTING_SERIAL_RESPONSE_DELAY 0x5E
#define TIC_SETTING_LIMIT_SWITCH_FORWARD_MAP 0x5F
#define TIC_SETTING_LIMIT_SWITCH_REVERSE_MAP 0x60
#define TIC_SETTING_HOMING_SPEED_TOWARDS 0x61
#define TIC_SETTING_HOMING_SPEED_AWAY 0x65
#define TIC_SETTING_SERIAL_DEVICE_NUMBER_HIGH 0x69
#define TIC_SETTING_SERIAL_ALT_DEVICE_NUMBER 0x6A

#define TIC_SETTING_AGC_MODE 0x6C
#define TIC_SETTING_AGC_BOTTOM_CURRENT_LIMIT 0x6D
#define TIC_SETTING_AGC_CURRENT_BOOST_STEPS 0x6E
#define TIC_SETTING_AGC_FREQUENCY_LIMIT 0x6F

#define TIC_SETTING_HP_ENABLE_UNRESTRICTED_CURRENT_LIMITS 0x6C
#define TIC_SETTING_HP_DRIVER_REGISTERS 0xF2

#define TIC_SERIAL_OPTIONS_BYTE_CRC_FOR_COMMANDS 0
#define TIC_SERIAL_OPTIONS_BYTE_CRC_FOR_RESPONSES 1
#define TIC_SERIAL_OPTIONS_BYTE_7BIT_RESPONSES 2
#define TIC_SERIAL_OPTIONS_BYTE_14BIT_DEVICE_NUMBER 3

#define TIC_OPTIONS_BYTE1_NEVER_SLEEP 0
#define TIC_OPTIONS_BYTE1_AUTO_HOMING 1
#define TIC_OPTIONS_BYTE1_AUTO_HOMING_FORWARD 2

#define TIC_BAUD_RATE_GENERATOR_FACTOR 12000000

#define TIC_MAX_USB_RESPONSE_SIZE 128

#define TIC_INPUT_NULL 0xFFFF

#define TIC_GO_HOME_REVERSE 0
#define TIC_GO_HOME_FORWARD 1

#define TIC_CONTROL_PIN_COUNT 5

#define TIC_HP_DRIVER_SETTING_REGISTER_COUNT 7

#endif
