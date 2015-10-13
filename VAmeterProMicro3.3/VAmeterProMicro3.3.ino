#include <Wire.h>

#include <Adafruit_INA219.h>
#include <wI2cLcdACM1602.h>

Adafruit_INA219 ina219;
wI2cLcdACM1602 lcd;           //

static unsigned int counter;
unsigned long lastMillis;
bool ledON;

void setup(void)
{
  uint32_t currentFrequency;

  counter = 0;

  lastMillis = 0;
  ledON = false;
  pinMode(13,OUTPUT);
  pinMode(3,INPUT_PULLUP);
  attachInterrupt(1,onPush,FALLING);

  while(!Serial);
  
  Serial.begin(115200);
  Serial.println("Measuring voltage and current with INA219 ...");

  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  ina219.begin();
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();

  lcd.begin();        // LCD初期化

  lcd.clear();        // 全クリア
  lcd.noBlink();      // カーソル点滅なし
}

void onPush(){
  unsigned long thisMillis = millis();
  if((thisMillis - lastMillis)>20){
    lastMillis = thisMillis;
    ledON = !ledON;
    digitalWrite(13,ledON);
  }
}

void loop(void)
{
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  loadvoltage = busvoltage + (shuntvoltage / 1000);

  if((counter%16) == 0){
    Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
    Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
    Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
    Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
    Serial.println("");
  }

  counter++;
  delay(1000/10);
}
