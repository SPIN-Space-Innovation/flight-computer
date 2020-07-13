#include "MPU9250_API.h"
#include "quaternionFilters.h"
#include "MPU9250.h"

#define I2Cclock 400000
#define I2Cport Wire
#define MPU9250_ADDRESS MPU9250_ADDRESS_AD0

static MPU9250_API& MPU9250_API::getInstance() {
  static MPU9250_API instance;
  return instance;
}

MPU9250 MPU9250_API::sensor(MPU9250_ADDRESS, I2Cport, I2Cclock);

void MPU9250_API::readSensorData() {
  sensor.readAccelData(sensor.accelCount);
  sensor.ax = (float)sensor.accelCount[0] * sensor.aRes; // - sensor.accelBias[0];
  sensor.ay = (float)sensor.accelCount[1] * sensor.aRes; // - sensor.accelBias[1];
  sensor.az = (float)sensor.accelCount[2] * sensor.aRes; // - sensor.accelBias[2];

  sensor.readGyroData(sensor.gyroCount);
  sensor.gx = (float)sensor.gyroCount[0] * sensor.gRes;
  sensor.gy = (float)sensor.gyroCount[1] * sensor.gRes;
  sensor.gz = (float)sensor.gyroCount[2] * sensor.gRes;

  sensor.readMagData(sensor.magCount);
  sensor.mx = (float)sensor.magCount[0] * sensor.mRes * sensor.factoryMagCalibration[0] - sensor.magBias[0];
  sensor.my = (float)sensor.magCount[1] * sensor.mRes * sensor.factoryMagCalibration[1] - sensor.magBias[1];
  sensor.mz = (float)sensor.magCount[2] * sensor.mRes * sensor.factoryMagCalibration[2] - sensor.magBias[2];

  sensor.updateTime();

  MahonyQuaternionUpdate(
    sensor.ax, sensor.ay, sensor.az, sensor.gx * DEG_TO_RAD,
    sensor.gy * DEG_TO_RAD, sensor.gz * DEG_TO_RAD, sensor.my,
    sensor.mx, sensor.mz, sensor.deltat);

  sensor.yaw = atan2(2.0f * (*(getQ()+1) * *(getQ()+2) + *getQ() * *(getQ()+3)), *getQ() * *getQ() + *(getQ()+1) * *(getQ()+1) - *(getQ()+2) * *(getQ()+2) - *(getQ()+3) * *(getQ()+3));
  sensor.pitch = -asin(2.0f * (*(getQ()+1) * *(getQ()+3) - *getQ() * *(getQ()+2)));
  sensor.roll = atan2(2.0f * (*getQ() * *(getQ()+1) + *(getQ()+2) * *(getQ()+3)), *getQ() * *getQ() - *(getQ()+1) * *(getQ()+1) - *(getQ()+2) * *(getQ()+2) + *(getQ()+3) * *(getQ()+3));
  sensor.pitch *= RAD_TO_DEG;
  sensor.yaw *= RAD_TO_DEG;
  sensor.yaw -= 5;
  sensor.roll *= RAD_TO_DEG;
}

float MPU9250_API::pitch() {
  readSensorData();
  return sensor.pitch;
}

float MPU9250_API::yaw() {
  readSensorData();
  return sensor.yaw;
}

float MPU9250_API::roll() {
  readSensorData();
  return sensor.roll;
}

float MPU9250_API::accelerationX() {
  readSensorData();
  return sensor.ax;
}

float MPU9250_API::accelerationY() {
  readSensorData();
  return sensor.ay;
}

float MPU9250_API::accelerationZ() {
  readSensorData();
  return sensor.az;
}

float MPU9250_API::gyroX() {
  readSensorData();
  return sensor.gx;
}

float MPU9250_API::gyroY() {
  readSensorData();
  return sensor.gy;
}

float MPU9250_API::gyroZ() {
  readSensorData();
  return sensor.gz;
}

float MPU9250_API::magX() {
  readSensorData();
  return sensor.mx;
}

float MPU9250_API::magY() {
  readSensorData();
  return sensor.my;
}

float MPU9250_API::magZ() {
  readSensorData();
  return sensor.mz;
}

MPU9250_API::MPU9250_API() {
  byte c = sensor.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);

  if (c == 0x71) {
    sensor.MPU9250SelfTest(sensor.selfTest);

    // Calibrate gyro and accelerometers, load biases in bias registers
    sensor.calibrateMPU9250(sensor.gyroBias, sensor.accelBias);
    sensor.initMPU9250();

    byte d = sensor.readByte(AK8963_ADDRESS, WHO_AM_I_AK8963);
    if (d != 0x48) {
      // Communication failed, stop here
      Serial.println(F("Communication failed, abort!"));
      Serial.flush();
      abort();
    }

    // Get magnetometer calibration from AK8963 ROM
    sensor.initAK8963(sensor.factoryMagCalibration);

    // Get sensor resolutions, only need to do this once
    sensor.getAres();
    sensor.getGres();
    sensor.getMres();
  } else {
    Serial.print("Could not connect to MPU9250: 0x");
    Serial.println(c, HEX);

    // Communication failed, stop here
    Serial.println(F("Communication failed, abort!"));
    Serial.flush();
    abort();
  }
}
