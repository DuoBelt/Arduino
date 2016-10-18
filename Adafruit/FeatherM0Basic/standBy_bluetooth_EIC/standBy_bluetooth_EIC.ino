#define INTREVAL(s) ((s)-1)
#define VBATPIN A7

#include <RTCZero.h>

#define PIN_TSW (5)
#define PIN_LED (6)

RTCZero rtcZERO;
unsigned long aCounter = 0;

void onTSW() {
  aCounter = 0;
}

void setup()
{
  Serial1.begin(115200);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  pinMode(PIN_TSW, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_TSW), onTSW, FALLING);

  rtcZERO.begin();
  // see http://forum.arduino.cc/index.php?topic=410699.msg2828987#msg2828987
  EIC->CONFIG[1].bit.FILTEN1 = 1;
  EIC->WAKEUP.vec.WAKEUPEN = (1 << 15); // D5 = EXTINT15
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK2 |  GCLK_CLKCTRL_ID( GCLK_CLKCTRL_ID_EIC_Val );
  while (GCLK->STATUS.bit.SYNCBUSY) {
    delay(1);
  }
  //
  rtcZERO.attachInterrupt(alarmSR);
  rtcZERO.enableAlarm(rtcZERO.MATCH_SS);
}

void loop()
{
  digitalWrite(PIN_LED, HIGH);
  // --------------------------------------------------------------------------------------------------
  float lipoV = (analogRead(VBATPIN) * (2 * 3.3)) / 0x400;

  int thisH = rtcZERO.getHours();
  int thisM = rtcZERO.getMinutes();
  int thisS = rtcZERO.getSeconds();

  String hms = String(thisH) + ":" + String(thisM) + ":" + String(thisS);

  String line = String(aCounter) + " at " + hms + " " + "VB=" + String(lipoV, 2);
  Serial1.println(line);

  //  int dd = random(1,3000);
  //  Serial1.println(dd);
  //  delay(dd);

  // --------------------------------------------------------------------------------------------------
  digitalWrite(PIN_LED, LOW);
  rtcZERO.setAlarmSeconds((rtcZERO.getSeconds() + INTREVAL(3)) % 60);
  rtcZERO.standbyMode();
}

void alarmSR() // alarm Service Routine
{
  aCounter++;
}
