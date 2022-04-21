#include "altimeter.h"
#include "Adafruit_BMP3XX.h"


class BMP3XX_API: public Altimeter {
  static Adafruit_BMP3XX sensor;

  public:
    static BMP3XX_API& getInstance();
    int32_t altitude_cm();
    int32_t pressure();
    int16_t temperature();
    int32_t aglCM();

    int32_t getGroundLevelCM();

    void setup();
    void calibrate();

  private:
    BMP3XX_API();

    unsigned long last_sensor_read = 0;
    int32_t cached_altitude_cm;
    int32_t ground_level = 0;
    bool broken_connection;
    void setGroundLevel();

    void readSensorData();
};
