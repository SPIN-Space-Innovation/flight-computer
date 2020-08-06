#include "fsm.h"
#include "memoryUsage.h"

#define LAUNCH_AGL_THRESHOLD 5 // meters
#define APOGEE_AGL_DIFF_THRESHOLD 5 // meters
#define LAUNCH_ACCELERATION_THRESHOLD 3 // g
#define TIME_TO_APOGEE 10 // s
#define GRAVITY 9.81 // m/s^2 -- update on other planets
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
    Transition(STATE::CALIBRATION, EVENT::CALIBRATION_COMPLETE, STATE::READY),
    Transition(STATE::READY, EVENT::LAUNCHED, STATE::ASCENDING),

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

    if (state == STATE::DEPLOYING_CHUTE) {
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
  telemetry->send("Altimeter: ground level set to " + String(altimeter->getGroundLevel()) + "m");

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

String internalBoardStatus() {
  float measuredvbat = analogRead(VBATPIN);
  measuredvbat *= 2;
  measuredvbat *= 3.3;
  measuredvbat /= 1024;

  return
    String(millis()) + "," +
    String(freeMemory()/1000) + "," +
    String(measuredvbat);
}

void FSM::runCurrentState() {
  String message = "RAW:" + internalBoardStatus() + "," + state_to_str(state);
  if (state != STATE::SETUP and state != STATE::IDLE and state != STATE::CALIBRATION) {
    message += "," +
      String(altimeter->agl()) + "," +
      String(imu_sensor->accelerationX()) + "," +
      String(imu_sensor->accelerationY()) + "," +
      String(imu_sensor->accelerationZ()) + "," +
      String(imu_sensor->gyroX()) + "," +
      String(imu_sensor->gyroY()) + "," +
      String(imu_sensor->gyroZ()) + "," +
      String(gps->fix()) + "," +
      String(gps->satellites()) + "," +
      gps->latitude() + "," +
      gps->longitude();
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
