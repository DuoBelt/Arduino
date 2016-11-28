#include <Arduino.h>   // required before wiring_private.h
#include "wiring_private.h" // pinPeripheral() function

#include <Adafruit_BME280.h>
#include <Adafruit_INA219.h>

#define INTREVAL(s) ((s)-1)
#define VBATPIN A7

#include <RTCZero.h>

#define PIN_TSW (5)
#define PIN_LED (6)

#define PIN_S2_RX (11)
#define PIN_S2_TX (10)

Adafruit_INA219 ina219;
Adafruit_BME280 bme280;

Uart Serial2 (&sercom1, PIN_S2_RX, PIN_S2_TX, SERCOM_RX_PAD_0, UART_TX_PAD_2);
RTCZero rtcZERO;
unsigned long aCounter = 0;

void SERCOM1_Handler() {
  Serial2.IrqHandler();
}

void onTSW() {
  aCounter = 0;
}

void setup()
{
  Serial1.begin(115200);
  Serial2.begin(115200);

  // Assign pins 10 & 11 SERCOM functionality
  pinPeripheral(PIN_S2_RX, PIO_SERCOM);
  pinPeripheral(PIN_S2_TX, PIO_SERCOM);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  pinMode(PIN_TSW, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_TSW), onTSW, FALLING);

  ina219.begin();
  bme280.begin(0x76);

  rtcZERO.begin();
  // see http://forum.arduino.cc/index.php?topic=410699.msg2828987#msg2828987
  EIC->CONFIG[1].bit.FILTEN1 = 1;
  EIC->WAKEUP.vec.WAKEUPEN = (1 << 15); // D5 = EXTINT15
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK2 |  GCLK_CLKCTRL_ID( GCLK_CLKCTRL_ID_EIC_Val );
  while (GCLK->STATUS.bit.SYNCBUSY) {
    delay(1);
  }
  //
  Serial1.println("Version " + String(__TIMESTAMP__) + " start");
  //
  GPSwriteln((char *)"PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"); // only RMC
  GPSwriteln((char *)"PMTK220,5000"); // 5 sec interval
  GPSwriteln((char *)"PMTK250,1,0,115200"); // RTMC
  GPSwriteln((char *)"PMTK301,1"); // RTMC
  GPSwriteln((char *)"PMTK330,1"); // Tokyo - JAPAN
  //
  rtcZERO.attachInterrupt(alarmSR);
  rtcZERO.enableAlarm(rtcZERO.MATCH_SS);
}

void loop()
{
  char buffer[0x400];
  
  digitalWrite(PIN_LED, HIGH);
  // --------------------------------------------------------------------------------------------------
  float thisT = bme280.readTemperature();
  float thisH = bme280.readHumidity();
  float thisP = bme280.readPressure() / 100.0F;

  String tph =  String(thisT, 2) + ":" + String(thisP, 2) + ":" + String(thisH, 2);

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

  String hms = String(rtcZERO.getHours()) + ":" + String(rtcZERO.getMinutes()) + ":" + String(rtcZERO.getSeconds());
  String va = String(loadvoltage, 2) + "V" + ":" + String(current_mA, 2) + "mA";

  String line = String(aCounter) + " at " + hms + " VB=" + String(lipoV, 2) + " VA=" + va + " TPH=" + tph;
  Serial1.println(line);

  //  int dd = random(1,3000);
  //  Serial1.println(dd);
  //  delay(dd);

  // --------------------------------------------------------------------------------------------------
//  int length = GPSreadln(buffer);
//  if (length) {
//    //    Serial.println(buffer);
//    checkNMEA(buffer);
//  }
  while(Serial2.available()>0){
    char c = Serial2.read();
    Serial1.print(c);
  }
  // --------------------------------------------------------------------------------------------------
  digitalWrite(PIN_LED, LOW);
  rtcZERO.setAlarmSeconds((rtcZERO.getSeconds() + INTREVAL(3)) % 60);
  rtcZERO.standbyMode();
}

void alarmSR() // alarm Service Routine
{
  aCounter++;
}
