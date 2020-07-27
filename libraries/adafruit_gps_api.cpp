#include "adafruit_gps_api.h"

#define GPSSerial Serial1

Adafruit_GPS_API::Adafruit_GPS_API() {};

Adafruit_GPS Adafruit_GPS_API::receiver(&GPSSerial);

Adafruit_GPS_API& Adafruit_GPS_API::getInstance() {
  static Adafruit_GPS_API instance;
  return instance;
}

void Adafruit_GPS_API::setup() {
  last_read = 0;

  receiver.begin(9600);
  receiver.sendCommand(PMTK_SET_NMEA_OUTPUT_GGAONLY);
  receiver.sendCommand(PMTK_SET_NMEA_UPDATE_10HZ);
  receiver.sendCommand(PMTK_API_SET_FIX_CTL_5HZ);
}

void Adafruit_GPS_API::readData() {
  if (millis() - last_read < 100) {
    return;
  }
  receiver.read();
  last_read = millis();
  if (receiver.newNMEAreceived()) {
    receiver.parse(receiver.lastNMEA());
  }
}

bool Adafruit_GPS_API::fix() {
  readData();
  return receiver.fix;
}

String Adafruit_GPS_API::latitude() {
  readData();
  if (!fix()) return "--";
  return String(receiver.latitude) + receiver.lat;
}

String Adafruit_GPS_API::longitude() {
  readData();
  if (!fix()) return "--";
  return String(receiver.longitude) + receiver.lon;
}

float Adafruit_GPS_API::altitude() {
  readData();
  if (!fix()) return 0;
  return receiver.altitude;
}

int Adafruit_GPS_API::satellites() {
  readData();
  return receiver.satellites;
}
