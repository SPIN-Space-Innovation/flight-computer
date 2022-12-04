#include "types.h"

class Telemetry {
  public:
    static Telemetry& getInstance();
    void send(TelemetryMessage message);
    void send(String data);
    void setup();
    void setRadioThrottle(uint16_t radio_throttle_ms);
    bool messageAvailable();
    String receiveMessage();

  private:
    Telemetry();
};
