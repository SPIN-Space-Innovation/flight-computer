#include "bmp3xx_api.h"

#define BMP_SCK  (13)
#define BMP_MISO (12) // SD0
#define BMP_MOSI (11) // SD1
#define BMP_CS   (10)
#define SEALEVELPRESSURE_HPA (1013.25)
#define BMP_REFRESH_RATE 50 // Hz

Adafruit_BMP3XX BMP3XX_API::sensor;

BMP3XX_API& BMP3XX_API::getInstance() {
  static BMP3XX_API instance;
  return instance;
}


void BMP3XX_API::readSensorData() {
  if (millis() - last_sensor_read < 1000/BMP_REFRESH_RATE) {
    return;
  }

  cached_altitude_cm = sensor.readAltitude(SEALEVELPRESSURE_HPA) * 100;
  last_sensor_read = millis();
}

int32_t BMP3XX_API::altitude_cm() {
  readSensorData();
  return cached_altitude_cm;
}

float BMP3XX_API::pressure() {
  return sensor.pressure / 100.0;
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
  if (!sensor.begin()) {
    while(1); //Freeze
  }
}

void BMP3XX_API::calibrate() {
  sensor.setTemperatureOversampling(BMP3_OVERSAMPLING_16X);
  sensor.setPressureOversampling(BMP3_OVERSAMPLING_16X);
  sensor.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);

  // TODO: use something better than a constant here
  while (sensor.readAltitude(SEALEVELPRESSURE_HPA) > 1700) {
    delay(100);
  }

  setGroundLevel();
}

BMP3XX_API::BMP3XX_API() {}
