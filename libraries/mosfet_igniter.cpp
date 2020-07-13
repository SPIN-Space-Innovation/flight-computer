#include "mosfet_igniter.h"
#include "Arduino.h"

const int mosfet_pin = 5; // IGN1
// const int mosfet_pin = 6; // IGN2

MosfetIgniter& MosfetIgniter::getInstance() {
  static MosfetIgniter instance;
  return instance;
}

void MosfetIgniter::setup() {
  pinMode(mosfet_pin, OUTPUT);
  digitalWrite(mosfet_pin, LOW);
}

void MosfetIgniter::enable() {
  digitalWrite(mosfet_pin, HIGH);
}

void MosfetIgniter::disable() {
  digitalWrite(mosfet_pin, LOW);
}
