#include <Adafruit_BNO055.h>
#include "imu.h"

class BNO055_API: public IMU {
  static Adafruit_BNO055 sensor;

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
    unsigned long last_read = 0;
    void readSensorData();
    imu::Vector<3> euler, acc, gyro, mag;
};
