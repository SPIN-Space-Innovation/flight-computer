#include "Wire.h"
#include "MPU9250_API.h"

MPU9250_API *imuSensor;

void setup()
{
  Serial.begin(9600);

  Wire.begin();

  imuSensor = &(imuSensor->getInstance());
}

void loop()
{
  Serial.print(imuSensor->pitch());
  Serial.print(",");
  Serial.print(imuSensor->yaw());
  Serial.print(",");
  Serial.print(imuSensor->roll());
  Serial.println();

  delay(100);
}
