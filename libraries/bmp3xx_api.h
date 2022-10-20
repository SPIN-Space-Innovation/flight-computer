#include "altimeter.h"
#include "Adafruit_BMP3XX.h"


class BMP3XX_API: public Altimeter {
  static Adafruit_BMP3XX sensor;

  public:
    static BMP3XX_API& getInstance();

    /**
     * @brief <h1>Get Altitude in centimeters.</h1>
     * Returns the Altitude in centimeters.<br />
     * The altitude can be computed by the pressure or the GPS.<br />
     * 
     * @see Altimeter
     * 
     * @examplecode
     * @code{.cpp}
     * BMP3XX_API& alt = BMP3XX_API::getInstance();
     * ...
     * Serial.println(alt.altitude_cm());
     * @endcode
     * 
     * @return int32_t Altitude from reference point.
     * 
     * @author themicp
     */
    int32_t altitude_cm();

    /**
     * @brief <h1>Get Pressure in Pascal.</h1>
     * Returns the pressure in Pascal.<br />
     * 
     * @see Altimeter
     * 
     * @examplecode
     * @code{.cpp}
     * BMP3XX_API& alt = BMP3XX_API::getInstance();
     * ...
     * Serial.println(alt.pressure());
     * @endcode
     * 
     * @return int32_t Pressure from last reading.
     * 
     * @author themicp
     */
    int32_t pressure();

    /**
     * @brief <h1>Get Temperature in Celcius * 100.</h1>
     * Returns the temperature in celcius * 100.<br />
     * 
     * @see Altimeter
     * 
     * @examplecode
     * @code{.cpp}
     * BMP3XX_API& alt = BMP3XX_API::getInstance();
     * ...
     * Serial.println(alt.temperature());
     * @endcode
     * 
     * @return int16_t Temperature from last reading.
     * 
     * @author themicp
     */
    int16_t temperature();

    /**
     * @brief <h1>Get Altitude in centimeters.</h1>
     * Returns the Altitude in centimeters.<br />
     * The altitude can be computed by the pressure or the GPS.<br />
     * The altitude is realtive to ground level.
     * 
     * @see Altimeter
     * 
     * @examplecode
     * @code{.cpp}
     * BMP3XX_API& alt = BMP3XX_API::getInstance();
     * ...
     * Serial.println(alt.aglCM());
     * @endcode
     * 
     * @return int32_t Altitude from reference point.
     * 
     * @author themicp
     */
    int32_t aglCM();

    /**
     * @brief <h1>Get the Ground Level height.</h1>
     * 
     * @return int32_t Ground level height.
     * 
     * @author themicp
     */
    int32_t getGroundLevelCM();

    /**
     * @brief <h1>Setup the Altimeter module.</h1>
     * Initialize the setup process of the altimeter module. <br />
     * 
     * @author themicp
     */
    void setup();
    
    /**
     * @brief <h1>Calibrate altimeter.</h1>
     * Altimeters need to be calibrated from data stored in persistend storage.
     * 
     * @author themicp
     */
    void calibrate();

  private:
    /**
     * @brief <h1>Construct a new BMP3XX_API object.</h1>
     * Private constructor used to create static object for singleton design pattern.
     * 
     * @author themicp
     */
    BMP3XX_API();

    /**
     * @brief <h1>Last read data time.</h1>
     * 
     * Ensure data is read every (1000/BMP_REFRESH_RATE)s.
     */
    unsigned long last_sensor_read = 0;


    int32_t cached_altitude_cm;
    
    /**
     * @brief <h1>Reference ground level.</h1>
     */
    int32_t ground_level = 0;
    
    /**
     * @brief <h1>Checks if communication protocol failed.</h1>
     */
    bool broken_connection;

    void setGroundLevel();

    /**
     * @brief <h1>Reads the latest data from the serial.</h1>
     * Clear the GPS buffer and read the last data available from the GPS module.
     * 
     * @author themicp
     */
    void readSensorData();
};
