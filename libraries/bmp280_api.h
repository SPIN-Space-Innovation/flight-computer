#include "altimeter.h"
#include "Adafruit_BMP280.h"


class BMP280_API: public Altimeter {
  static Adafruit_BMP280 sensor;

  public:
    static BMP280_API& getInstance();
    float altitude();
    float pressure();
    float agl();

    float getGroundLevel();

    void setup();
    void calibrate();

  private:
    BMP280_API();

    float ground_level = 0;
    void setGroundLevel();
};
