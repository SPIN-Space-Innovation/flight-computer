#include <Logger.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include "bno055_api.h"

// #define BNO055_SAMPLERATE_DELAY_MS (100)

#define READ_INTERVAL (10)

extern SPIN::Log::ILogger* logger;

Adafruit_BNO055 BNO055_API::sensor(55, 0x28);

BNO055_API& BNO055_API::getInstance() {
  static BNO055_API instance;
  return instance;
}

void BNO055_API::readSensorData() {
  logger->Verbose("BNO055: Update: Init");
  // TODO: fix units
  if (millis() - last_read < READ_INTERVAL) {
    logger->Verbose("BNO055: Update: Failed: Not enough time between updates");
    return;
  }

  euler = sensor.getVector(Adafruit_BNO055::VECTOR_EULER);
  acc = sensor.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  gyro = sensor.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
  mag = sensor.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);

  last_read = millis();
  logger->Verbose("BNO055: Update: Finished");
}

float BNO055_API::pitch() {
  readSensorData();
  return euler.y();
}

float BNO055_API::yaw() {
  readSensorData();
  return euler.x();
}

float BNO055_API::roll() {
  readSensorData();
  return euler.z();
}

int16_t BNO055_API::accelerationX() {
  readSensorData();
  return acc.x() * 100;
}

int16_t BNO055_API::accelerationY() {
  readSensorData();
  return acc.y() * 100;
}

int16_t BNO055_API::accelerationZ() {
  readSensorData();
  return acc.z() * 100;
}

int16_t BNO055_API::gyroX() {
  readSensorData();
  return gyro.x() * (180 / M_PI);
}

int16_t BNO055_API::gyroY() {
  readSensorData();
  return gyro.y() * (180 / M_PI);
}

int16_t BNO055_API::gyroZ() {
  readSensorData();
  return gyro.z() * (180 / M_PI);
}

float BNO055_API::magX() {
  readSensorData();
  return mag.x();
}

float BNO055_API::magY() {
  readSensorData();
  return mag.y();
}

float BNO055_API::magZ() {
  readSensorData();
  return mag.z();
}

void BNO055_API::calibrate() {
  uint8_t system, gyro, accel, mag;
  system = gyro = accel = mag = 0;

  while (sensor.isFullyCalibrated()) {}
}

void BNO055_API::setup() {
  logger->Verbose("BNO055: Setup: Init");
  // Relative heading -- no magnetometer
  if(!sensor.begin(Adafruit_BNO055::OPERATION_MODE_ACCGYRO)) {
    while(1);
  }

  // 2G = 0, 4G = 1, 8G = 2, 16G = 3
  sensor.setAccelRange(3);

  // TODO: do we need this?
  sensor.setExtCrystalUse(true);

  logger->Verbose("BNO055: Setup: Finished");
}

BNO055_API::BNO055_API() {}
