#include <Logger.h>

#include "adafruit_gps_api.h"

#define GPSSerial Serial1

extern SPIN::Log::CFormattedLogger<1024> logger logger;

Adafruit_GPS_API::Adafruit_GPS_API() {};

Adafruit_GPS Adafruit_GPS_API::receiver(&GPSSerial);

Adafruit_GPS_API& Adafruit_GPS_API::getInstance() {
  static Adafruit_GPS_API instance;
  return instance;
}

void Adafruit_GPS_API::setup() {
  logger->Verbose("GPS: Setup: Init");
  last_read = 0;

  receiver.begin(9600);

  // turn on GPRMC and GGA.
  receiver.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  
  // Update every 1Hz.
  receiver.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  
  // Possition fix update every 1Hz.
  receiver.sendCommand(PMTK_API_SET_FIX_CTL_1HZ);
  logger->Verbose("GPS: Setup: Finished");
}

void Adafruit_GPS_API::readData() {
  logger->Verbose("GPS: Update: Init");
  // Wait for 100ms between meassurments.
  if (millis() - last_read < 100) {
    logger->Verbose("GPS: Update: Failed: Not enough time between updates");
    return;
  }

  // Empty buffer.
  while(receiver.read());

  last_read = millis();

  // Parse data.
  if (receiver.newNMEAreceived()) {
    receiver.parse(receiver.lastNMEA());
  }

  logger->Verbose("GPS: Update: Finished");
}

bool Adafruit_GPS_API::fix() {
  // Check if last data is updated.
  readData();

  return receiver.fix;
}

int32_t Adafruit_GPS_API::latitude() {
  // Check if last data is updated.
  readData();
  
  // Return 0 if no fix.
  if (!fix())
  {
    return 0;
  }

  return receiver.latitude_fixed;
}

int32_t Adafruit_GPS_API::longitude() {
  // Check if last data is updated.
  readData();
  
  // Return 0 if no fix.
  if (!fix())
  {
    return 0;
  }

  return receiver.longitude_fixed;
}

float Adafruit_GPS_API::altitude() {
  // Check if last data is updated.
  readData();
  
  // Return 0 if no fix.
  if (!fix())
  {
    return 0;
  }

  return receiver.altitude;
}

int Adafruit_GPS_API::satellites() {
  // Check if last data is updated.
  readData();

  return receiver.satellites;
}
