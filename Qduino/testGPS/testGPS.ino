#include <SoftwareSerial.h>

#include <avr/sleep.h>
#include <RTC8564.h>

#define PIN_RTC_INT (9)
#define INTERVAL_SECS (1)

RTC8564 rtc;

SoftwareSerial ss(4, 5);

volatile bool gotInterrupt = false;
volatile unsigned int counter = 0;

void setup() {
  set_sleep_mode(SLEEP_MODE_IDLE);
  while (!Serial) {
    sleep_mode();
  }
  Serial.begin(115200);

  ss.begin(115200);

  pinMode(PIN_RTC_INT, INPUT_PULLUP);
  rtc.begin();
  rtc.setTimer( rtc.TIMING_1_SEC, INTERVAL_SECS, true, true );

  PCMSK0 |= (1 << PCINT5); // on D15
  PCICR  |= (1 << PCIE0); // enable PCI
}

ISR(PCINT5_vect) {
  if (digitalRead(PIN_RTC_INT) == LOW) {
    counter++;
    gotInterrupt = true;
  }
}

void loop() {
  if (gotInterrupt) {
    RTC_TIME thisTime;
    gotInterrupt = false;
    //    Serial.println(counter);
    //
    while (ss.available()) {
      Serial.write(ss.read());
    }
    //
    if (rtc.now(&thisTime))  {
      char wboard[0x100];
      const char* week[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
      snprintf(wboard, 32, "%04u-%02u-%02u(%s) %02u:%02u:%02u\n",
               thisTime.year,
               thisTime.month,
               thisTime.day,
               week[thisTime.wday],
               thisTime.hour,
               thisTime.min,
               thisTime.sec   );

      Serial.print(wboard);
    }
  }
  else {
    sleep_mode();
  }
}
