#include "Wire.h"
#include "BMP280_API.h"

BMP280_API pressureSensor = BMP280_API::getInstance();

void setup()
{
  Serial.begin(9600);

  Wire.begin();

  pressureSensor.setup();
}

void loop()
{
  Serial.print(pressureSensor.pressure());
  Serial.print(",");
  Serial.print(pressureSensor.altitude());
  Serial.println();

  delay(100);
}
