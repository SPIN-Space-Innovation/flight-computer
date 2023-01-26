#include "altimeter.h"

#define DESKTOP
#ifdef DESKTOP
#include <fstream>
#include <iostream>
#include <string>
#else
#include "Adafruit_BMP3XX.h"
#include "SD.H"
#endif

/*
  MOCK_ALTIMETER mimics BMP3XX

  Difference in implementation between MOCK_ALTIMETER and BMP3XX:
  MOCK_ALTIMETER  reads time,  AGL, Pressure and Temperature from a file which contains data from a previous flight.
  It uses the updateValues function to do this.
  updateValues is basically the sensor.readAltitude(SEALEVELPRESSURE_HPA) * 100, EXCEPT THAT it does not return anything
  however because altitude is needed in the readSensorData method,
  the altitude is decoded from the AGL (altitude = aglCM+ground_level)
*/



class MOCK_ALTIMETER: public Altimeter 
{
  #ifdef DESKTOP
  static std::ifstream data_file;
  static std::string data_filename;
  #else
  static File data_file;
  #endif
  static int32_t agl;
  static int32_t press;
  static int16_t temp;
  static unsigned long time;
  
  public:
   // file methods
    static void openFile();
    static void closeFile();  // not used anywhere currently <- fix this 
    static std::string readLine();
    static MOCK_ALTIMETER& getInstance();
    static void updateValues();
    int32_t altitude_cm();
    int32_t pressure();
    int16_t temperature();
    int32_t aglCM();
    int32_t getGroundLevelCM();
    void setup();
    void calibrate();
    MOCK_ALTIMETER();
  private:
    unsigned long last_sensor_read;
    int32_t cached_altitude_cm;
    int32_t ground_level;
    void setGroundLevel();
    void readSensorData();
};



/*
  The Adafruit Feather M0 RFM96 is based on the ATSAMD21G18 chip which is an ARM Cortex-M0+ microcontroller.
  The Feather M0 RFM96 also has a built-in SD card slot.
  The SAMD21G18 has a speed of 48 MHz, it also has a 32-bit data bus width and 32-bit memory address space which are good numbers to work with an SD card.
  The read speed of the SD card itself is also a important factor.
  Standard SD card typically have a read speed of around 20MB/s to 25MB/s, while high-speed SD cards can have read speeds of up to 90MB/s.
  Additionally, the SD library used can also have an impact on read performance.
  Assuming we are working with a standard SD card with a read speed of around 20MB/s, it would take roughly 0.1s to read a 1MB file, and about 2s to read a 20MB file.
*/