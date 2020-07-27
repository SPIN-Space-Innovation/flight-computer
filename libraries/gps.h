#pragma once //(or use a guard symbol)

class GPS {
  public:
    virtual void setup();
    virtual bool fix();
    virtual String latitude();
    virtual String longitude();
    virtual float altitude();
    virtual int satellites();
};
