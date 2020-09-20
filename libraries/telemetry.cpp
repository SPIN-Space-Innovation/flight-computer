#include "telemetry.h"

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2
#define SERIAL_DEBUG false
#define SD_LOGS true
#define SD_CS 10
#define SD_OFFLOAD_INTERVAL 1000 // ms
#define MAX_MESSAGE_SIZE 85
#define TELEMETRY_MESSAGE_SIZE 33

// Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
RH_RF95 Telemetry::rf95(8, 3);
RHReliableDatagram Telemetry::rf_manager(Telemetry::rf95, SERVER_ADDRESS);

Telemetry& Telemetry::getInstance() {
  static Telemetry instance;
  return instance;
}

uint8_t* Telemetry::marshall(TelemetryMessage message) {
  static uint8_t stream[MAX_MESSAGE_SIZE];
  stream[0] = (uint8_t)message.type;
  stream[1] = (message.count & 0xFF000000) >> 24 ;
  stream[2] = (message.count & 0x00FF0000) >> 16;
  stream[3] = (message.count & 0x0000FF00) >> 8;
  stream[4] = message.count & 0x000000FF;

  if (message.type == MESSAGE_TYPE::DEBUG) {
    memcpy(stream + 5, message.debug_message, sizeof(message.debug_message));
  } else if (message.type == MESSAGE_TYPE::TELEMETRY) {
    stream[5] = (message.met & 0xFF000000) >> 24 ;
    stream[6] = (message.met & 0x00FF0000) >> 16;
    stream[7] = (message.met & 0x0000FF00) >> 8;
    stream[8] = message.met & 0x000000FF;

    stream[9] = message.free_memory_kb;

    stream[10] = (message.battery_voltage_mv & 0xFF00) >> 8;
    stream[11] = message.battery_voltage_mv & 0x00FF;

    stream[12] = (uint8_t)message.state;

    stream[13] = (message.backup_deployer_status & 0xFF00) >> 8;
    stream[14] = message.backup_deployer_status & 0x00FF;

    if (message.state != STATE::SETUP and message.state != STATE::IDLE and message.state != STATE::CALIBRATION) {
      stream[15] = (message.payload.agl_cm & 0xFF000000) >> 24 ;
      stream[16] = (message.payload.agl_cm & 0x00FF0000) >> 16;
      stream[17] = (message.payload.agl_cm & 0x0000FF00) >> 8;
      stream[18] = message.payload.agl_cm & 0x000000FF;

      stream[19] = (message.payload.acceleration_x & 0xFF00) >> 8;
      stream[20] = message.payload.acceleration_x & 0x00FF;
      stream[21] = (message.payload.acceleration_y & 0xFF00) >> 8;
      stream[22] = message.payload.acceleration_y & 0x00FF;
      stream[23] = (message.payload.acceleration_z & 0xFF00) >> 8;
      stream[24] = message.payload.acceleration_z & 0x00FF;

      stream[25] = (message.payload.gyroscope_x & 0xFF00) >> 8;
      stream[26] = message.payload.gyroscope_x & 0x00FF;
      stream[27] = (message.payload.gyroscope_y & 0xFF00) >> 8;
      stream[28] = message.payload.gyroscope_y & 0x00FF;
      stream[29] = (message.payload.gyroscope_z & 0xFF00) >> 8;
      stream[30] = message.payload.gyroscope_z & 0x00FF;

      stream[31] = (uint8_t)message.payload.gps_fix;
      stream[32] = message.payload.gps_satellites;
    }
  }

  return stream;
}

void Telemetry::send(TelemetryMessage message) {
  ++message_count;
  message.count = message_count;
  if (!init) {
    return;
  }

  uint8_t *stream = marshall(message);
  String stringifiedMessage = stringifyTelemetryMessage(message);
  int message_size = message.type == MESSAGE_TYPE::DEBUG ? MAX_MESSAGE_SIZE : TELEMETRY_MESSAGE_SIZE;

#if SERIAL_DEBUG
    Serial.print("RADIO: ");
    for (int i = 0; i < message_size; ++i) {
      Serial.print(stream[i], BIN);
    }
    Serial.println();
    Serial.println("STRINGIFIED: " + stringifiedMessage);
#endif

#if SD_LOGS
  logs_file.println(stringifiedMessage);
  if (millis() - last_sd_sync > SD_OFFLOAD_INTERVAL) {
    logs_file.close();
    logs_file = SD.open(logs_filename, FILE_WRITE);
    last_sd_sync = millis();
  }
#endif

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

  rf95.setSignalBandwidth(500000);
  rf95.setSpreadingFactor(8);
  rf95.setTxPower(18);

#if SD_LOGS
  if (!SD.begin(SD_CS)) {
    while(1);
  }

  if (SD.exists(logs_filename)) {
    // TODO: Rotate log files
    // SD.remove(logs_filename);
  }

  logs_file = SD.open(logs_filename, FILE_WRITE);
#endif

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
