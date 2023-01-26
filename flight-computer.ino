#include "bmp3xx_api.h"
#include "lsm9ds1_api.h"
#include "fsm.h"
#include "mosfet_igniter.h"
#include "adafruit_gps_api.h"
#include "definitions.h"
#include "SD.h"

static LSM9DS1_API* imu_sensor = nullptr;
static BMP3XX_API* altimeter = nullptr;
static Telemetry* telemetry = nullptr;
static MosfetIgniter* igniter = nullptr;
static Adafruit_GPS_API* gps = nullptr;
static FSM* fsm = nullptr;
// TODO: improve buzzer code
const int buzzer = 17;
bool sound = false;
uint8_t loop_frequency = 10; // Hz

bool SetupSensorsAndCommunication();

void setup() {
  #if SERIAL_DEBUG
    Serial.begin(115200);
  #endif

  if (!SetupSensorsAndCommunication()) {
    Serial.println("Did not initialize sensors");
    while(1);
  }

#if BUZZER
  noTone(buzzer);
#endif
}

void loop() {
  int timerStart = millis();

  if (telemetry->messageAvailable()) {
    String message = telemetry->receiveMessage();
    if (message.substring(0, 5).equals("SPCMD")) {
      int event;
      if (sscanf(message.c_str(), "SPCMD:%i--", &event) == 1) {
        fsm->process_event((EVENT)event);
      }
    }
  }

#if SERIAL_DEBUG
  if (Serial.available()) {
    int a = Serial.parseInt();
    fsm->process_event((EVENT)a);
  }
#endif

#if BUZZER
  if (sound) {
    sound = false;
    noTone(buzzer);
  } else {
    sound = true;
    tone(buzzer, 500);
  }
#endif

  fsm->runCurrentState();

  int delayTime = (1000/loop_frequency) - (millis() - timerStart);

  delay(max(0, delayTime));
}

bool SetupSensorsAndCommunication() {
#if SD_LOGS
  while(!SD.begin(SD_CS)) {
#if SERIAL_DEBUG
    Serial.println("SD did not initialize");
#endif
  }
#endif

#if BUZZER
  pinMode(buzzer, OUTPUT);
  tone(buzzer, 500);
#endif

  imu_sensor = static_cast<LSM9DS1_API*>(malloc(sizeof(LSM9DS1_API)));
  if (imu_sensor == nullptr) {
    return false;
  }
  *imu_sensor = LSM9DS1_API::getInstance();

  altimeter = static_cast<BMP3XX_API*>(malloc(sizeof(BMP3XX_API)));
  if (altimeter == nullptr) {
    return false;
  }
  *altimeter = BMP3XX_API::getInstance();

  telemetry = static_cast<Telemetry*>(malloc(sizeof(Telemetry)));
  if (telemetry == nullptr) {
    return false;
  }
  *telemetry = Telemetry::getInstance();

  igniter = static_cast<MosfetIgniter*>(malloc(sizeof(MosfetIgniter)));
  if (igniter == nullptr) {
    return false;
  }
  *igniter = MosfetIgniter::getInstance();

  gps = static_cast<Adafruit_GPS_API*>(malloc(sizeof(Adafruit_GPS_API)));
  if (gps == nullptr) {
    return false;
  }
  *gps = Adafruit_GPS_API::getInstance();

  fsm = new FSM(telemetry, imu_sensor, altimeter, gps, igniter, &loop_frequency);
  return fsm != nullptr;
}