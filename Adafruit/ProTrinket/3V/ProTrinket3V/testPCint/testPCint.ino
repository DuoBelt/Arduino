#include <avr/sleep.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define PIN_LED (13)
#define PIN_TSW (4) // for tactile switch (active HIGH)

volatile int lastValue = LOW;

void setup() {
  Serial.begin(115200);
  Serial.println("Start Pin - Change - Interrupts test!");

  pinMode(PIN_LED , OUTPUT);
  pinMode(PIN_TSW , INPUT);

  digitalWrite(PIN_LED, LOW);

  PCMSK2 |= (1 << PCINT20); // on D4
  PCICR  |= (1 << PCIE2); // enable interrupt from group PCIE2

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}

void loop() {
  if (lastValue) {
    Serial.println("Good morning, Mr.PCI!");
    digitalWrite(PIN_LED, HIGH);
    delay(100);
    digitalWrite(PIN_LED, LOW);
    lastValue = 0;
  }
  else {
    Serial.println("zzz ...");
    delay(10);
    sleep_mode(); // Good night baby .......
  }
}

ISR(PCINT2_vect) {
  lastValue = digitalRead(PIN_TSW);
}

