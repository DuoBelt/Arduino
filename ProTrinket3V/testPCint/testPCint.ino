#include <avr/sleep.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define PIN_LED (13)
#define PIN_TSW (4) // for tactile switch (active HIGH)

volatile bool gotInt = false;
volatile int lastValue = 0;

void setup() {
  Serial.begin(115200);

  pinMode(PIN_LED , OUTPUT);
  pinMode(PIN_TSW , INPUT);

  digitalWrite(PIN_LED, LOW);

  PCMSK2 |= (1 << PCINT20); // on D4
  PCICR  |= (1 << PCIE2); // enable interrupt from group PCIE2

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}

void loop() {
  if (gotInt) {
    Serial.println("Good morning, Mr.PCI! " + String(lastValue));
    digitalWrite(PIN_LED, HIGH);
    delay(1000);
    digitalWrite(PIN_LED, LOW);
    gotInt = false;
  }
  else {
    Serial.println("zzz ...");
    delay(10);
    sleep_mode();
  }
}

ISR(PCINT2_vect) {
  lastValue = digitalRead(PIN_TSW);
  if (lastValue) {
    gotInt = true;
  }
}

