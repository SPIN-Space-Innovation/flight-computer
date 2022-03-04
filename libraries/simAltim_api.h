#include "altimeter.h"


class SIM_ALTIM_API: public Altimeter {
  // static Adafruit_BMP3XX sensor; //DMY: none for the moment, but this could be a values set reader instance

  public:
    static SIM_ALTIM_API& getInstance();

    int32_t altitude_cm();
    float pressure();
    int32_t aglCM();

    int32_t getGroundLevelCM();

    void setup();
    void calibrate();

  private:
    SIM_ALTIM_API();

    unsigned long last_sensor_read = 0;
    int32_t cached_altitude_cm = 0;
    int32_t ground_level = 0;
    bool is_broken_connection = false;

    void setGroundLevel();
    void updateDataFromSensor();
};
