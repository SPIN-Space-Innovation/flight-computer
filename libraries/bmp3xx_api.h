#include "altimeter.h"
#include "Adafruit_BMP3XX.h"


class BMP3XX_API: public Altimeter {
  static Adafruit_BMP3XX sensor;

  public:
    static BMP3XX_API& getInstance();
    float altitude();
    float pressure();
    float agl();

    float getGroundLevel();

    void setup();
    void calibrate();

  private:
    BMP3XX_API();

    float ground_level = 0;
    void setGroundLevel();
};
