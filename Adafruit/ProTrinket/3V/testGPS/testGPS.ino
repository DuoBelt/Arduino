#include <TinyGPS++.h>

#include <avr/sleep.h>
#include <SoftwareSerial.h>

SoftwareSerial ss(5, 6);
TinyGPSPlus gps;

int lastSec = 0;

void setup() {
  while (true) {
    if (Serial) break;
  }
  Serial.begin(115200);

  ss.begin(9600);

  set_sleep_mode(SLEEP_MODE_IDLE);
}

void loop() {
  bool loop;
  for (loop = true; loop; ) {
    int bytes = ss.available();
    while (bytes--) {
      char data = ss.read();

      if (gps.encode(data)) {
        loop = false;
        displayInfo();
      }
    }

  }
  sleep_mode();
}

void displayInfo()
{
  int thisSec = gps.time.second();
  if (thisSec > lastSec) {
    thisSec = lastSec;
    //
    Serial.print(F("Location: "));
    if (gps.location.isValid())  {
      Serial.print(gps.location.lat(), 6);
      Serial.print(F(","));
      Serial.print(gps.location.lng(), 6);
    }
    else  {
      Serial.print(F("INVALID"));
    }
    //
    Serial.println();
  }
}
