#include <ESP8266WiFi.h>
#include <BME280_MOD-1022.h>
#include <Wire.h>
extern "C" {
#include "user_interface.h"
}

void printFormattedFloat(float x, uint8_t precision) {
  char buffer[10];

  dtostrf(x, 7, precision, buffer);
  Serial.print(buffer);

}

void printCompensatedMeasurements(void) {

  float temp, humidity,  pressure, pressureMoreAccurate;
  double tempMostAccurate, humidityMostAccurate, pressureMostAccurate;
  char buffer[80];

  temp      = BME280.getTemperature();
  humidity  = BME280.getHumidity();
  pressure  = BME280.getPressure();

  pressureMoreAccurate = BME280.getPressureMoreAccurate();  // t_fine already calculated from getTemperaure() above

  tempMostAccurate     = BME280.getTemperatureMostAccurate();
  humidityMostAccurate = BME280.getHumidityMostAccurate();
  pressureMostAccurate = BME280.getPressureMostAccurate();

  String nickname = "sekitakovich";
  String info = "t=" + String(tempMostAccurate) + "&h=" +  String(humidityMostAccurate) + "&p=" + String(pressureMostAccurate) + "&v=0" + "&nickname=" + nickname;
  Serial.println(info);

  const char* host = "www.klabo.co.jp";
  WiFiClient client;
  const int httpPort = 80;
  if (client.connect(host, httpPort)) {
    String url = String("/tph.php?")  + info;
    client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
  }
}

void setup()
{
  const char* ssid     = "ms101";
  const char* password = "sekitakovich";

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Wire.begin();
  Serial.begin(115200);

  //    wifi_set_sleep_type(NONE_SLEEP_T);
  //    wifi_set_sleep_type(MODEM_SLEEP_T);

  wifi_set_sleep_type(LIGHT_SLEEP_T);

//  pinMode(0, INPUT);
//  pinMode(13, OUTPUT);
//  attachInterrupt(0, sendThis, FALLING);
  //
  //  uint8_t chipID;

  Serial.println("Welcome to the BME280 MOD-1022 weather multi-sensor test sketch!");
  Serial.println("Embedded Adventures (www.embeddedadventures.com)");

  //  chipID = BME280.readChipId();

  // find the chip ID out just for fun
  //  Serial.print("ChipID = 0x");
  //  Serial.print(chipID, HEX);

  // need to read the NVM compensation parameters
  BME280.readCompensationParams();

  // Need to turn on 1x oversampling, default is os_skipped, which means it doesn't measure anything
  BME280.writeOversamplingPressure(os1x);  // 1x over sampling (ie, just one sample)
  BME280.writeOversamplingTemperature(os1x);
  BME280.writeOversamplingHumidity(os1x);

  // example of a forced sample.  After taking the measurement the chip goes back to sleep
  BME280.writeMode(smForced);
  while (BME280.isMeasuring()) {
    Serial.println("Measuring...");
    delay(50);
  }
  Serial.println("Done!");

  BME280.writeStandbyTime(tsb_0p5ms);        // tsb = 0.5ms
  BME280.writeFilterCoefficient(fc_16);      // IIR Filter coefficient 16
  BME280.writeOversamplingPressure(os16x);    // pressure x16
  BME280.writeOversamplingTemperature(os2x);  // temperature x2
  BME280.writeOversamplingHumidity(os1x);     // humidity x1

  BME280.writeMode(smNormal);

}

void sendThis() {
  digitalWrite(13,HIGH);
  while (BME280.isMeasuring()) {
    delay(1);
  }

  BME280.readMeasurements();
  printCompensatedMeasurements();

  digitalWrite(13,LOW);
}

void loop()
{
  sendThis();
  delay(1000 * 60 * 3);	// do this every n min
}

