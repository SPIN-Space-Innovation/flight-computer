#include "altimeter.h"
#include "Adafruit_BMP3XX.h"


class BMP3XX_API: public Altimeter {
  static Adafruit_BMP3XX sensor;

  public:
    static BMP3XX_API& getInstance();
    int32_t altitude_cm();
    float pressure();
    int32_t aglCM();

    int32_t getGroundLevelCM();

    void setup();
    void calibrate();

  private:
    BMP3XX_API();

    int32_t ground_level = 0;
    void setGroundLevel();
};
