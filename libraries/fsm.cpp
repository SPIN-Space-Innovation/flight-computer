#include "fsm.h"
#include "memoryUsage.h"

#define LAUNCH_AGL_THRESHOLD 5 // meters
#define APOGEE_AGL_DIFF_THRESHOLD 5 // meters
#define LAUNCH_ACCELERATION_THRESHOLD 3 // g
#define TIME_TO_APOGEE 10 // s
#define GRAVITY 981 // cm/s^2 -- update on other planets
#define VBATPIN A7
#define EJECTION_TIMEOUT 4000 // ms

FSM::FSM(Telemetry* telemetry, IMU* imu_sensor, Altimeter* altimeter, GPSReceiver* gps, Igniter* igniter)
  : telemetry(telemetry)
  , imu_sensor(imu_sensor)
  , altimeter(altimeter)
  , gps(gps)
  , igniter(igniter)
{
  Transition flight_state_transitions[] = {
    // Ground
    Transition(STATE::SETUP, EVENT::SETUP_COMPLETE, STATE::IDLE),
    Transition(STATE::IDLE, EVENT::INIT_CALIBRATION, STATE::CALIBRATION),
    Transition(STATE::READY, EVENT::INIT_CALIBRATION, STATE::CALIBRATION),
    Transition(STATE::READY, EVENT::SET_EJECTION_TEST, STATE::EJECTION_TEST_READY),
    Transition(STATE::CALIBRATION, EVENT::CALIBRATION_COMPLETE, STATE::READY),
    Transition(STATE::READY, EVENT::LAUNCHED, STATE::ASCENDING),

    // Ejection Test
    Transition(STATE::EJECTION_TEST_READY, EVENT::TRIGGER_FTS, STATE::EJECTION_TEST_EJECT),
    Transition(STATE::EJECTION_TEST_EJECT, EVENT::CHUTE_EJECTED, STATE::EJECTION_TEST_COMPLETE),

    // Flying
    Transition(STATE::ASCENDING, EVENT::APOGEE_TIMER_TIMEOUT, STATE::APOGEE_TIMEOUT),
    Transition(STATE::ASCENDING, EVENT::INIT_CALIBRATION, STATE::CALIBRATION),
    Transition(STATE::ASCENDING, EVENT::APOGEE_DETECTED, STATE::DEPLOYING_CHUTE),
    Transition(STATE::APOGEE_TIMEOUT, EVENT::APOGEE_DETECTED, STATE::DEPLOYING_CHUTE),
    Transition(STATE::DEPLOYING_CHUTE, EVENT::CHUTE_EJECTED, STATE::RECOVERING),

    // FTS from all states
    Transition(STATE::SETUP, EVENT::TRIGGER_FTS, STATE::DEPLOYING_CHUTE),
    Transition(STATE::IDLE, EVENT::TRIGGER_FTS, STATE::DEPLOYING_CHUTE),
    Transition(STATE::CALIBRATION, EVENT::TRIGGER_FTS, STATE::DEPLOYING_CHUTE),
    Transition(STATE::READY, EVENT::TRIGGER_FTS, STATE::DEPLOYING_CHUTE),
    Transition(STATE::ASCENDING, EVENT::TRIGGER_FTS, STATE::DEPLOYING_CHUTE),
    Transition(STATE::APOGEE_TIMEOUT, EVENT::TRIGGER_FTS, STATE::DEPLOYING_CHUTE),
    Transition(STATE::RECOVERING, EVENT::TRIGGER_FTS, STATE::DEPLOYING_CHUTE)
  };

  register_state_transitions(flight_state_transitions);
}

template<size_t N>
void FSM::register_state_transitions(Transition (&transitions)[N]) {
  for (int i = 0; i < (int)STATE::Count; ++i) {
    for (int j = 0; j < (int)EVENT::Count; ++j) {
      state_transitions[i][j] = STATE::INVALID_STATE;
    }
  }

  for (int i = 0; i < N; ++i) {
    state_transitions[(int)transitions[i].src_state][(int)transitions[i].event] = transitions[i].dst_state;
  }
}

void FSM::process_event(EVENT event) {
  telemetry->send("FSM Event: " + event_to_str(event));

  if (state_transitions[(int)state][(int)event] != STATE::INVALID_STATE) {
    state = state_transitions[(int)state][(int)event];

    if (state == STATE::DEPLOYING_CHUTE || state == STATE::EJECTION_TEST_EJECT) {
      ejection_start = millis();
    }
  } else {
    telemetry->send("No transition from state " + state_to_str(state) + " with event " + event_to_str(event));
  }

  if (event == EVENT::LAUNCHED) {
    // TODO: not the best place to do that
    launch_time = millis();
  }
}

