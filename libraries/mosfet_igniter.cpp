#include "mosfet_igniter.h"
#include "Arduino.h"
#include "definitions.h"

const int mosfet_pin = 5; // IGN1
const int IGN_LED = 16; // IGN1

/*
const int mosfet_pin = 6; // IGN2
const int IGN_LED = 15; // IGN2
*/

MosfetIgniter& MosfetIgniter::getInstance() {
  static MosfetIgniter instance;
  return instance;
}

void MosfetIgniter::setup() {
  pinMode(mosfet_pin, OUTPUT);
#if IGN_DEBUG
  pinMode(IGN_LED, OUTPUT);
#endif
  digitalWrite(mosfet_pin, LOW);
}

void MosfetIgniter::enable() {
  digitalWrite(mosfet_pin, HIGH);
#if IGN_DEBUG
  digitalWrite(IGN_LED, HIGH);
#endif
}

void MosfetIgniter::disable() {
  digitalWrite(mosfet_pin, LOW);
#if IGN_DEBUG
  digitalWrite(IGN_LED, LOW);
#endif
}
