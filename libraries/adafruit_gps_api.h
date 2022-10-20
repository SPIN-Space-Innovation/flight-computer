#include <Adafruit_GPS.h>
#include "gps_receiver.h"

class Adafruit_GPS_API: public GPSReceiver {
  public:
    static Adafruit_GPS_API& getInstance();

    /**
     * @brief <h1>Setup Adafruit GPS Receiver.</h1>
     * Initialize the setup process of the GPS module. <br />
     * The setup function does not mean that the GPS module will function normally or that it will have access to at least 3 satelites.
     * 
     * @see GPSReceiver
     * @examplecode
     * @code{.cpp}
     * Adafruit_GPS_API receiver = Adafruit_GPS_API::getInstance();
     * receiver.setup();
     * @endcode 
     * 
     * @author themicp
     */
    void setup();

    /**
     * @brief <h1>Checks if GPS Module has access to 3 satelites.</h1>
     * Each GPS modules needs direct access to 3 satelits to be able to determine its latitude and longitude. <br />
     * This function returns whether the module is able to access atleast 3 satelites.
     * 
     * @return true When the GPS module has successfully "paired" to 3 satelites.
     * @return false When the GPS module has not successfully "paired" to 3 satelites.
     * 
     * @author themicp
     */
    bool fix();

    /**
     * @brief <h1>Returns latitude.</h1>
     * This function is used to return the last recorder latitude of the gps
     * module. The value returned is in degrees and minutes in the following
     * format: Latitude: DDMM.MMMM (The first two characters are the degrees.)
     * 
     * @return int32_t The latitude in degress and minutes.
     * 
     * @author themicp
     */
    int32_t latitude();

    /**
     * @brief <h1>Returns longitude.</h1>
     * This function is used to return the last recorder longitude of the gps
     * module. The value returned is in degrees and minutes in the following
     * format: Longitude: DDDMM.MMMM (The first three characters are the
     * degrees.)
     * 
     * @return int32_t The longitude in degress and minutes.
     * 
     * @author themicp
     */
    int32_t longitude();

    /**
     * @brief <h1>Returns Altitude.</h1>
     * This function is used to return the last recorder altitude of the gps
     * module. The value returned is in meters.
     * 
     * @return float The altitude in meters.
     * 
     * @author themicp
     */
    float altitude();
    
    /**
     * @brief <h1>Returns the number of available satelites.</h1>
     * 
     * @return int The number of satelites available to the GPS Module.
     * 
     * @author themicp
     */
    int satellites();

  private:
    /**
     * @brief <h1>Construct a new Adafruit_GPS_API object.</h1>
     * Private constructor used to create static object for singleton design pattern.
     * 
     * @author themicp
     */
    Adafruit_GPS_API();

    static Adafruit_GPS receiver;
    
    /**
     * @brief <h1>Last read data time.</h1>
     * 
     * Ensure data is read every 100ms.
     */
    unsigned long last_read;

    /**
     * @brief <h1>Reads the latest data from the serial.</h1>
     * Clear the GPS buffer and read the last data available from the GPS module.
     * 
     * @author themicp
     */
    void readData();
};
