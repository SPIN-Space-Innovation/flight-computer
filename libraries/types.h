#pragma once

#include <stdint.h>
#include "Arduino.h"

enum class STATE {INVALID_STATE, SETUP, IDLE, CALIBRATION, READY, ASCENDING, APOGEE_TIMEOUT, DEPLOYING_CHUTE, RECOVERING, Count};

enum class EVENT {SETUP_COMPLETE, INIT_CALIBRATION, CALIBRATION_COMPLETE, LAUNCHED, APOGEE_TIMER_TIMEOUT, APOGEE_DETECTED, TRIGGER_FTS, CHUTE_EJECTED, Count};

String state_to_str(STATE state);
String event_to_str(EVENT event);

union TelemetryMessagePayload {
  uint8_t debug_code;
  struct {
    float agl;
    float acceleration_x;
    float acceleration_y;
    float acceleration_z;
    float gyroscope_x;
    float gyroscope_y;
    float gyroscope_z;
    bool gps_fix;
    uint8_t gps_satellites;
  };
};

struct TelemetryMessage {
  uint32_t met;
  uint8_t free_memory_kb;
  uint16_t battery_voltage_mv;
  STATE state;
  TelemetryMessagePayload payload;
};
