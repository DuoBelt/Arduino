#define USE_USB true

#define USE_PWF false
/*
   Notice! 'OUT' must pull down with 10K register
*/

#include <avr/sleep.h>

#if USE_PWF
#include <avr/power.h>
#endif

#define LEDPIN (17)
#define INTPIN (7)

volatile unsigned int counter = 0;
volatile bool catchPIR = false;
volatile bool lastStatus = false;

void cyclePoint() {
  catchPIR = digitalRead(INTPIN);
  counter++;
}

void setup() {
#if USE_USB
  while (true) {
    if (Serial) break;
  }
  Serial.begin(115200);
  for(int a=0; a<5; a++){
    Serial.println("Waiting ...");
    delay(1000);
  }
  Serial.println("Start PaPIRs");
#endif
  pinMode(LEDPIN, OUTPUT);
  pinMode(INTPIN, INPUT);

  set_sleep_mode(SLEEP_MODE_IDLE);
//  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

#if USE_PWF
  power_timer1_disable();
  power_timer2_disable();
  power_timer3_disable();
#endif

  attachInterrupt(digitalPinToInterrupt(INTPIN), cyclePoint, CHANGE);
  //  attachInterrupt(digitalPinToInterrupt(INTPIN), cyclePoint, RISING);
  //  attachInterrupt(digitalPinToInterrupt(INTPIN), cyclePoint, FALLING);
}

unsigned int loopCounter = 0;

void loop() {
  if (catchPIR != lastStatus) {
    digitalWrite(LEDPIN, catchPIR);
    lastStatus = catchPIR;
#if USE_USB
    Serial.println(loopCounter);
#endif
  }
  else {
    sleep_mode();
    loopCounter++;
  }
}
