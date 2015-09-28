#ifndef __SWITCHES_H__

#include <Arduino.h>

#define SWITCH_LEVEL  0
#define SWITCH_NLEVEL 1
#define SWITCH_HtoL   2
#define SWITCH_LtoH   3

class Switches {
public:
  Switches() { }
  Switches(int Pin, bool pullup = true, int mode = SWITCH_HtoL);
  void init(int Pin, bool pullup = true, int mode = SWITCH_HtoL);
  bool operator()();

  void setInterval(int t) { interval = t; }
  void setPin(int Pin, int mode = SWITCH_LEVEL);
  bool read();

private:
  int interval;
  int mode_;
  int pin;
  unsigned long prev;
  bool val;
  void readPin();
};

#endif
