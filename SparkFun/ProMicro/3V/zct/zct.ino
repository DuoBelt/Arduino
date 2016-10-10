#define USE_USB true

#include <avr/sleep.h>
#include <RTC8564.h>

#define LEDPIN (4)

#define PIN_RTC_INT (7)

#define INTERVAL_SECS (1)

RTC8564 rtc;

volatile unsigned long counter = 0;
unsigned long lastC = 0;

void timerHandler()
{
  counter++;
}

void setup() {
  Serial1.begin(115200); // 3GIM
#if USE_USB

  while (!Serial);

  Serial.begin(115200);
  Serial.println("start RTC test");
#endif
  set_sleep_mode(SLEEP_MODE_IDLE);
  pinMode(PIN_RTC_INT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_RTC_INT), timerHandler, FALLING);

  rtc.begin();
  rtc.setTimer( rtc.TIMING_1_SEC, INTERVAL_SECS, true, true );

  pinMode(LEDPIN, OUTPUT);

  //  Serial1.println("$YV");
  Serial1.println("$WG http://www.klabo.co.jp");
}


void loop() {
  while (Serial1.available()) {
    char c = Serial1.read();
    Serial.print(c);
  }
  sleep_mode();
  digitalWrite(LEDPIN, counter % 2);
  if (counter > lastC) {
    lastC = counter;
#if USE_USB
    Serial.println(counter);
#endif
  }
}
