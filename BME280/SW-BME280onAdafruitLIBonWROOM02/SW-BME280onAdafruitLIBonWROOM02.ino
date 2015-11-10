#include <BME280_MOD-1022.h>

#include <Wire.h>
#include <SPI.h>

// Vio - NC
// Vcore - Vcc
// GND - GND
// CS8(?) - Vcc
// SDI - A4
// SCK - A5
// SDO - GND

Adafruit_BME280 bme; // I2C

void setup() {
  Serial.begin(115200);
  Serial.println("start BME280 Logger");

  bme.begin(0x76); // Adafruit:0x77 SwitchScience:0x76
}

void loop() {
  float t = bme.readTemperature();
  float p = bme.readPressure() / 100.0F;
  float h = bme.readHumidity();

  String info = "T:P:H = " + String(t) + ' ' + String(p) + ' ' + String(h);
  Serial.println(info);
  
  delay(1000*1);
}

