#include "Adafruit_BMP280.h"
#include "bmp280_api.h"

#define BMP_SCK  (13)
#define BMP_MISO (12) // SD0
#define BMP_MOSI (11) // SD1
#define BMP_CS   (10)

static BMP280_API* instance = nullptr;
static Adafruit_BMP280* sensor = nullptr;
static int32_t ground_level = 0;

BMP280_API& BMP280_API::getInstance() {
  if (instance == nullptr)
  {
    instance = new BMP280_API();
    sensor = new Adafruit_BMP280();
  }
  return *instance;
}

int32_t BMP280_API::altitude_cm() {
  return sensor->readAltitude() * 100;
}

int32_t BMP280_API::pressure() {
  return sensor->readPressure();
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
  if (!sensor->begin()) {
    while(1); //Freeze
  }
}

void BMP280_API::calibrate() {
  sensor->setSampling(
    Adafruit_BMP280::MODE_NORMAL,
    Adafruit_BMP280::SAMPLING_X16,
    Adafruit_BMP280::SAMPLING_X16,
    Adafruit_BMP280::FILTER_X2, // TODO: verify that value
    Adafruit_BMP280::STANDBY_MS_1);

  setGroundLevel();
}

BMP280_API::BMP280_API() {}
