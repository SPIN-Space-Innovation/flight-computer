#include <Adafruit_GPS.h>
#include "adafruit_gps_api.h"

#define GPSSerial Serial1

static Adafruit_GPS_API* instance = nullptr;
static Adafruit_GPS* receiver = nullptr;
static unsigned long last_read = 0;

Adafruit_GPS_API::Adafruit_GPS_API() {};

Adafruit_GPS_API& Adafruit_GPS_API::getInstance() {
  if (instance == nullptr)
  {
    instance = new Adafruit_GPS_API();
    receiver = new Adafruit_GPS(&GPSSerial);
  }
  return *instance;
}

void Adafruit_GPS_API::setup() {
  last_read = 0;

  receiver->begin(9600);

  // turn on GPRMC and GGA.
  receiver->sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  
  // Update every 1Hz.
  receiver->sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  
  // Possition fix update every 1Hz.
  receiver->sendCommand(PMTK_API_SET_FIX_CTL_1HZ);
}

void Adafruit_GPS_API::readData() {
  // Wait for 100ms between meassurments.
  if (millis() - last_read < 100) {
    return;
  }

  // Empty buffer.
  while(receiver->read());

  last_read = millis();

  // Parse data.
  if (receiver->newNMEAreceived()) {
    receiver->parse(receiver->lastNMEA());
  }
}

bool Adafruit_GPS_API::fix() {
  // Check if last data is updated.
  readData();

  return receiver->fix;
}

int32_t Adafruit_GPS_API::latitude() {
  // Check if last data is updated.
  readData();
  
  // Return 0 if no fix.
  if (!fix())
  {
    return 0;
  }

  return receiver->latitude_fixed;
}

int32_t Adafruit_GPS_API::longitude() {
  // Check if last data is updated.
  readData();
  
  // Return 0 if no fix.
  if (!fix())
  {
    return 0;
  }

  return receiver->longitude_fixed;
}

float Adafruit_GPS_API::altitude() {
  // Check if last data is updated.
  readData();
  
  // Return 0 if no fix.
  if (!fix())
  {
    return 0;
  }

  return receiver->altitude;
}

int Adafruit_GPS_API::satellites() {
  // Check if last data is updated.
  readData();

  return receiver->satellites;
}