void FSM::onSetup() {
  telemetry->setup();
  telemetry->send("Hello!");

  Wire.begin();
  telemetry->send("Wire begin");

  telemetry->send("Setting up IMU..");
  imu_sensor->setup();
  telemetry->send("IMU setup complete.");

  telemetry->send("Setting up Altimeter..");
  altimeter->setup();
  telemetry->send("Altimeter setup complete.");

  telemetry->send("Setting up GPS..");
  gps->setup();
  telemetry->send("GPS setup complete.");

  telemetry->send("Setting up Igniter..");
  igniter->setup();
  telemetry->send("Igniter setup complete.");

  process_event(EVENT::SETUP_COMPLETE);
}

void FSM::onIDLE() {}

void FSM::onCalibration() {
  telemetry->send("Altimeter: calibrating..");
  altimeter->calibrate();
  telemetry->send("Altimeter: ground level set to " + String((float)altimeter->getGroundLevelCM() / 100) + "m");

  telemetry->send("IMU: calibrating..");
  imu_sensor->calibrate();
  telemetry->send("IMU: calibrated.");

  process_event(EVENT::CALIBRATION_COMPLETE);
}

void FSM::onReady() {
  if (altimeter->agl() > LAUNCH_AGL_THRESHOLD or
      imu_sensor->accelerationX() / GRAVITY > LAUNCH_ACCELERATION_THRESHOLD) {
    process_event(EVENT::LAUNCHED);
  }
}

void FSM::onEjectionTestReady() {}

void FSM::onEjectionTestEject() {
  onDeployingChute();
  // set high frequency
}

void FSM::onEjectionTestComplete() {}

void FSM::onAscending() {
  float agl = altimeter->agl();
  if (agl > max_agl) {
    max_agl = agl;
  }

  if (max_agl - agl >= APOGEE_AGL_DIFF_THRESHOLD) {
    process_event(EVENT::APOGEE_DETECTED);
  } else if (millis() - launch_time > TIME_TO_APOGEE * 1000) {
    process_event(EVENT::APOGEE_TIMER_TIMEOUT);
  }
  // TODO: check IMU ?
}

void FSM::onApogeeTimeout() {
  process_event(EVENT::APOGEE_DETECTED);
}

void FSM::onDeployingChute() {
  igniter->enable();
  if (millis() - ejection_start > EJECTION_TIMEOUT) {
    igniter->disable();
    process_event(EVENT::CHUTE_EJECTED);
  }
}

void FSM::onRecovering() {
  // TODO: celebrate
}

uint16_t get_battery_voltage_mv() {
  float measuredvbat = analogRead(VBATPIN);
  measuredvbat *= 2;
  measuredvbat *= 3.3;
  return measuredvbat;
}

void FSM::runCurrentState() {
  TelemetryMessage message;
  TelemetryMessagePayload payload;

  message.type = MESSAGE_TYPE::TELEMETRY;
  message.met = millis();
  message.free_memory_kb = freeMemory()/1000;
  message.battery_voltage_mv = get_battery_voltage_mv();
  message.state = state;

  if (state != STATE::SETUP and state != STATE::IDLE and state != STATE::CALIBRATION) {
    payload.agl_cm = altimeter->aglCM();
    payload.acceleration_x = imu_sensor->accelerationX();
    payload.acceleration_y = imu_sensor->accelerationY();
    payload.acceleration_z = imu_sensor->accelerationZ();
    payload.gyroscope_x = imu_sensor->gyroX();
    payload.gyroscope_y = imu_sensor->gyroY();
    payload.gyroscope_z = imu_sensor->gyroZ();
    payload.gps_fix = gps->fix();
    payload.gps_satellites = gps->satellites();
    message.payload = payload;
  }
  telemetry->send(message);

  switch (state) {
    case STATE::SETUP:
      onSetup();
      break;
    case STATE::IDLE:
      onIDLE();
      break;
    case STATE::CALIBRATION:
      onCalibration();
      break;
    case STATE::READY:
      onReady();
      break;
    case STATE::EJECTION_TEST_READY:
      onEjectionTestReady();
      break;
    case STATE::EJECTION_TEST_EJECT:
      onEjectionTestEject();
      break;
    case STATE::EJECTION_TEST_COMPLETE:
      onEjectionTestComplete();
      break;
    case STATE::ASCENDING:
      onAscending();
      break;
    case STATE::APOGEE_TIMEOUT:
      onApogeeTimeout();
      break;
    case STATE::DEPLOYING_CHUTE:
      onDeployingChute();
      break;
    case STATE::RECOVERING:
      onRecovering();
      break;
  }
}
