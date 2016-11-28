#include <Arduino.h>   // required before wiring_private.h
#include "wiring_private.h" // pinPeripheral() function
#include <Adafruit_INA219.h>

#define INTREVAL(s) ((s)-1)
#define VBATPIN A7

#include <RTCZero.h>

#define PIN_TSW (5)
#define PIN_LED (6)

#define PIN_S2_RX (11)
#define PIN_S2_TX (10)

Uart Serial2 (&sercom1, PIN_S2_RX, PIN_S2_TX, SERCOM_RX_PAD_0, UART_TX_PAD_2);
RTCZero rtcZERO;
Adafruit_INA219 ina219;

volatile unsigned long aCounter = 0;

void SERCOM1_Handler() {
  Serial2.IrqHandler();
}

void onTSW() {
  aCounter = 0;
}

int offset = 0;
char buffer[0x400];

void alarmSR() // alarm Service Routine
{
  aCounter++;
}

void setup() {
  Serial1.begin(115200);
  Serial2.begin(115200);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  pinMode(PIN_TSW, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_TSW), onTSW, FALLING);

  pinPeripheral(PIN_S2_RX, PIO_SERCOM);
  pinPeripheral(PIN_S2_TX, PIO_SERCOM);

  delay(100);

  ina219.begin();

  //  Serial1.println((char *)"PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"); // only RMC
  //  sendPMTK((char *)"PMTK314,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"); // only GGA
  sendPMTK((char *)"PMTK314,0,1,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"); // only RMC+GGA
  sendPMTK((char *)"PMTK220,5000"); // sec interval
  sendPMTK((char *)"PMTK250,1,0,115200"); // RTMC
  sendPMTK((char *)"PMTK301,1"); // RTMC
  sendPMTK((char *)"PMTK330,1"); // Tokyo - JAPAN

  Serial2.println("Version " + String(__TIMESTAMP__) + " start");

//  Serial2.println(EIC->WAKEUP.vec.WAKEUPEN);
  
  rtcZERO.begin();
  // see http://forum.arduino.cc/index.php?topic=410699.msg2828987#msg2828987
  EIC->CONFIG[1].bit.FILTEN1 = 1;
    EIC->WAKEUP.vec.WAKEUPEN = EIC->WAKEUP.vec.WAKEUPEN | (1 << 15); // D5 = EXTINT15
//  EIC->WAKEUP.vec.WAKEUPEN = ((1 << 15) | (1 << 1) | (1 << 0)); // D5 = EXTINT15
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK2 |  GCLK_CLKCTRL_ID( GCLK_CLKCTRL_ID_EIC_Val );
  while (GCLK->STATUS.bit.SYNCBUSY) {
    delay(1);
  }

//    Serial2.println(EIC->WAKEUP.vec.WAKEUPEN);

  rtcZERO.attachInterrupt(alarmSR);
  rtcZERO.enableAlarm(rtcZERO.MATCH_SS);
}

void loop() {
/*
  int bytes;
  if ((bytes = Serial1.available()) > 0) {
    digitalWrite(PIN_LED, HIGH);
    while (bytes--) {
      char c = Serial1.read();
      buffer[offset++] = c;
      if (c == 0x0a) {
        buffer[offset] = 0;
        Serial2.print(buffer);
        offset = 0;
      }
    }
    digitalWrite(PIN_LED, LOW);
  }
  */
  if(Serial1.available() > 0){
    digitalWrite(PIN_LED, HIGH);
    String s = Serial1.readStringUntil(0x0A);
    Serial2.print(s);
    digitalWrite(PIN_LED, LOW);
  }
  else {
    // --------------------------------------------------------------------------------------------------
    float shuntvoltage = 0;
    float busvoltage = 0;
    float current_mA = 0;
    float loadvoltage = 0;

    shuntvoltage = ina219.getShuntVoltage_mV();
    busvoltage = ina219.getBusVoltage_V();
    current_mA = ina219.getCurrent_mA();
    loadvoltage = busvoltage + (shuntvoltage / 1000);
    // --------------------------------------------------------------------------------------------------
    float lipoV = (analogRead(VBATPIN) * (2 * 3.3)) / 0x400;
    Serial2.println("LiPo " + String(lipoV, 2));
    Serial2.println("Main " + String(loadvoltage, 2));
    rtcZERO.setAlarmSeconds((rtcZERO.getSeconds() + INTREVAL(3)) % 60);
//        rtcZERO.standbyMode();
    delay(3000);
  }
}

void sendPMTK(char *p) {
  char checksum;
  char c;

  Serial1.print('$');
  for (checksum = 0; c = *p++; ) {
    checksum ^= c;
    Serial1.print(c);
  }
  Serial1.print('*');
  Serial1.println(checksum, HEX);
}
