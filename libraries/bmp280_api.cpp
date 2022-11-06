#include <Logger.h>

#include "bmp280_api.h"

#define BMP_SCK  (13)
#define BMP_MISO (12) // SD0
#define BMP_MOSI (11) // SD1
#define BMP_CS   (10)

extern SPIN::Log::CFormattedLogger<1024> logger;

Adafruit_BMP280 BMP280_API::sensor;

BMP280_API& BMP280_API::getInstance() {
  static BMP280_API instance;
  return instance;
}

int32_t BMP280_API::altitude_cm() {
  return sensor.readAltitude() * 100;
}

int32_t BMP280_API::pressure() {
  return sensor.readPressure();
}

int32_t BMP280_API::aglCM() {
  return altitude_cm() - ground_level;
}

void BMP280_API::setGroundLevel() {
  ground_level = altitude_cm();
}

int32_t BMP280_API::getGroundLevelCM() {
  return ground_level;
}

void BMP280_API::setup() {
  logger->Verbose("BMP280: Setup: Init");
  if (!sensor.begin()) {
    while(1); //Freeze
  }
  logger->Verbose("BMP280: Setup: Finished");
}

void BMP280_API::calibrate() {
  sensor.setSampling(
    Adafruit_BMP280::MODE_NORMAL,
    Adafruit_BMP280::SAMPLING_X16,
    Adafruit_BMP280::SAMPLING_X16,
    Adafruit_BMP280::FILTER_X2, // TODO: verify that value
    Adafruit_BMP280::STANDBY_MS_1);

  setGroundLevel();
}

BMP280_API::BMP280_API() {}
