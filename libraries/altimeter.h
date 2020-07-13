#pragma once //(or use a guard symbol)

class Altimeter {
  public:
    virtual float altitude(); // meters
    virtual float pressure(); // pascal
    virtual float agl(); // meters

    virtual float getGroundLevel();

    virtual void setup();
    virtual void calibrate();

  private:
    virtual void setGroundLevel();
};
