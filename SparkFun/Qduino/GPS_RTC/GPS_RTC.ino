#include <avr/sleep.h>

#include "Arduino.h"
#include "Wire.h"
#include "Qduino.h"
#include <RTC8564.h>

#define PIN_LED (13)
#define PIN_RTC_INT (15)
#define INTERVAL_SECS (1)

qduino Qres; // Qduino resources
fuelGauge battery;  // initialize the library
RTC8564 rtc;

volatile bool gotInterrupt = false;
volatile unsigned int counter = 0;

ISR(PCINT0_vect) {
  if (digitalRead(PIN_RTC_INT) == LOW) {
    digitalWrite(PIN_LED, counter % 2);
    counter++;
    gotInterrupt = true;
  }
}

void setup() {
  Wire.begin();
  Qres.setup();
  battery.setup();  // setup fuel gauge

  Qres.setRGB("pink");

  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_RTC_INT, INPUT_PULLUP);
  rtc.begin();
  rtc.setTimer( rtc.TIMING_1_SEC, INTERVAL_SECS, true, true );
  PCMSK0 |= (1 << PCINT1); // on D15
  //  PCMSK0 |= (1 << PCINT4); // on D8
  PCICR  |= (1 << PCIE0); // enable PCI

  set_sleep_mode(SLEEP_MODE_IDLE);

  while (!Serial) {
    ;
  }
  Serial.begin(115200);
  Serial1.begin(115200);

  Serial.println("Start");

  GPSwriteln((char *)"PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"); // only RMC
  GPSwriteln((char *)"PMTK220,5000"); // 5 sec interval
  GPSwriteln((char *)"PMTK250,1,0,9600"); // RTMC
  GPSwriteln((char *)"PMTK301,1"); // RTMC
  GPSwriteln((char *)"PMTK330,1"); // Tokyo - JAPAN

  //  GPSwriteln((char *)"PMTK251,115200");
}

void loop() {
  char buffer[0x100];

  if (gotInterrupt) {
    gotInterrupt = false;
    int charge = battery.chargePercentage();  // get %
    battery.reset();  // reset for next data request
    Serial.println("Battery " + String(charge) + "%");
  }
  else {
    int length = GPSreadln(buffer);
    if (length) {
      //    Serial.println(buffer);
      checkNMEA(buffer);
    }
  }
  sleep_mode();
}

int GPSreadln(char *buffer) {
  int length = 0;

  if (Serial1.available()) {
    char checkSum = 0;
    bool loop = true;
    bool isChar = true;
    char sumb[3];
    char *cp = sumb;

    while (loop) {
      int bytes = Serial1.available();
      while (bytes--) {
        char c = Serial1.read();
        //      Serial.print(c);
        switch (c) {
          case '$':
            break;
          case '*':
            isChar = false;
            break;
          case 0x0d:
            break;
          case 0x0a:
            *cp = 0;
            *buffer = 0;
            loop = false;
            break;
          default:
            if (isChar) {
              checkSum ^= c;
              *buffer++ = c;
              length++;
            }
            else {
              *cp++ = c;
            }
            break;
        }
      }
    }

    char gotSum;
    sscanf(sumb, "%x", &gotSum);

    //  Serial.println(gotSum, HEX);
    //  Serial.println(checkSum, HEX);

    if (checkSum != gotSum) {
      length = 0;
    }
  }
  return (length);
}

void GPSwriteln(char *p)
{
  char checkSum;
  char c;

  Serial1.print('$');

  for (checkSum = 0; c = *p++; ) {
    checkSum ^= c;
    Serial1.print(c);
  }

  Serial1.print('*');
  Serial1.println(checkSum, HEX);
}

void checkNMEA(char *line) {
  char *p;
  char c;
  int es = 0;
  char *ppp = line;
  char *ep[0x100];

  for (p = line; c = *p;) {
    if (c == ',') {
      ep[es++] = ppp;
      *p++ = 0;
      ppp = p;
    }
    else {
      p++;
    }
  }
  ep[es++] = ppp;


  if (strcmp(ep[0], "GPRMC") == 0) {
    char *vp = ep[2];
    char *dp = ep[9];
    char *tp = ep[1];
    char *lonP = ep[3];
    char *latP = ep[5];
    char *mp = ep[12];

    float lon = atof(lonP);
    float lat = atof(latP);

    Serial.println(String(vp) + " " + "20" + String(dp).substring(4, 6) + "-" + String(dp).substring(2, 4) + "-" + String(dp).substring(0, 2) + " " + String(tp).substring(0, 2) + ":" + String(tp).substring(2, 4) + ":" + String(tp).substring(4, 6));
    Serial.println(String(mp) + " lon:lat = [" + String(lon, 3) + ":" + String(lat, 3) + "]");
  }
  //  Serial.println();
}



