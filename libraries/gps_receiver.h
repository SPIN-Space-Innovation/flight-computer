#pragma once //(or use a guard symbol)

class GPSReceiver {
  public:
    /**
     * @brief <h1>Setup specified GPS Receiver.</h1>
     * Initialize the setup process of the GPS module. <br />
     * The setup function does not mean that the GPS module will function normally or that it will have access to at least 3 satelites.
     * 
     * @see Adafruit_GPS_API
     * @examplecode
     * @code{.cpp}
     * GPSReceiver receiver = Adafruit_GPS_API::getInstance();
     * receiver.setup();
     * @endcode 
     * 
     * @author themicp
     */
    virtual void setup();

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
    virtual bool fix();

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
    virtual int32_t latitude();
    
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
    virtual int32_t longitude();
    
    /**
     * @brief <h1>Returns Altitude.</h1>
     * This function is used to return the last recorder altitude of the gps
     * module. The value returned is in meters.
     * 
     * @return float The altitude in meters.
     * 
     * @author themicp
     */
    virtual float altitude();
    
    /**
     * @brief <h1>Returns the number of available satelites.</h1>
     * 
     * @return int The number of satelites available to the GPS Module.
     * 
     * @author themicp
     * */
    virtual int satellites();
};
