#include <Adafruit_GPS.h>
#include "gps_receiver.h"

class Adafruit_GPS_API: public GPSReceiver {
  public:
    static Adafruit_GPS_API& getInstance();

    void setup();
    bool fix();
    int32_t latitude();
    int32_t longitude();
    float altitude();
    int satellites();

  private:
    Adafruit_GPS_API();
    static Adafruit_GPS receiver;
    unsigned long last_read;

    void readData();
};
