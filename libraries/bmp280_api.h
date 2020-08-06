#include "altimeter.h"
#include "Adafruit_BMP280.h"
#include <stdint.h>

class BMP280_API: public Altimeter {
  static Adafruit_BMP280 sensor;

  public:
    static BMP280_API& getInstance();
    int32_t altitude_cm();
    float pressure();
    int32_t aglCM();

    int32_t getGroundLevelCM();

    void setup();
    void calibrate();

  private:
    BMP280_API();

    int32_t ground_level = 0;
    void setGroundLevel();
};
