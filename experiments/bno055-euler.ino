#include "Wire.h"
#include "BNO055_API.h"

BNO055_API imuSensor = BNO055_API::getInstance();

void setup()
{
  Serial.begin(9600);

  Wire.begin();

  imuSensor.setup();
}

void loop()
{
  Serial.print(imuSensor.pitch());
  Serial.print(",");
  Serial.print(imuSensor.yaw());
  Serial.print(",");
  Serial.print(imuSensor.roll());
  Serial.println();

  delay(100);
}
