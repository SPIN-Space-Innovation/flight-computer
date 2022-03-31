#pragma once

#include <stdint.h>
#include "Arduino.h"

enum class STATE {INVALID_STATE, SETUP, IDLE, CALIBRATION, READY, ARM_BACKUP_DEPLOYER, RESET_BACKUP_DEPLOYER, EJECTION_TEST_READY, EJECTION_TEST_EJECT, EJECTION_TEST_COMPLETE, ASCENDING, APOGEE_TIMEOUT, DEPLOYING_CHUTE, RECOVERING, Count};

enum class EVENT {SETUP_COMPLETE, INIT_CALIBRATION, CALIBRATION_COMPLETE, START_BACKUP_COUNTDOWN, BACKUP_COUNTDOWN_STARTED, RESET_BACKUP_COUNTDOWN, BACKUP_COUNTDOWN_RESETTED, SET_EJECTION_TEST, LAUNCHED, APOGEE_TIMER_TIMEOUT, APOGEE_DETECTED, TRIGGER_FTS, CHUTE_EJECTED, Count};

enum class MESSAGE_TYPE {TELEMETRY, DEBUG};

String state_to_str(STATE state);
String event_to_str(EVENT event);

union TelemetryMessagePayload {
  struct {
    int32_t agl_cm;
    int16_t acceleration_x;
    int16_t acceleration_y;
    int16_t acceleration_z;
    int16_t gyroscope_x;
    int16_t gyroscope_y;
    int16_t gyroscope_z;
    bool gps_fix;
    uint8_t gps_satellites;
    int32_t gps_latitude;
    int32_t gps_longitude;
  };
};

struct TelemetryMessage {
  MESSAGE_TYPE type;
  uint32_t count;
  uint32_t met;
  uint8_t free_memory_kb;
  uint16_t battery_voltage_mv;
  STATE state;
  uint16_t backup_deployer_status;
  TelemetryMessagePayload payload;
  char debug_message[80];
};

String stringifyTelemetryMessage(TelemetryMessage message);
