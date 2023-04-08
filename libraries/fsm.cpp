#include "fsm.h"
#include "definitions.h"
#include "memoryUsage.h"

#define LAUNCH_AGL_THRESHOLD 5 // meters
#define APOGEE_AGL_DIFF_THRESHOLD 5 // meters
#define LAUNCH_ACCELERATION_THRESHOLD 3 // g
#define TIME_TO_APOGEE 15 // s
#define BOOST_TIME 12 // s
#define GRAVITY 981 // cm/s^2 -- Earth
/*
#define GRAVITY 372 // cm/s^2 -- Mars
#define GRAVITY 162 // cm/s^2 -- Moon
*/
#define MAIN_CHUTE_ALTITUDE 200 // meters
#define MAIN_CHUTE_TIMEOUT 10 // seconds
#define VBATPIN A7
#define PYRO_TIMEOUT 4000 // ms

FSM::FSM(
  Telemetry* telemetry,
  IMU* imu_sensor,
  Altimeter* altimeter,
  GPSReceiver* gps,
  Igniter* drogueIgniter,
  Igniter* mainIgniter,
  uint8_t* loop_frequency)
  : telemetry(telemetry)
  , imu_sensor(imu_sensor)
  , altimeter(altimeter)
  , gps(gps)
  , drogueIgniter(drogueIgniter)
  , mainIgniter(mainIgniter)
  , loop_frequency(loop_frequency)
{
  Transition flight_state_transitions[] = {
    // Ground
    Transition(STATE::SETUP, EVENT::SETUP_COMPLETE, STATE::IDLE),
    Transition(STATE::IDLE, EVENT::INIT_CALIBRATION, STATE::CALIBRATION),
    Transition(STATE::READY, EVENT::INIT_CALIBRATION, STATE::CALIBRATION),
    Transition(STATE::READY, EVENT::SET_EJECTION_TEST, STATE::EJECTION_TEST_READY),
    Transition(STATE::CALIBRATION, EVENT::CALIBRATION_COMPLETE, STATE::READY),
    Transition(STATE::READY, EVENT::LAUNCHED, STATE::ASCENDING),
    Transition(STATE::READY, EVENT::GO_IDLE, STATE::IDLE),

    // Ejection Test
    Transition(STATE::EJECTION_TEST_READY, EVENT::TRIGGER_FTS, STATE::EJECTION_TEST_EJECT),
    Transition(STATE::EJECTION_TEST_EJECT, EVENT::DROGUE_DEPLOYED, STATE::EJECTION_TEST_COMPLETE),

    // Flying
    Transition(STATE::ASCENDING, EVENT::APOGEE_TIMER_TIMEOUT, STATE::APOGEE_TIMEOUT),
    Transition(STATE::ASCENDING, EVENT::INIT_CALIBRATION, STATE::CALIBRATION),
    Transition(STATE::ASCENDING, EVENT::APOGEE_DETECTED, STATE::DEPLOYING_DROGUE),
    Transition(STATE::APOGEE_TIMEOUT, EVENT::APOGEE_DETECTED, STATE::DEPLOYING_DROGUE),
    Transition(STATE::DEPLOYING_DROGUE, EVENT::DROGUE_DEPLOYED, STATE::WAITING_FOR_MAIN),
    Transition(STATE::WAITING_FOR_MAIN, EVENT::MAIN_CHUTE_TIMER_TIMEOUT, STATE::DEPLOYING_MAIN),
    Transition(STATE::WAITING_FOR_MAIN, EVENT::REACHED_MAIN_CHUTE_ALTITUDE, STATE::DEPLOYING_MAIN),
    Transition(STATE::DEPLOYING_MAIN, EVENT::MAIN_DEPLOYED, STATE::RECOVERING),

    // FTS from all states except IDLE
    Transition(STATE::SETUP, EVENT::TRIGGER_FTS, STATE::DEPLOYING_DROGUE),
    Transition(STATE::CALIBRATION, EVENT::TRIGGER_FTS, STATE::DEPLOYING_DROGUE),
    Transition(STATE::READY, EVENT::TRIGGER_FTS, STATE::DEPLOYING_DROGUE),
    Transition(STATE::ASCENDING, EVENT::TRIGGER_FTS, STATE::DEPLOYING_DROGUE),
    Transition(STATE::APOGEE_TIMEOUT, EVENT::TRIGGER_FTS, STATE::DEPLOYING_DROGUE),
    Transition(STATE::RECOVERING, EVENT::TRIGGER_FTS, STATE::DEPLOYING_DROGUE)
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

    if (state == STATE::DEPLOYING_DROGUE || state == STATE::EJECTION_TEST_EJECT) {
      ejection_start = millis();
    }

    if (state == STATE::DEPLOYING_MAIN) {
      main_deployment_start = millis();
    }

    if (event == EVENT::LAUNCHED) {
      onLaunched();
    }
  } else {
    telemetry->send("No transition from state " + state_to_str(state) + " with event " + event_to_str(event));
  }
}

