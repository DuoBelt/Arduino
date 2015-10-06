#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

//#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  Serial.begin(115200);
  Serial.println("start BME280 test");

  bme.begin(0x76); // Adafruit:0x77 SwitchScience:0x76
}

void loop() {
  String info = String("T=") + bme.readTemperature() + String(",P=") + bme.readPressure() / 100.0F + String(",H=") + bme.readHumidity();
  Serial.println(info);
  delay(2000);
}
