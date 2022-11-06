#include <Logger.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_LSM9DS1.h>
#include "lsm9ds1_api.h"

#define IMU_REFRESH_RATE 100 // Hz

extern SPIN::Log::CFormattedLogger<1024> logger;

Adafruit_LSM9DS1 LSM9DS1_API::sensor;

LSM9DS1_API& LSM9DS1_API::getInstance() {
  static LSM9DS1_API instance;
  return instance;
}

void LSM9DS1_API::readSensorData() {
  // TODO: fix units
  if (millis() - last_read < 1000/IMU_REFRESH_RATE || broken_connection) {
    return;
  }

  Wire.begin();
  Wire.beginTransmission(LSM9DS1_ADDRESS_ACCELGYRO);
  if (Wire.endTransmission() != 0) {
    broken_connection = true;
    return;
  }

  sensor.read();
  sensor.getEvent(&acc, &mag, &gyro, &temp);

  last_read = millis();
}

float LSM9DS1_API::pitch() {
  return 0;
}

float LSM9DS1_API::yaw() {
  return 0;
}

float LSM9DS1_API::roll() {
  return 0;
}

int16_t LSM9DS1_API::accelerationX() {
  readSensorData();
  return acc.acceleration.y * 100;
}

int16_t LSM9DS1_API::accelerationY() {
  readSensorData();
  return acc.acceleration.x * 100;
}

int16_t LSM9DS1_API::accelerationZ() {
  readSensorData();
  return -acc.acceleration.z * 100;
}

int16_t LSM9DS1_API::gyroX() {
  readSensorData();
  return -gyro.gyro.y * (180 / M_PI);
}

int16_t LSM9DS1_API::gyroY() {
  readSensorData();
  return -gyro.gyro.x * (180 / M_PI);
}

int16_t LSM9DS1_API::gyroZ() {
  readSensorData();
  return gyro.gyro.z * (180 / M_PI);
}

float LSM9DS1_API::magX() {
  readSensorData();
  return mag.magnetic.y;
}

float LSM9DS1_API::magY() {
  readSensorData();
  return mag.magnetic.x;
}

float LSM9DS1_API::magZ() {
  readSensorData();
  return mag.magnetic.z;
}

void LSM9DS1_API::calibrate() {
  // noop -- already calibrated
}

void LSM9DS1_API::setup() {
  // Relative heading -- no magnetometer
  if (!sensor.begin()) {
    while(1);
  }

  sensor.setupAccel(sensor.LSM9DS1_ACCELRANGE_16G, sensor.LSM9DS1_ACCELDATARATE_119HZ);
  sensor.setupMag(sensor.LSM9DS1_MAGGAIN_4GAUSS);
  sensor.setupGyro(sensor.LSM9DS1_GYROSCALE_500DPS);
}

LSM9DS1_API::LSM9DS1_API() {}
