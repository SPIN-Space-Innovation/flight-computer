#include "bmp3xx_api.h"
#include "lsm9ds1_api.h"
#include "fsm.h"
#include "mosfet_igniter.h"
#include "adafruit_gps_api.h"

LSM9DS1_API imu_sensor = LSM9DS1_API::getInstance();
BMP3XX_API altimeter = BMP3XX_API::getInstance();
Telemetry telemetry = Telemetry::getInstance();
MosfetIgniter igniter = MosfetIgniter::getInstance();
Adafruit_GPS_API gps = Adafruit_GPS_API::getInstance();
FSM *fsm;
// TODO: improve buzzer code
const int buzzer = 17;
bool sound = false;
uint8_t loop_frequency = 10; // Hz

#define SERIAL_DEBUG false

void setup() {
#if SERIAL_DEBUG
  Serial.begin(115200);
#endif

  pinMode(buzzer, OUTPUT);
  tone(buzzer, 500);
  fsm = new FSM(&telemetry, &imu_sensor, &altimeter, &gps, &igniter, &loop_frequency);
  noTone(buzzer);
}

void loop() {
  int timerStart = millis();

  if (telemetry.messageAvailable()) {
    String message = telemetry.receiveMessage();
    if (message.substring(0, 3).equals("CMD")) {
      int event = message.substring(4).toInt();
      fsm->process_event((EVENT)event);
    }
  }

#if SERIAL_DEBUG
  if (Serial.available()) {
    int a = Serial.parseInt();
    fsm->process_event((EVENT)a);
  }
#endif

  if (sound) {
    sound = false;
    noTone(buzzer);
  } else {
    sound = true;
    tone(buzzer, 500);
  }
  fsm->runCurrentState();

  int delayTime = (1000/loop_frequency) - (millis() - timerStart);

  delay(max(0, delayTime));
}
