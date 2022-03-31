#pragma once //(or use a guard symbol)

class GPSReceiver {
  public:
    virtual void setup();
    virtual bool fix();
    virtual int32_t latitude();
    virtual int32_t longitude();
    virtual float altitude();
    virtual int satellites();
};
