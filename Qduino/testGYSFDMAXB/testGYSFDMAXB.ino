#include <Qduino.h>

#include <avr/sleep.h>
#include <RTC8564.h>

#define PIN_RTC_INT (15)
#define INTERVAL_SECS (5)

#define PIN_TSW (9)

qduino thisQ;
RTC8564 rtc;

volatile bool intRTC = false;
volatile bool intTSW = false;
volatile unsigned int counter = 0;

void setup() {
  thisQ.setup();

  set_sleep_mode(SLEEP_MODE_IDLE);
  while (!Serial) {
    delay(1);
  }
  Serial.begin(115200);

  Serial1.begin(115200);

  pinMode(PIN_TSW, INPUT_PULLUP);

  pinMode(PIN_RTC_INT, INPUT_PULLUP);
  rtc.begin();
  rtc.setTimer( rtc.TIMING_1_SEC, INTERVAL_SECS, true, true );

//  PCMSK0 |= ((1 << PCINT1) | (1 << PCINT5)); // on D15 + D9
//  PCICR  |= (1 << PCIE0); // enable PCI
}

ISR(PCINT0_vect) {
  if (digitalRead(PIN_RTC_INT) == LOW) {
    intRTC = true;
  }
  if (digitalRead(PIN_TSW) == LOW) {
    intTSW = true;
    counter++;
  }
}

void loop() {
  char buffer[0x100];

  if (GPSreadln(buffer)) {
    Serial.println(buffer);
    thisQ.setRGB("orange");
    checkNMEA(buffer);
    thisQ.ledOff();
  }

  if (intTSW) {
    intTSW = false;
    Serial.println(">>> Counter = " + String(counter));
  }
  if (intRTC) {
    RTC_TIME thisTime;
    intRTC = false;
    //
    //
    if (rtc.now(&thisTime), false)  {
      const char* week[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
      snprintf(buffer, 32, "%04u-%02u-%02u(%s) %02u:%02u:%02u\n",
               thisTime.year,
               thisTime.month,
               thisTime.day,
               week[thisTime.wday],
               thisTime.hour,
               thisTime.min,
               thisTime.sec   );

      Serial.print(buffer);
    }
  }
  else {
    sleep_mode();
  }
}
