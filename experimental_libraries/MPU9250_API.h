#include "IMU.h"
#include "MPU9250.h"

class MPU9250_API: public IMU {
  static MPU9250 sensor;

  public:
    static MPU9250_API& getInstance();
    float pitch();
    float yaw();
    float roll();

    float accelerationX();
    float accelerationY();
    float accelerationZ();

    float gyroX();
    float gyroY();
    float gyroZ();

    float magX();
    float magY();
    float magZ();

  private:
    MPU9250_API();
    void readSensorData();
};
