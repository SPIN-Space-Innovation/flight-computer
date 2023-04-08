#include "igniter.h"

class MosfetIgniter: public Igniter {
  private:
    int igniterChannel;
  public:
    MosfetIgniter(int channel);
    void setup();
    void enable();
    void disable();
};

inline MosfetIgniter::MosfetIgniter(int channel)
  : igniterChannel(channel)
{}