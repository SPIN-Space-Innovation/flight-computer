#pragma once //(or use a guard symbol)

#include <stdint.h>

class Altimeter {
  public:
    /**
     * @brief <h1>Get Altitude in centimeters.</h1>
     * Returns the Altitude in centimeters.<br />
     * The altitude can be computed by the pressure or the GPS.<br />
     * 
     * @see BMP280_API
     * @see BMP3XX_API
     * 
     * @examplecode
     * @code{.cpp}
     * Altimeter& alt = BM280_API::getInstance();
     * ...
     * Serial.println(alt.altitude_cm());
     * @endcode
     * 
     * @return int32_t Altitude from reference point.
     * 
     * @author themicp
     */
    virtual int32_t altitude_cm();

    /**
     * @brief <h1>Get Altitude in meters.</h1>
     * Returns the Altitude in meters.<br />
     * The altitude can be computed by the pressure or the GPS.<br />
     * 
     * @see BMP280_API
     * @see BMP3XX_API
     * 
     * @examplecode
     * @code{.cpp}
     * Altimeter& alt = BM280_API::getInstance();
     * ...
     * Serial.println(alt.altitude());
     * @endcode
     * 
     * @return float Altitude from reference point.
     * 
     * @author themicp
     */
    float altitude() {
      return (float)altitude_cm() / 100;
    }


    /**
     * @brief <h1>Get Altitude in centimeters.</h1>
     * Returns the Altitude in centimeters.<br />
     * The altitude can be computed by the pressure or the GPS.<br />
     * The altitude is realtive to ground level.
     * 
     * @see BMP280_API
     * @see BMP3XX_API
     * 
     * @examplecode
     * @code{.cpp}
     * Altimeter& alt = BM280_API::getInstance();
     * ...
     * Serial.println(alt.aglCM());
     * @endcode
     * 
     * @return int32_t Altitude from reference point.
     * 
     * @author themicp
     */
    virtual int32_t aglCM();

    /**
     * @brief <h1>Get Altitude in meters.</h1>
     * Returns the Altitude in meters.<br />
     * The altitude can be computed by the pressure or the GPS.<br />
     * The altitude is realtive to ground level.
     * 
     * @see BMP280_API
     * @see BMP3XX_API
     * 
     * @examplecode
     * @code{.cpp}
     * Altimeter& alt = BM280_API::getInstance();
     * ...
     * Serial.println(alt.agl());
     * @endcode
     * 
     * @return float Altitude from reference point.
     * 
     * @author themicp
     */
    float agl() {
      return (float)aglCM() / 100;
    }

    /**
     * @brief <h1>Get Pressure in Pascal.</h1>
     * Returns the pressure in Pascal.<br />
     * 
     * @see BMP280_API
     * @see BMP3XX_API
     * 
     * @examplecode
     * @code{.cpp}
     * Altimeter& alt = BM280_API::getInstance();
     * ...
     * Serial.println(alt.pressure());
     * @endcode
     * 
     * @return int32_t Pressure from last reading.
     * 
     * @author themicp
     */
    virtual int32_t pressure();

    /**
     * @brief <h1>Get Temperature in Celcius * 100.</h1>
     * Returns the temperature in celcius * 100.<br />
     * 
     * @see BMP280_API
     * @see BMP3XX_API
     * 
     * @examplecode
     * @code{.cpp}
     * Altimeter& alt = BM280_API::getInstance();
     * ...
     * Serial.println(alt.temperature());
     * @endcode
     * 
     * @return int16_t Temperature from last reading.
     * 
     * @author themicp
     */
    virtual int16_t temperature();

    /**
     * @brief <h1>Get the Ground Level height.</h1>
     * 
     * @return int32_t Ground level height.
     * 
     * @author themicp
     */
    virtual int32_t getGroundLevelCM();

    /**
     * @brief <h1>Setup the Altimeter module.</h1>
     * Initialize the setup process of the altimeter module. <br />
     * 
     * @author themicp
     */
    virtual void setup();

    /**
     * @brief <h1>Calibrate altimeter.</h1>
     * Altimeters need to be calibrated from data stored in persistend storage.
     * 
     * @author themicp
     */
    virtual void calibrate();

  private:
    virtual void setGroundLevel();
};
