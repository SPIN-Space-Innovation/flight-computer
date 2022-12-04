#include "Adafruit_BMP3XX.h"
#include "bmp3xx_api.h"

#define BMP_SCK  (13)
#define BMP_MISO (12) // SD0
#define BMP_MOSI (11) // SD1
#define BMP_CS   (10)
#define SEALEVELPRESSURE_HPA (1013.7)
#define BMP_REFRESH_RATE 50 // Hz
#define MAX_ALTITUDE_STEP 100000 // cm

static BMP3XX_API* instance = nullptr;
static Adafruit_BMP3XX* sensor = nullptr;
static unsigned long last_sensor_read = 0;
static int32_t cached_altitude_cm;
static int32_t ground_level = 0;
static bool broken_connection;

BMP3XX_API& BMP3XX_API::getInstance() {
  if (instance == nullptr)
  {
    instance = new BMP3XX_API();
    sensor = new Adafruit_BMP3XX();
  }
  return *instance;
}


void BMP3XX_API::readSensorData() {
  if (millis() - last_sensor_read < 1000/BMP_REFRESH_RATE || broken_connection) {
    return;
  }

  Wire.begin();
  Wire.beginTransmission(BMP3XX_DEFAULT_ADDRESS);
  if (Wire.endTransmission() != 0) {
    broken_connection = true;
    return;
  }

  int32_t reading = sensor->readAltitude(SEALEVELPRESSURE_HPA) * 100;
  if (abs(reading - cached_altitude_cm) >= MAX_ALTITUDE_STEP) {
    // Ignore reading if difference is extreme. Probably problem with the sensor.
    return;
  }
  cached_altitude_cm = sensor->readAltitude(SEALEVELPRESSURE_HPA) * 100;
  last_sensor_read = millis();
}

int32_t BMP3XX_API::altitude_cm() {
  readSensorData();
  return cached_altitude_cm;
}

int32_t BMP3XX_API::pressure() {
  return sensor->pressure;
}

int16_t BMP3XX_API::temperature() {
  return sensor->temperature * 100;
}

int32_t BMP3XX_API::aglCM() {
  return altitude_cm() - ground_level;
}

void BMP3XX_API::setGroundLevel() {
  ground_level = altitude_cm();
}

int32_t BMP3XX_API::getGroundLevelCM() {
  return ground_level;
}

void BMP3XX_API::setup() {
  if (!sensor->begin()) {
    while(1); //Freeze
  }
}

void BMP3XX_API::calibrate() {
  sensor->setPressureOversampling(BMP3_OVERSAMPLING_16X);
  sensor->setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_7);
  sensor->setOutputDataRate(BMP3_ODR_100_HZ);

  // TODO: use something better than a constant here
  while (sensor->readAltitude(SEALEVELPRESSURE_HPA) > 1700) {
    delay(100);
  }
  delay(1000); // TODO: refine the value. New BMP needed more time

  setGroundLevel();
}

BMP3XX_API::BMP3XX_API() {}
