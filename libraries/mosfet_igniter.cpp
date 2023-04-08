#include "mosfet_igniter.h"
#include "Arduino.h"
#include "definitions.h"

struct IgniterOptions {
  int mosfet_pin;
  int led_pin;
};

IgniterOptions igniters[2] = {
  {5, 16}, {6, 15}
};

void MosfetIgniter::setup() {
  pinMode(igniters[igniterChannel - 1].mosfet_pin, OUTPUT);
#if IGN_DEBUG
  pinMode(igniters[igniterChannel - 1].led_pin, OUTPUT);
#endif
  digitalWrite(igniters[igniterChannel - 1].mosfet_pin, LOW);
}

void MosfetIgniter::enable() {
  digitalWrite(igniters[igniterChannel - 1].mosfet_pin, HIGH);
#if IGN_DEBUG
  digitalWrite(igniters[igniterChannel - 1].led_pin, HIGH);
#endif
}

void MosfetIgniter::disable() {
  digitalWrite(igniters[igniterChannel - 1].mosfet_pin, LOW);
#if IGN_DEBUG
  digitalWrite(igniters[igniterChannel - 1].led_pin, LOW);
#endif
}
