#include <Adafruit_Sensor.h>
#include <Adafruit_LSM9DS1.h>
#include "lsm9ds1_api.h"

#define READ_INTERVAL (10)

Adafruit_LSM9DS1 LSM9DS1_API::sensor;

LSM9DS1_API& LSM9DS1_API::getInstance() {
  static LSM9DS1_API instance;
  return instance;
}

void LSM9DS1_API::readSensorData() {
  // TODO: fix units
  if (millis() - last_read < READ_INTERVAL) {
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

float LSM9DS1_API::accelerationX() {
  readSensorData();
  return acc.acceleration.x;
}

float LSM9DS1_API::accelerationY() {
  readSensorData();
  return acc.acceleration.y;
}

float LSM9DS1_API::accelerationZ() {
  readSensorData();
  return acc.acceleration.z;
}

float LSM9DS1_API::gyroX() {
  readSensorData();
  return gyro.gyro.x;
}

float LSM9DS1_API::gyroY() {
  readSensorData();
  return gyro.gyro.y;
}

float LSM9DS1_API::gyroZ() {
  readSensorData();
  return gyro.gyro.z;
}

float LSM9DS1_API::magX() {
  readSensorData();
  return mag.magnetic.x;
}

float LSM9DS1_API::magY() {
  readSensorData();
  return mag.magnetic.y;
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

  sensor.setupAccel(sensor.LSM9DS1_ACCELRANGE_16G);
  sensor.setupMag(sensor.LSM9DS1_MAGGAIN_4GAUSS);
  sensor.setupGyro(sensor.LSM9DS1_GYROSCALE_500DPS);
}

LSM9DS1_API::LSM9DS1_API() {}
