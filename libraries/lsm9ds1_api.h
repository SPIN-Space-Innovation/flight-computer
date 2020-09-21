#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>
#include "imu.h"
#include <stdint.h>

class LSM9DS1_API: public IMU {
  static Adafruit_LSM9DS1 sensor;

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
    unsigned long last_read = 0;
    void readSensorData();
    sensors_event_t acc, mag, gyro, temp;
    bool broken_connection = false;
};
