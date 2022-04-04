#include "mosfet_igniter.h"
#include "Arduino.h"
#include "definitions.h"

const int mosfet_pin = 5; // IGN1
const int continuity_pin = 11; //IGN1 GND 
// const int mosfet_pin = 6; // IGN2
//
const int IGN1_LED = 16;
// const int IGN2_LED = 15;

MosfetIgniter& MosfetIgniter::getInstance() {
  static MosfetIgniter instance;
  return instance;
}

void MosfetIgniter::setup() {
  pinMode(mosfet_pin, OUTPUT);
#if IGN_DEBUG
  pinMode(IGN1_LED, OUTPUT);
#endif
  digitalWrite(mosfet_pin, LOW);
}

void MosfetIgniter::enable() {
  digitalWrite(mosfet_pin, HIGH);
#if IGN_DEBUG
  digitalWrite(IGN1_LED, HIGH);
#endif
}

void MosfetIgniter::disable() {
  digitalWrite(mosfet_pin, LOW);
#if IGN_DEBUG
  digitalWrite(IGN1_LED, LOW);
#endif
}

bool MosfetIgniter::continuity() {
  int state = digitalRead(continuity_pin);
  if(state == HIGH) {
    Serial.print("HIGH");
    return true;
  } else {
    Serial.print("LOW");
    return false;
  }
}