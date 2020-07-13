#pragma once //(or use a guard symbol)

class IMU {
  public:
    virtual float pitch(); // degrees
    virtual float yaw(); // degrees
    virtual float roll(); // degrees

    virtual float accelerationX(); // m/s^2
    virtual float accelerationY(); // m/s^2
    virtual float accelerationZ(); // m/s^2

    virtual float gyroX();
    virtual float gyroY();
    virtual float gyroZ();

    virtual void setup();
    virtual void calibrate();
};
