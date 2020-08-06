#include <stdint.h>
#include "types.h"

String state_to_str(STATE state) {
  String names[] = {"INVALID_STATE", "SETUP", "IDLE", "CALIBRATION", "READY", "ASCENDING", "APOGEE_TIMEOUT", "DEPLOYING_CHUTE", "RECOVERING"};
  if ((int)state >= (int)STATE::Count or (int)state < 0) {
    return "Unknown";
  }
  return names[(int)state];
}

String event_to_str(EVENT event) {
  String names[] = {"SETUP_COMPLETE", "INIT_CALIBRATION", "CALIBRATION_COMPLETE", "LAUNCHED", "APOGEE_TIMER_TIMEOUT", "APOGEE_DETECTED", "TRIGGER_FTS", "CHUTE_EJECTED"};
  if ((int)event >= (int)EVENT::Count or (int)event < 0) {
    return "Unknown";
  }
  return names[(int)event];
}
