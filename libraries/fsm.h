#include "Wire.h"
#include "telemetry.h"
#include "imu.h"
#include "altimeter.h"
#include "gps_receiver.h"
#include "igniter.h"

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

    FSM(Telemetry* telemetry, IMU* imu_sensor, Altimeter* altimeter, GPSReceiver* gps, Igniter* drogueIgniter, Igniter* mainIgniter, uint8_t* loop_frequency);

  private:
    STATE state = STATE::SETUP;
    STATE state_transitions[(int)STATE::Count][(int)EVENT::Count];

    Telemetry* telemetry;
    IMU* imu_sensor;
    Altimeter* altimeter;
    GPSReceiver* gps;
    Igniter* drogueIgniter;
    Igniter* mainIgniter;
    uint8_t* loop_frequency;

    unsigned long launch_time;
    unsigned long ejection_start;
    unsigned long main_deployment_start;
    unsigned long drogue_deployment_time;
    float max_agl = 0;

    void onSetup();
    void onIDLE();
    void onCalibration();
    void onReady();
    void onLaunched();
    void onEjectionTestReady();
    void onEjectionTestEject();
    void onEjectionTestComplete();
    void onAscending();
    void onApogeeTimeout();
    void onDeployingDrogue();
    void onWaitingForMain();
    void onDeployingMain();
    void onRecovering();
};
