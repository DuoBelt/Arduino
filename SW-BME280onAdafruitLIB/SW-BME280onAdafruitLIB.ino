// スイッチサイエンスのそれをAdafruitのライブラリで動かすシリーズ(笑)

// Vio - NC
// Vcore - Vcc
// GND - GND
// CS8(?) - Vcc
// SDI - A4
// SCK - A5
// SDO - GND

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme; // I2C

void setup() {
  Serial.begin(115200);
  Serial.println("start BME280");

  bme.begin(0x76); // Adafruit:0x77 SwitchScience:0x76
}

void loop() {
  String info = String("T=") + bme.readTemperature() + String(",P=") + bme.readPressure() / 100.0F + String(",H=") + bme.readHumidity();
  Serial.println(info);
  delay(2000);
}
