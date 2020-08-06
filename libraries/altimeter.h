#pragma once //(or use a guard symbol)

#include <stdint.h>

class Altimeter {
  public:
    virtual int32_t altitude_cm();
    float altitude() { // meters
      return (float)altitude_cm() / 100;
    }

    virtual int32_t aglCM();
    float agl() { // meters
      return (float)aglCM() / 100;
    }

    virtual float pressure(); // pascal

    virtual int32_t getGroundLevelCM();

    virtual void setup();
    virtual void calibrate();

  private:
    virtual void setGroundLevel();
};
