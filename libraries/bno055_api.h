#include "imu.h"

class BNO055_API: public IMU {
  public:
    static BNO055_API& getInstance();
    float pitch();
    float yaw();
    float roll();

    int16_t accelerationX();
    int16_t accelerationY();
    int16_t accelerationZ();

    // TODO: verify values
    int16_t gyroX();
    int16_t gyroY();
    int16_t gyroZ();

    float magX();
    float magY();
    float magZ();

    void setup();
    void calibrate();

  private:
    BNO055_API();
    void readSensorData();
};
