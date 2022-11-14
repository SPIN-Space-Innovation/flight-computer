#include <SPIN-Logger.hpp>

#include "mosfet_igniter.h"
#include "Arduino.h"
#include "definitions.h"

struct IgniterOptions {
  int mosfet_pin;
  int led_pin;
};

extern SPIN::Log::CFormattedLogger<1024> logger;

IgniterOptions igniters[2] = {
  {5, 16}, {6, 15}
};

MosfetIgniter& MosfetIgniter::getInstance() {
  static MosfetIgniter instance;
  return instance;
}

void MosfetIgniter::setup() {
  pinMode(igniters[IGNITER - 1].mosfet_pin, OUTPUT);
#if IGN_DEBUG
  pinMode(igniters[IGNITER - 1].led_pin, OUTPUT);
#endif
  digitalWrite(igniters[IGNITER - 1].mosfet_pin, LOW);
}

void MosfetIgniter::enable() {
  digitalWrite(igniters[IGNITER - 1].mosfet_pin, HIGH);
#if IGN_DEBUG
  digitalWrite(igniters[IGNITER - 1].led_pin, HIGH);
#endif
}

void MosfetIgniter::disable() {
  digitalWrite(igniters[IGNITER - 1].mosfet_pin, LOW);
#if IGN_DEBUG
  digitalWrite(igniters[IGNITER - 1].led_pin, LOW);
#endif
}
