#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>  // not used in this demo but required!

// i2c
Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();

void setupSensor()
{
  lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_16G);
  lsm.setupMag(lsm.LSM9DS1_MAGGAIN_4GAUSS);
  lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_500DPS);
}


void setup() {
  Serial.begin(9600);

  while (!Serial) {
    delay(1);
  }

  Serial.println("LSM9DS1 data read demo");

  // Try to initialise and warn if we couldn't detect the chip
  if (!lsm.begin()) {
    Serial.println("Oops ... unable to initialize the LSM9DS1. Check your wiring!");
    while (1);
  }

  Serial.println("Found LSM9DS1 9DOF");

  setupSensor();
}

void loop() {
  lsm.read();  /* ask it to read in the data */

  sensors_event_t a, m, g, temp;

  lsm.getEvent(&a, &m, &g, &temp);

  Serial.print("Accel X: "); Serial.print(a.acceleration.x); Serial.print(" m/s^2");
  Serial.print("\tY: "); Serial.print(a.acceleration.y);     Serial.print(" m/s^2 ");
  Serial.print("\tZ: "); Serial.print(a.acceleration.z);     Serial.println(" m/s^2 ");

  Serial.print("Mag X: "); Serial.print(m.magnetic.x);   Serial.print(" gauss");
  Serial.print("\tY: "); Serial.print(m.magnetic.y);     Serial.print(" gauss");
  Serial.print("\tZ: "); Serial.print(m.magnetic.z);     Serial.println(" gauss");

  Serial.print("Gyro X: "); Serial.print(g.gyro.x);   Serial.print(" dps");
  Serial.print("\tY: "); Serial.print(g.gyro.y);      Serial.print(" dps");
  Serial.print("\tZ: "); Serial.print(g.gyro.z);      Serial.println(" dps");

  Serial.println();
  delay(200);
}
