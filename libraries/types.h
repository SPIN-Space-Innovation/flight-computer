#pragma once

#include <stdint.h>
#include "Arduino.h"


/**
 * @see state_to_str
 * @see TelemetryMessage
 * 
 * @author themicp
 */
enum class STATE : uint8_t {
  INVALID_STATE, /*!< Invalid state. */
  SETUP, /*!< Program still setting up. */
  IDLE, /*!< Program is idling. */
  CALIBRATION, /*!< Program is still calibrating. */
  READY, /*!< Program is ready to run normally. */
  EJECTION_TEST_READY, /*!< */
  EJECTION_TEST_EJECT, /*!< */
  EJECTION_TEST_COMPLETE, /*!< */
  ASCENDING, /*!< The rocket has detected that it is ascending. */
  APOGEE_TIMEOUT, /*!< */
  DEPLOYING_DROGUE, /*!< */
  WAITING_FOR_MAIN, /*!< */
  DEPLOYING_MAIN, /*!< */
  RECOVERING, /*!< The recovery process of the rocket has began. */
  Count /*!< */
};

enum class EVENT : uint8_t {
  SETUP_COMPLETE, /*!< */
  INIT_CALIBRATION, /*!< */
  CALIBRATION_COMPLETE, /*!< */
  SET_EJECTION_TEST, /*!< */
  LAUNCHED, /*!< */
  APOGEE_TIMER_TIMEOUT, /*!< */
  APOGEE_DETECTED, /*!< */
  TRIGGER_FTS, /*!< */
  DROGUE_DEPLOYED, /*!< */
  MAIN_CHUTE_TIMER_TIMEOUT, /*!< */
  REACHED_MAIN_CHUTE_ALTITUDE, /*!< */
  MAIN_DEPLOYED, /*!< */
  GO_IDLE, /*!< */
  Count /*!< */
};

enum class MESSAGE_TYPE {
  TELEMETRY, /*!< Data from sensors and state. */
  DEBUG /*!< Debug message. Usefull for detecting problems. */
};


/**
 * @brief Strigify STATE.
 * Returns Arduino String version of enum class STATE.
 * 
 * @param state Enum class STATE to be stringified.
 * @return String Stringified version of enum.
 * 
 * @see STATE
 * @examplecode
 * @code{.cpp}
 * String stringifiedState = state_to_str(STATE::READY);
 * Serial.println(stringifiedState);
 * @endcode
 * 
 * @author themicp
 */
String state_to_str(STATE state);

/**
 * @brief Strigify EVENT.
 * Returns Arduino String version of enum class EVENT.
 * 
 * @param event Enum class EVENT to be stringified.
 * @return String Stringified version of enum.
 * 
 * @see EVENT
 * @examplecode
 * @code{.cpp}
 * String stringifiedEvent = state_to_str(EVENT::LAUNCHED);
 * Serial.println(stringifiedEvent);
 * @endcode
 * 
 * @author themicp
 */
String event_to_str(EVENT event);


/**
 * @brief Used to store data from the sensors.
 * 
 * @see TelemetryMessage
 * 
 * @author themicp
 */
struct TelemetryMessagePayload {
  int32_t agl_cm; /*!< Altitude from ground level in m. */
  int32_t pressure; /*!< Pressure returned from sensor in Pascal. */
  int16_t temperature; /*!< Temperature returned from sensor in Celcius * 100. */
  int16_t acceleration_x; /*!< Acceleration in the x axis returned from sensor in cm / s^2. */
  int16_t acceleration_y; /*!< Acceleration in the y axis returned from sensor in cm / s^2. */
  int16_t acceleration_z; /*!< Acceleration in the z axis returned from sensor in cm / s^2. */
  int16_t gyroscope_x; /*!< Angular velocity in the x axis returned from sensor in degrees / s. */
  int16_t gyroscope_y; /*!< Angular velocity in the y axis returned from sensor in degrees / s. */
  int16_t gyroscope_z; /*!< Angular velocity in the z axis returned from sensor in degrees / s. */
  bool gps_fix; /*!< GPS Module has direct access to at least 3 satelites. */
  uint8_t gps_satellites; /*!< Number of satelites availabe to the GPS Module. */
  int32_t gps_latitude; /*!< Latitude returned from the GPS module in DDMM.MMMM. */
  int32_t gps_longitude; /*!< Longitude returned from the GPS module in DDDMM.MMMM. */
};

/**
 * @brief Used to transfer data between flight computer and ground station.
 * 
 * @see stringifyTelemetryMessage
 * 
 * @author themicp
 */
struct TelemetryMessage {
  MESSAGE_TYPE type; /*!< Determines if the Message is for data or Debug purposes. */
  uint32_t count; /*!< Incremental number to determine the number of packets lost in transmission. */
  uint32_t met; /*!< Milliseconds from the boot time of the microcontroller. */
  uint8_t free_memory_kb; /*!< Unused memory from the stack and heap. */
  uint16_t battery_voltage_mv; /*!< Voltage of the battery. Usefull in determining battery percentage. */
  STATE state; /*!< State of the microcontroller and the rocket in general. */
  bool sd_logs_enabled; /*!< True if the logging output will be saved to the SD as well. */
  TelemetryMessagePayload payload; /*!< Data returned from the sensors. */
  char debug_message[80]; /*!< Debug message c style string. */
};

/**
 * @brief Strigify TelemetryMessage.
 * Returns Arduino String version of struct TelemetryMessage. <br />
 * Used to store Telemetry message to the SD. <br />
 * To be depricated and replaced by the new Logger.
 * 
 * @param message struct TelemetryMessage to be stringified.
 * @return String Stringified version of struct.
 * 
 * @see TelemetryMessage
 * @examplecode
 * @code{.cpp}
 * struct TelemetryMessage message = { 0 };
 * ...
 * Serial.println(stringifyTelemetryMessage(message));
 * @endcode
 * 
 * @author themicp
 */
String stringifyTelemetryMessage(TelemetryMessage message);
