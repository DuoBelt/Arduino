#include <Arduino.h>

#include "Switches.h"

Switches::Switches(int p, bool pullup, int mode)
{
  init(p, pullup, mode);
}

void Switches::init(int p, bool pullup, int mode)
{
  if (pullup) 
    pinMode(p, INPUT_PULLUP);
  interval = 20;
  setPin(p, mode);
}

bool Switches::operator()() {
  bool p = val;

  switch (mode_) {
  case SWITCH_HtoL:
    if (read() == LOW && p == HIGH)
      return true;
    return false;

  case SWITCH_LtoH:
    if (read() == HIGH && p == LOW)
      return true;
    return false;

  case SWITCH_NLEVEL:
    return !read();

  case SWITCH_LEVEL:
      /* FALL THROUGH */
  default:
    return read();
  }
}

bool Switches::read()
{
  if (millis() - prev < interval)
    return val;

  readPin();
  return val;
}

void Switches::readPin()
{
    val = digitalRead(pin);
    prev = millis();
}

void Switches::setPin(int p, int mode)
{
  pin = p;
  mode_ = mode;
  readPin();
}
