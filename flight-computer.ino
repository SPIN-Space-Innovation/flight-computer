#include <inttypes.h>

#include "SPIN-Logger.hpp"

#include "bmp3xx_api.h"
#include "lsm9ds1_api.h"
#include "fsm.h"
#include "mosfet_igniter.h"
#include "adafruit_gps_api.h"
#include "definitions.h"


static auto serialSink = SPIN::Log::Sinks::Factory::SerialSinkFactory()
        .SetStream(&Serial)
        .SetColoured(true)
        .Build();

static auto fileSink = SPIN::Log::Sinks::Factory::FileSinkFactory()
        .SetFileNameFormatter("log-%" PRIu32 ".log")
        .Build();

SPIN::Log::CFormattedLogger<1024> logger = SPIN::Log::Factory::CFormattedLoggerFactory()
        .AddSink(&serialSink)
        .AddSink(&fileSink)
        .Build<1024>();


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

void setup() {
  SPIN::Log::SimpleLoggerFactory loggerFactory;
#if SERIAL_DEBUG
  Serial.begin(115200);
  loggerFactory.AddSink(SPIN::Log::Sinks::SerialSink(SPIN::Log::Sinks::SerialSinkConfigurations()
    .SetStream(&Serial)
    .SetColoured(true)));
#endif

#if SD_LOGS
  SD.begin(SD_CS);
  loggerFactory.AddSink(SPIN::Log::Sinks::FileSink(SPIN::Log::Sinks::FileSinkConfigurations()
    .SetFileNameFormatter("log-%" PRIu32 ".log")));
#endif
  logger = loggerFactory.Build();

#if BUZZER
  pinMode(buzzer, OUTPUT);
  tone(buzzer, 500);
#endif

  fsm = new FSM(&telemetry, &imu_sensor, &altimeter, &gps, &igniter, &loop_frequency);

#if BUZZER
  noTone(buzzer);
#endif
}

void loop() {
  int timerStart = millis();

  if (telemetry.messageAvailable()) {
    String message = telemetry.receiveMessage();
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
