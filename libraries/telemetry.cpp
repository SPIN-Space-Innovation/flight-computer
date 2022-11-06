#include <Logger.h>

#include "definitions.h"
#include "telemetry.h"

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2
#define SD_CS 10
#define SD_OFFLOAD_INTERVAL 1000 // ms

extern SPIN::Log::CFormattedLogger<1024> logger;

// Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
RH_RF95 Telemetry::rf95(8, 3);
RHReliableDatagram Telemetry::rf_manager(Telemetry::rf95, SERVER_ADDRESS);

Telemetry& Telemetry::getInstance() {
  static Telemetry instance;
  return instance;
}

void Telemetry::send(TelemetryMessage message) {
  ++message_count;
  message.count = message_count;
  if (!init) {
    return;
  }

  const uint8_t *stream = (uint8_t*)&message;
  size_t message_size = sizeof(TelemetryMessage);
  if (message.type != MESSAGE_TYPE::DEBUG) {
    message_size -= sizeof(message.debug_message);
  }
  String stringifiedMessage = stringifyTelemetryMessage(message);

  logger->Information("%s", stringifyTelemetryMessage);

  if (millis() - last_radio_message_time > radio_throttle_ms) {
    rf95.send(stream, message_size);
    rf95.waitPacketSent();
    rf95.setModeRx(); // continue listening
    last_radio_message_time = millis();
  }
}

void Telemetry::send(String debug_message) {
  if (!init) {
    return;
  }
  TelemetryMessage message;
  message.type = MESSAGE_TYPE::DEBUG;
  debug_message.toCharArray(message.debug_message, sizeof(message.debug_message));
  send(message);
}

void Telemetry::setup() {
  if (!rf_manager.init()) {
    while(1);
  }

  rf95.setFrequency(432.25);
  rf95.setSignalBandwidth(500000);
  rf95.setSpreadingFactor(8);
  rf95.setTxPower(18);

  message_count = 0;
  init = true;
}

bool Telemetry::messageAvailable() {
  return rf_manager.available();
}

String Telemetry::receiveMessage() {
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  uint8_t from;

  rf_manager.recvfromAck(buf, &len, &from);
  return String((char*)buf);
}

void Telemetry::setRadioThrottle(uint16_t radio_throttle_ms) {
  this->radio_throttle_ms = radio_throttle_ms;
}

Telemetry::Telemetry() {}
