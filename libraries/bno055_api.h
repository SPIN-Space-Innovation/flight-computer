#include <Adafruit_BNO055.h>
#include "imu.h"

class BNO055_API: public IMU {
  static Adafruit_BNO055 sensor;

  public:
    /**
     * @brief <h1>Get the Instance singleton BNO055</h1>
     * 
     * @return BNO055_API& BNO055 instance.
     * 
     * @author themicp
     */
    static BNO055_API& getInstance();

    /**
     * @brief <h1>Returns the pitch in degrees.</h1>
     * Returns the absolute pitch of the BNO055 in degrees.
     * 
     * @examplecode
     * @code{.cpp}
     * BNO055_API& bno = BNO055_API::getInstance();
     * Serial.println(bno.pitch());
     * @endcode
     * 
     * @return float The pitch in degrees.
     * 
     * @author themicp
     */
    float pitch();

    /**
     * @brief <h1>Returns the yaw in degrees.</h1>
     * Returns the absolute yaw of the BNO055 in degrees.
     * 
     * @examplecode
     * @code{.cpp}
     * BNO055_API& bno = BNO055_API::getInstance();
     * Serial.println(bno.yaw());
     * @endcode
     * 
     * @return float The yaw in degrees.
     * 
     * @author themicp
     */
    float yaw();

    /**
     * @brief <h1>Returns the roll in degrees.</h1>
     * Returns the absolute roll of the BNO055 in degrees.
     * 
     * @examplecode
     * @code{.cpp}
     * BNO055_API& bno = BNO055_API::getInstance();
     * Serial.println(bno.roll());
     * @endcode
     * 
     * @return float The roll in degrees.
     * 
     * @author themicp
     */
    float roll();

    /**
     * @brief <h1>Returns the acceleration in the x axis in cm/s^2.</h1>
     * Returns the acceleration recorded by the BNO055 in centimeters/second^2.
     * 
     * @examplecode
     * @code{.cpp}
     * BNO055_API& bno = BNO055_API::getInstance();
     * Serial.println(bno.accelerationX());
     * @endcode
     * 
     * @return int16_t The acceleration in the x axis in cm/s^2.
     * 
     * @author themicp
     */
    int16_t accelerationX();

    /**
     * @brief <h1>Returns the acceleration in the y axis in cm/s^2.</h1>
     * Returns the acceleration recorded by the BNO055 in centimeters/second^2.
     * 
     * @examplecode
     * @code{.cpp}
     * BNO055_API& bno = BNO055_API::getInstance();
     * Serial.println(bno.accelerationY());
     * @endcode
     * 
     * @return int16_t The acceleration in the x axis in cm/s^2.
     * 
     * @author themicp
     */
    int16_t accelerationY();

    /**
     * @brief <h1>Returns the acceleration in the z axis in cm/s^2.</h1>
     * Returns the acceleration recorded by the BNO055 in centimeters/second^2.
     * 
     * @examplecode
     * @code{.cpp}
     * BNO055_API& bno = BNO055_API::getInstance();
     * Serial.println(bno.accelerationZ());
     * @endcode
     * 
     * @return int16_t The acceleration in the x axis in cm/s^2.
     * 
     * @author themicp
     */
    int16_t accelerationZ();

    /**
     * @brief <h1>Returns the rotation speed in the x axis in degrees/s.</h1>
     * Returns the rotation speed recorded by the BNO055 in degrees/second.
     * 
     * @examplecode
     * @code{.cpp}
     * BNO055_API& bno = BNO055_API::getInstance();
     * Serial.println(bno.gyroX());
     * @endcode
     * 
     * @todo Verify values
     * 
     * @return int16_t The rotation speed in the x axis in degrees/s.
     * 
     * @author themicp
     */
    int16_t gyroX();

    /**
     * @brief <h1>Returns the rotation speed in the y axis in degrees/s.</h1>
     * Returns the rotation speed recorded by the BNO055 in degrees/second.
     * 
     * @examplecode
     * @code{.cpp}
     * BNO055_API& bno = BNO055_API::getInstance();
     * Serial.println(bno.gyroY());
     * @endcode
     * 
     * @todo Verify values
     * 
     * @return int16_t The rotation speed in the y axis in degrees/s.
     * 
     * @author themicp
     */
    int16_t gyroY();

    /**
     * @brief <h1>Returns the rotation speed in the z axis in degrees/s.</h1>
     * Returns the rotation speed recorded by the BNO055 in degrees/second.
     * 
     * @examplecode
     * @code{.cpp}
     * BNO055_API& bno = BNO055_API::getInstance();
     * Serial.println(bno.gyroZ());
     * @endcode
     * 
     * @todo Verify values
     * 
     * @return int16_t The rotation speed in the z axis in degrees/s.
     * 
     * @author themicp
     */
    int16_t gyroZ();

    /**
     * @brief <h1>Returns the magnetic field in the x axis in uT.</h1>
     * 
     * 
     * @examplecode
     * @code{.cpp}
     * BNO055_API& bno = BNO055_API::getInstance();
     * Serial.println(bno.magX());
     * @endcode
     * 
     * @return float The magnetic field in the x axis in uT.
     * 
     * @author themicp
     */
    float magX();
    /**
     * @brief <h1>Returns the magnetic field in the y axis in uT.</h1>
     * 
     * 
     * @examplecode
     * @code{.cpp}
     * BNO055_API& bno = BNO055_API::getInstance();
     * Serial.println(bno.magX());
     * @endcode
     * 
     * @return float The magnetic field in the y axis in uT.
     * 
     * @author themicp
     */
    float magY();
    
    /**
     * @brief <h1>Returns the magnetic field in the z axis in uT.</h1>
     * 
     * 
     * @examplecode
     * @code{.cpp}
     * BNO055_API& bno = BNO055_API::getInstance();
     * Serial.println(bno.magX());
     * @endcode
     * 
     * @return float The magnetic field in the z axis in uT.
     * 
     * @author themicp
     */
    float magZ();

    /**
     * @brief <h1>Setup the BNO055.</h1>
     * Used to setup the communication protocol, the bus and the sensor itself.
     * 
     * @examplecode
     * @code{.cpp}
     * BNO055_API& bno = BNO055_API::getInstance();
     * bno.setup();
     * @endcode
     *
     * @author themicp
     */
    void setup();

    /**
     * @brief <h1>Calibrate the BNO055.</h1>
     * Used to calibrate the BNO055 to the desired settings.
     * 
     * @examplecode
     * @code{.cpp}
     * BNO055_API& bno = BNO055_API::getInstance();
     * bno.calibrate();
     * @endcode
     *
     * @author themicp
     */
    void calibrate();

  private:
    BNO055_API();
    unsigned long last_read = 0;
    void readSensorData();
    imu::Vector<3> euler, acc, gyro, mag;
};
