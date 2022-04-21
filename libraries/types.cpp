#include <stdint.h>
#include "types.h"

String state_to_str(STATE state) {
  String names[] = {"INVALID_STATE", "SETUP", "IDLE", "CALIBRATION", "READY", "ARM_BACKUP_DEPLOYER", "RESET_BACKUP_DEPLOYER", "EJECTION_TEST_READY",  "EJECTION_TEST_EJECT", "EJECTION_TEST_COMPLETE", "ASCENDING", "APOGEE_TIMEOUT", "DEPLOYING_CHUTE", "RECOVERING"};
  if ((int)state >= (int)STATE::Count or (int)state < 0) {
    return "Unknown";
  }
  return names[(int)state];
}

String event_to_str(EVENT event) {
  String names[] = {"SETUP_COMPLETE", "INIT_CALIBRATION", "CALIBRATION_COMPLETE", "START_BACKUP_COUNTDOWN", "BACKUP_COUNTDOWN_STARTED", "RESET_BACKUP_COUNTDOWN",  "BACKUP_COUNTDOWN_RESETTED", "SET_EJECTION_TEST", "LAUNCHED", "APOGEE_TIMER_TIMEOUT", "APOGEE_DETECTED", "TRIGGER_FTS", "CHUTE_EJECTED"};
  if ((int)event >= (int)EVENT::Count or (int)event < 0) {
    return "Unknown";
  }
  return names[(int)event];
}

String stringifyTelemetryMessage(TelemetryMessage message) {
  String message_str =
    String((uint8_t)message.type) + "," +
    String(message.count) + ",";

  if (message.type == MESSAGE_TYPE::DEBUG) {
    message_str += message.debug_message;
  } else if (message.type == MESSAGE_TYPE::TELEMETRY) {
    message_str +=
      String(message.met) + "," +
      String(message.free_memory_kb) + "," +
      String(message.battery_voltage_mv) + "," +
      String(state_to_str(message.state)) + "," +
      String(message.sd_logs_enabled) + "," +
      String(message.selected_igniter);

    if (message.state != STATE::SETUP and message.state != STATE::IDLE and message.state != STATE::CALIBRATION) {
      message_str += "," +
        String(message.payload.agl_cm) + "," +
        String(message.payload.pressure) + "," +
        String(message.payload.acceleration_x) + "," +
        String(message.payload.acceleration_y) + "," +
        String(message.payload.acceleration_z) + "," +
        String(message.payload.gyroscope_x) + "," +
        String(message.payload.gyroscope_y) + "," +
        String(message.payload.gyroscope_z) + "," +
        String(message.payload.gps_fix) + "," +
        String(message.payload.gps_satellites) + "," +
        String(message.payload.gps_latitude) + "," +
        String(message.payload.gps_longitude);
    }
  }

  return message_str;
}
