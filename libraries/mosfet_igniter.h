#include "igniter.h"

class MosfetIgniter: public Igniter {
  public:
    static MosfetIgniter& getInstance();
    void setup();
    void enable();
    void disable();
    bool continuity();
};
