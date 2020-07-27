#include "Wire.h"
#include "telemetry.h"
#include "imu.h"
#include "altimeter.h"
#include "gps_receiver.h"
#include "igniter.h"

enum class STATE {INVALID_STATE, SETUP, IDLE, CALIBRATION, READY, ASCENDING, APOGEE_TIMEOUT, DEPLOYING_CHUTE, RECOVERING, Count};

enum class EVENT {SETUP_COMPLETE, INIT_CALIBRATION, CALIBRATION_COMPLETE, LAUNCHED, APOGEE_TIMER_TIMEOUT, APOGEE_DETECTED, TRIGGER_FTS, CHUTE_EJECTED, Count};

class Transition {
  public:
    STATE src_state;
    EVENT event;
    STATE dst_state;

    Transition(STATE src_state, EVENT event, STATE dst_state)
      : src_state(src_state)
      , event(event)
      , dst_state(dst_state)
    {}
};

class FSM {
  public:
    template<size_t N>
    void register_state_transitions(Transition (&transitions)[N]);

    void process_event(EVENT event);
    void runCurrentState();

    FSM(Telemetry* telemetry, IMU* imu_sensor, Altimeter* altimeter, GPSReceiver* gps, Igniter* igniter);

  private:
    STATE state = STATE::SETUP;
    STATE state_transitions[(int)STATE::Count][(int)EVENT::Count];

    Telemetry* telemetry;
    IMU* imu_sensor;
    Altimeter* altimeter;
    GPSReceiver* gps;
    Igniter* igniter;

    unsigned long launch_time;
    unsigned long ejection_start;
    float max_agl = 0;

    void onSetup();
    void onIDLE();
    void onCalibration();
    void onReady();
    void onAscending();
    void onApogeeTimeout();
    void onDeployingChute();
    void onRecovering();
};
