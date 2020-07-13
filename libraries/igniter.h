#pragma once //(or use a guard symbol)

class Igniter {
  public:
    virtual void setup();
    virtual void enable();
    virtual void disable();
};
