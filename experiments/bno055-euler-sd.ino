#include "SPI.h"
#include "SD.h"
#include "Wire.h"
#include "BNO055_API.h"

BNO055_API *imuSensor;
String imuLogsFilename = "imulogs";
File imuLogsFile;

void setup()
{
  Serial.begin(9600);

  if (!SD.begin(4)) {
    Serial.println("SD initialization failed!");
    while(1);
  }
  Serial.println("SD initialization done.");

  if (SD.exists(imuLogsFilename)) {
    Serial.println("Removing previous logfile.");
    SD.remove(imuLogsFilename);
  }

  Wire.begin();

  imuSensor = &(imuSensor->getInstance());
}

int i = 0;
void loop()
{
  String dataString = String(millis()) + "," + String(imuSensor->pitch()) + "," + String(imuSensor->yaw()) + "," + String(imuSensor->roll()) + ",";
  Serial.println(dataString);

  imuLogsFile = SD.open(imuLogsFilename, FILE_WRITE);
  if (imuLogsFile) {
    imuLogsFile.println(dataString);
    imuLogsFile.close();
  } else {
    Serial.println("Error opening logfile.");
  }

  delay(100);
}