void FSM::onLaunched() {
  // TODO: not the best place to do that
  launch_time = millis();

  telemetry->setRadioThrottle(1000);
  *loop_frequency = 100;
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

  telemetry->send("Setting up Drogue Igniter..");
  drogueIgniter->setup();
  telemetry->send("Drogue Igniter setup complete.");

  telemetry->send("Setting up Main Igniter..");
  mainIgniter->setup();
  telemetry->send("Main Igniter setup complete.");

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
  telemetry->setRadioThrottle(0);
  *loop_frequency = 10;

  if (altimeter->agl() > LAUNCH_AGL_THRESHOLD or
      (imu_sensor->accelerationX() / GRAVITY) * -1 > LAUNCH_ACCELERATION_THRESHOLD) {
    process_event(EVENT::LAUNCHED);
  }
}

void FSM::onEjectionTestReady() {}

void FSM::onEjectionTestEject() {
  telemetry->setRadioThrottle(1000);
  *loop_frequency = 100;
  onDeployingDrogue();
}

void FSM::onEjectionTestComplete() {
  telemetry->setRadioThrottle(0);
  *loop_frequency = 10;
}

void FSM::onAscending() {
  float agl = altimeter->agl();
  if (agl > max_agl) {
    max_agl = agl;
  }

  if (max_agl - agl >= APOGEE_AGL_DIFF_THRESHOLD &&
    millis() - launch_time > BOOST_TIME * 1000) {
    process_event(EVENT::APOGEE_DETECTED);
  } else if (millis() - launch_time > TIME_TO_APOGEE * 1000) {
    process_event(EVENT::APOGEE_TIMER_TIMEOUT);
  }
  // TODO: check IMU ?
}

void FSM::onApogeeTimeout() {
  process_event(EVENT::APOGEE_DETECTED);
}

void FSM::onDeployingDrogue() {
  drogueIgniter->enable();
  if (millis() - ejection_start > PYRO_TIMEOUT) {
    drogueIgniter->disable();
    drogue_deployment_time = millis();
    process_event(EVENT::DROGUE_DEPLOYED);
    telemetry->setRadioThrottle(0);
    *loop_frequency = 10;
  }
}

void FSM::onWaitingForMain() {
  if (millis() - drogue_deployment_time >= MAIN_CHUTE_TIMEOUT * 1000) {
    process_event(EVENT::MAIN_CHUTE_TIMER_TIMEOUT);
  }
}

void FSM::onDeployingMain() {
  mainIgniter->enable();
  if (millis() - main_deployment_start > PYRO_TIMEOUT) {
    mainIgniter->disable();
    process_event(EVENT::MAIN_DEPLOYED);
    telemetry->setRadioThrottle(0);
    *loop_frequency = 10;
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
#if SD_LOGS
  message.sd_logs_enabled = true;
#else
  message.sd_logs_enabled = false;
#endif

  if (state != STATE::SETUP and state != STATE::IDLE and state != STATE::CALIBRATION) {
    payload.agl_cm = altimeter->aglCM();
    payload.pressure = altimeter->pressure();
    payload.temperature = altimeter->temperature();

    payload.acceleration_x = imu_sensor->accelerationX();
    payload.acceleration_y = imu_sensor->accelerationY();
    payload.acceleration_z = imu_sensor->accelerationZ();
    payload.gyroscope_x = imu_sensor->gyroX();
    payload.gyroscope_y = imu_sensor->gyroY();
    payload.gyroscope_z = imu_sensor->gyroZ();

    payload.gps_fix = gps->fix();
    payload.gps_satellites = gps->satellites();
    payload.gps_latitude = gps->latitude();
    payload.gps_longitude = gps->longitude();
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
    case STATE::DEPLOYING_DROGUE:
      onDeployingDrogue();
      break;
    case STATE::WAITING_FOR_MAIN:
      onWaitingForMain();
      break;
    case STATE::DEPLOYING_MAIN:
      onDeployingMain();
      break;
    case STATE::RECOVERING:
      onRecovering();
      break;
  }
}
