#pragma once

#include <stdint.h>

class IMU {
  public:
    /**
     * @brief <h1>Returns the pitch in degrees.</h1>
     * Returns the absolute pitch of the IMU sensor in degrees.
     * 
     * @examplecode
     * @code{.cpp}
     * IMU& imu = BNO055_API::getInstance();
     * Serial.println(imu.pitch());
     * @endcode
     * 
     * @return float The pitch in degrees.
     * 
     * @author themicp
     */
    virtual float pitch();

    /**
     * @brief <h1>Returns the yaw in degrees.</h1>
     * Returns the absolute yaw of the IMU sensor in degrees.
     * 
     * @examplecode
     * @code{.cpp}
     * IMU& imu = BNO055_API::getInstance();
     * Serial.println(imu.yaw());
     * @endcode
     * 
     * @return float The yaw in degrees.
     * 
     * @author themicp
     */
    virtual float yaw();

    /**
     * @brief <h1>Returns the roll in degrees.</h1>
     * Returns the absolute roll of the IMU sensor in degrees.
     * 
     * @examplecode
     * @code{.cpp}
     * IMU& imu = BNO055_API::getInstance();
     * Serial.println(imu.roll());
     * @endcode
     * 
     * @return float The roll in degrees.
     * 
     * @author themicp
     */
    virtual float roll();


    /**
     * @brief <h1>Returns the acceleration in the x axis in cm/s^2.</h1>
     * Returns the acceleration recorded by the IMU sensor in centimeters/second^2.
     * 
     * @examplecode
     * @code{.cpp}
     * IMU& imu = BNO055_API::getInstance();
     * Serial.println(imu.accelerationX());
     * @endcode
     * 
     * @return int16_t The acceleration in the x axis in cm/s^2.
     * 
     * @author themicp
     */
    virtual int16_t accelerationX();

    /**
     * @brief <h1>Returns the acceleration in the y axis in cm/s^2.</h1>
     * Returns the acceleration recorded by the IMU sensor in centimeters/second^2.
     * 
     * @examplecode
     * @code{.cpp}
     * IMU& imu = BNO055_API::getInstance();
     * Serial.println(imu.accelerationY());
     * @endcode
     * 
     * @return int16_t The acceleration in the y axis in cm/s^2.
     * 
     * @author themicp
     */
    virtual int16_t accelerationY();

    /**
     * @brief <h1>Returns the acceleration in the z axis in cm/s^2.</h1>
     * Returns the acceleration recorded by the IMU sensor in centimeters/second^2.
     * 
     * @examplecode
     * @code{.cpp}
     * IMU& imu = BNO055_API::getInstance();
     * Serial.println(imu.accelerationZ());
     * @endcode
     * 
     * @return int16_t The acceleration in the z axis in cm/s^2.
     * 
     * @author themicp
     */
    virtual int16_t accelerationZ();

    /**
     * @brief <h1>Returns the rotation speed in the x axis in degrees/s.</h1>
     * Returns the rotation speed recorded by the IMU sensor in degrees/second.
     * 
     * @examplecode
     * @code{.cpp}
     * IMU& imu = BNO055_API::getInstance();
     * Serial.println(imu.gyroX());
     * @endcode
     * 
     * @return int16_t The rotation speed in the x axis in degrees/s.
     * 
     * @author themicp
     */
    virtual int16_t gyroX();

    /**
     * @brief <h1>Returns the rotation speed in the y axis in degrees/s.</h1>
     * Returns the rotation speed recorded by the IMU sensor in degrees/second.
     * 
     * @examplecode
     * @code{.cpp}
     * IMU& imu = BNO055_API::getInstance();
     * Serial.println(imu.gyroY());
     * @endcode
     * 
     * @return int16_t The rotation speed in the y axis in degrees/s.
     * 
     * @author themicp
     */
    virtual int16_t gyroY();

    /**
     * @brief <h1>Returns the rotation speed in the z axis in degrees/s.</h1>
     * Returns the rotation speed recorded by the IMU sensor in degrees/second.
     * 
     * @examplecode
     * @code{.cpp}
     * IMU& imu = BNO055_API::getInstance();
     * Serial.println(imu.gyroZ());
     * @endcode
     * 
     * @return int16_t The rotation speed in the z axis in degrees/s.
     * 
     * @author themicp
     */
    virtual int16_t gyroZ();

    /**
     * @brief <h1>Setup the IMU sensor.</h1>
     * Used to setup the communication protocol, the bus and the sensor itself.
     * 
     * @examplecode
     * @code{.cpp}
     * IMU& imu = BNO055_API::getInstance();
     * imu.setup();
     * @endcode
     *
     * @author themicp
     */
    virtual void setup();

    /**
     * @brief <h1>Calibrate the IMU sensor.</h1>
     * Used to calibrate the IMU sensor to the desired settings.
     * 
     * @examplecode
     * @code{.cpp}
     * IMU& imu = BNO055_API::getInstance();
     * imu.calibrate();
     * @endcode
     *
     * @author themicp
     */
    virtual void calibrate();
};
