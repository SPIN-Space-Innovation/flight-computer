#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>
#include "imu.h"

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

    float magX();
    float magY();
    float magZ();

    void setup();
    void calibrate();

  private:
    LSM9DS1_API();
    unsigned long last_read = 0;
    void readSensorData();
    sensors_event_t acc, mag, gyro, temp;
};
