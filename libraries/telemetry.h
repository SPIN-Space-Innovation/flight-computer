#include "Arduino.h"
#include "SPI.h"
#include "RH_RF95.h"
#include "RHReliableDatagram.h"
#include "SD.h"
#include "types.h"

class Telemetry {
  public:
    static Telemetry& getInstance();
    void send(TelemetryMessage message);
    void send(String data);
    void setup();
    bool messageAvailable();
    String receiveMessage();

  private:
    bool init = false;
    uint32_t message_count;
    static RH_RF95 rf95;
    static RHReliableDatagram rf_manager;
    String logs_filename = "flight.log"; // max length: 8.3
    File logs_file;
    uint8_t* marshall(TelemetryMessage message);

    Telemetry();
};
