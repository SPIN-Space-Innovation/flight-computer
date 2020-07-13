#include "LSM9DS1_API.h"
#include <MadgwickAHRS.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>

static LSM9DS1_API& LSM9DS1_API::getInstance() {
  static LSM9DS1_API instance;
  return instance;
}

Adafruit_LSM9DS1 LSM9DS1_API::sensor = Adafruit_LSM9DS1();
Madgwick filter;

extern void MadgwickQuaternionUpdate(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz);
extern float q[4];

void LSM9DS1_API::readSensorData() {
  sensors_event_t a, m, g, temp;
  sensor.getEvent(&a, &m, &g, &temp);

  float ax = a.acceleration.x;
  float ay = a.acceleration.y;
  float az = a.acceleration.z;

  float mx = m.magnetic.x;
  float my = m.magnetic.y;
  float mz = m.magnetic.z;

  float gx = g.gyro.x;
  float gy = g.gyro.y;
  float gz = g.gyro.z;

  MadgwickQuaternionUpdate(-ax, ay, az, gx*PI/180.0f, -gy*PI/180.0f, -gz*PI/180.0f,  my,  -mx, mz);

  float a12 =   2.0f * (q[1] * q[2] + q[0] * q[3]);
  float a22 =   q[0] * q[0] + q[1] * q[1] - q[2] * q[2] - q[3] * q[3];
  float a31 =   2.0f * (q[0] * q[1] + q[2] * q[3]);
  float a32 =   2.0f * (q[1] * q[3] - q[0] * q[2]);
  float a33 =   q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3];
  float pitch = -asinf(a32);
  float roll  = atan2f(a31, a33);
  float yaw   = atan2f(a12, a22);
  pitch *= 180.0f / PI;
  yaw   *= 180.0f / PI; 
  yaw   += 13.8f; // Declination at Danville, California is 13 degrees 48 minutes and 47 seconds on 2014-04-04
  if(yaw < 0) yaw   += 360.0f;
  roll  *= 180.0f / PI;

  Serial.print("Yaw, Pitch, Roll: ");
  Serial.print(yaw, 2);
  Serial.print(", ");
  Serial.print(pitch, 2);
  Serial.print(", ");
  Serial.println(roll, 2);
}

float LSM9DS1_API::pitch() {
  return filter.getPitch();
}

float LSM9DS1_API::yaw() {
  return filter.getYaw();
}

float LSM9DS1_API::roll() {
  return filter.getRoll();
}

float LSM9DS1_API::accelerationX() {
  return 0;
}

float LSM9DS1_API::accelerationY() {
  return 0;
}

float LSM9DS1_API::accelerationZ() {
  return 0;
}

float LSM9DS1_API::gyroX() {
  return 0;
}

float LSM9DS1_API::gyroY() {
  return 0;
}

float LSM9DS1_API::gyroZ() {
  return 0;
}

LSM9DS1_API::LSM9DS1_API() {
  if (!sensor.begin()) {
    Serial.print(F("No LSM9DS1 detected ... Check your wiring or I2C ADDR!"));
    while(1);
  }

  filter.begin(25);

  sensor.setupAccel(sensor.LSM9DS1_ACCELRANGE_2G);
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_4G);
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_6G);
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_8G);
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_16G);

  // 2.) Set the magnetometer sensitivity
  sensor.setupMag(sensor.LSM9DS1_MAGGAIN_4GAUSS);
  //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_4GAUSS);
  //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_8GAUSS);
  //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_12GAUSS);

  // 3.) Setup the gyroscope
  sensor.setupGyro(sensor.LSM9DS1_GYROSCALE_245DPS);
  //lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_500DPS);
  //lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_2000DPS);
}
