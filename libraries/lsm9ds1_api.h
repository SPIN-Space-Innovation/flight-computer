#include "imu.h"
#include <stdint.h>

class LSM9DS1_API: public IMU {
  public:
    static LSM9DS1_API& getInstance();
    float pitch();
    float yaw();
    float roll();

    int16_t accelerationX();
    int16_t accelerationY();
    int16_t accelerationZ();

    int16_t gyroX();
    int16_t gyroY();
    int16_t gyroZ();

    float magX();
    float magY();
    float magZ();

    void setup();
    void calibrate();

  private:
    LSM9DS1_API();
    void readSensorData();
};