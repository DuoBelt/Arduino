#include <avr/sleep.h>

#define PIN_TSW (9)

// ---------------------------------------------------------------------------------------------------------------
class NMEA {
  public:
    NMEA();
  private:
    float __lon;
    float __lat;
};

NMEA::NMEA(){
  __lon = 0.0;
  __lat = 0.0;
}

NMEA nmea;
// ---------------------------------------------------------------------------------------------------------------

void setup() {
  while (true) {
    if (Serial) {
      break;
    }
    else delay(10);
  }
  Serial.begin(115200);

  Serial1.begin(9600);
  pinMode(PIN_TSW, INPUT_PULLUP);

  set_sleep_mode(SLEEP_MODE_IDLE);

  PCMSK0 |= (1 << PCINT5);
  PCICR |= (1 << PCIE0);
}

volatile int counter = 0;
volatile bool tako = false;

ISR(PCINT0_vect) {
  if (digitalRead(PIN_TSW) == LOW) {
    counter++;
    tako = true;
  }
}

void loop() {
  char buffer[0x100];
  int length = GPSreadln(buffer);

  checkNMEA(buffer);
  //  Serial.println(buffer);

  //  int bytes = Serial1.available();
  //  while (bytes--) {
  //    char c = (char)Serial1.read();
  //    Serial.print(c);
  //  }
  if (tako) {
    tako = false;
    Serial.println(counter);
  }
  else {
    //    delay(1000);
    sleep_mode();
  }
}
