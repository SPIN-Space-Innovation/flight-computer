#include "IMU.h"
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>

class LSM9DS1_API: public IMU {
  static Adafruit_LSM9DS1 sensor;

  public:
    static LSM9DS1_API& getInstance();
    float pitch();
    float yaw();
    float roll();

    float accelerationX();
    float accelerationY();
    float accelerationZ();

    float gyroX();
    float gyroY();
    float gyroZ();

    void readSensorData();

  private:
    LSM9DS1_API();
};
