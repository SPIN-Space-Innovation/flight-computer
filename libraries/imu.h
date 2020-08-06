#pragma once //(or use a guard symbol)

#include <stdint.h>

class IMU {
  public:
    virtual float pitch(); // degrees
    virtual float yaw(); // degrees
    virtual float roll(); // degrees

    virtual int16_t accelerationX(); // cm/s^2
    virtual int16_t accelerationY(); // cm/s^2
    virtual int16_t accelerationZ(); // cm/s^2

    virtual int16_t gyroX(); // degrees/s
    virtual int16_t gyroY(); // degrees/s
    virtual int16_t gyroZ(); // degrees/s

    virtual void setup();
    virtual void calibrate();
};
