#include "mock_altimeter.h"


#ifdef DESKTOP
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#else
#include "Adafruit_BMP3XX.h"
#include "SD.H"
#endif


#define BMP_SCK  (13)
#define BMP_MISO (12) // SD0
#define BMP_MOSI (11) // SD1
#define BMP_CS   (10)
#define SEALEVELPRESSURE_HPA (1013.7)
#define BMP_REFRESH_RATE 50 // Hz
#define MAX_ALTITUDE_STEP 100000 // cm


#ifdef DESKTOP
std::string MOCK_ALTIMETER::data_filename = std::string("data.csv");
std::ifstream MOCK_ALTIMETER::data_file; 
#else 
File data_file;
#endif
int32_t MOCK_ALTIMETER::agl;
int32_t MOCK_ALTIMETER::press;
int16_t MOCK_ALTIMETER::temp;
unsigned long MOCK_ALTIMETER::time;

MOCK_ALTIMETER& MOCK_ALTIMETER::getInstance() {
  static MOCK_ALTIMETER instance;
  return instance;
}


/*
  NOTE to developer:
  sensor.readAltitude(SEALEVELPRESSURE_HPA) reads the current altitude above sea level in meters.
  It takes one argument, SEALEVELPRESSURE_HPA, which is the reference sea-level pressure that is used to calculate the altitude.
  When the function is called, it first reads the current pressure and temperature values from  the sensor using the sensor.pressure and sensor.temperature properties.
  Then it calculates the altitude using the following equation:
    Altitude = 44330.0 * (1.0 - pow(Pressure / SEALEVELPRESSURE_HPA, 0.1903));
  So when the the function is called, pressure, tempetature are `computed ` that time
*/


#ifdef DESKTOP

void MOCK_ALTIMETER::readSensorData() {
    using namespace std::chrono;
    auto now = system_clock::now();
    // if (duration_cast<milliseconds>(now - last_sensor_read).count() < 1000/BMP_REFRESH_RATE || broken_connection) {
    //     return;
    // }
    updateValues();
    int32_t reading = agl + ground_level;
    if (abs(reading - cached_altitude_cm) >= MAX_ALTITUDE_STEP) {
        return;
    }
    cached_altitude_cm = reading;
    // last_sensor_read = now;
}
#else
void MOCK_ALTIMETER::readSensorData() {
  if (millis() - last_sensor_read < 1000/BMP_REFRESH_RATE || broken_connection) {
    return;
  }

  // int32_t reading = sensor.readAltitude(SEALEVELPRESSURE_HPA) * 100; <-- change this
  updateValues();
  int32_t reading = aglCM + ground_level;
  if (abs(reading - cached_altitude_cm) >= MAX_ALTITUDE_STEP) {
    // Ignore reading if difference is extreme. Probably problem with the sensor.
    return;
  }
  updateValues();
  reading = aglCM + ground_level;
  //  cached_altitude_cm = sensor.readAltitude(SEALEVELPRESSURE_HPA) * 100;  <-- change this
  cached_altitude_cm = reading;
  last_sensor_read = millis();
}
#endif


int32_t MOCK_ALTIMETER::altitude_cm() {
  readSensorData();
  return cached_altitude_cm;
}


int32_t MOCK_ALTIMETER::pressure() {
  return press;
}


int16_t MOCK_ALTIMETER::temperature() {
  // not sure if multiplication is needed...
  return temp; // original: temperature * 100
}


int32_t MOCK_ALTIMETER::aglCM() {
  return altitude_cm() - ground_level;
}


void MOCK_ALTIMETER::setGroundLevel() {
  ground_level = altitude_cm();
}


int32_t MOCK_ALTIMETER::getGroundLevelCM() {
  return ground_level;
}


void MOCK_ALTIMETER::setup() {
  #ifdef DESKTOP
  std::cout << "SETUP READY" << std::endl;
  #else
  Serial.println("SETUP READY");
  #endif
}


void MOCK_ALTIMETER::calibrate() {
  #ifdef DESKTOP
  std::cout << "CALIBRATION READY" << std::endl;
  #else
  Serial.println("CALIBRATION READY");
  #endif
}

#ifdef DESKTOP
void MOCK_ALTIMETER::openFile() {
    if (!data_file.is_open()) {
        data_file.open(data_filename);
    }
    std::cout << "opened file" << std::endl;
    // skip first line (columns names)
    readLine();
}
#else
void MOCK_ALTIMETER::openFile() {
    if (!SD.begin(BUILTIN_SDCARD)) {
        Serial.println("SD card not found");
        return;
    }
    data_file = SD.open(data_filename, FILE_READ);
    if (!data_file) {
        // error message if file is not found
        Serial.print("Mock Data not found: ");
        return;
    }
    // skip first line (columns names)
    readLine();
}
#endif

void MOCK_ALTIMETER::closeFile() {
    data_file.close();
}


#ifdef DESKTOP
std::string MOCK_ALTIMETER::readLine() {
    if (!data_file.is_open()){
      openFile();
    }
    std::string line;
    if (MOCK_ALTIMETER::data_file.good()) {
        std::getline(data_file, line);
    } else {
        std::cout << "not good" << std::endl;
        return std::string("");
    }
    return line;
}
#else
String MOCK_ALTIMETER::readLine() {
    if (!data_file) {
        openFile();
    }
    String line;
    if (data_file.available() && current_line < data_file.size()) {
        line = data_file.readStringUntil('\n');
        current_line++;
    } else {
        return "";
    }
    return line;
}
#endif


// Reads the next 
// after the line is read it splits the line into different values, and updates the class attributes with the parsed values.
#ifdef DESKTOP
void MOCK_ALTIMETER::updateValues() { // desktop version
    std::string line = readLine();
    // use std::stringstream to parse the line
    std::stringstream line_stream(line);
    std::string token;
    // use std::getline to extract each token from the line
    std::getline(line_stream, token, ',');
    time = std::stoi(token);
    std::getline(line_stream, token, ',');
    agl = std::stoi(token);
    std::getline(line_stream, token, ',');
    press = std::stoi(token);
    std::getline(line_stream, token);
    temp = std::stoi(token);
}
#else
void MOCK_ALTIMETER::updateValues() {
  
  String line = readLine();
  int start_index = 0;
  int end_index = line.indexOf(',');
  time = line.substring(start_index, end_index).toInt();

  start_index = end_index + 1;
  end_index = line.indexOf(',', start_index);
  AGL = line.substring(start_index, end_index).toInt();

  start_index = end_index + 1;
  end_index = line.indexOf(',', start_index);
  pressure = line.substring(start_index, end_index).toInt();

  start_index = end_index + 1;
  temperature = line.substring(start_index).toInt();
}
#endif

MOCK_ALTIMETER::MOCK_ALTIMETER() {}