#include <avr/sleep.h>
#include <RTC8564.h>

#define PIN_RTC_INT (15)
#define INTERVAL_SECS (5)

RTC8564 rtc;

volatile bool gotInterrupt = false;
volatile unsigned int counter = 0;

void setup() {
  set_sleep_mode(SLEEP_MODE_IDLE);
  while (!Serial) {
    sleep_mode();
  }
  Serial.begin(115200);

  Serial1.begin(115200);

  pinMode(PIN_RTC_INT, INPUT_PULLUP);
  rtc.begin();
  rtc.setTimer( rtc.TIMING_1_SEC, INTERVAL_SECS, true, true );

  PCMSK0 |= (1 << PCINT1); // on D15
  PCICR  |= (1 << PCIE0); // enable PCI
}

ISR(PCINT0_vect) {
  if (digitalRead(PIN_RTC_INT) == LOW) {
    counter++;
    gotInterrupt = true;
  }
}

void loop() {
  char buffer[0x100];
  int length = GPSreadln(buffer);

  checkNMEA(buffer);
//  while (Serial1.available()) {
//    Serial.write(Serial1.read());
//  }
  if (gotInterrupt) {
    RTC_TIME thisTime;
    gotInterrupt = false;
    //    Serial.println(counter);
    //
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
