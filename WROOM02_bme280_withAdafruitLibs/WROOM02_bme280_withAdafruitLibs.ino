#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <ESP8266WiFi.h>
extern "C" {
#include "user_interface.h"
}

Adafruit_BME280 bme; // I2C

#define DELAYMS (1000 * 60 * 1)
#define LEDPIN (13)

void setup() {
  Serial.begin(115200);
  Serial.println("Start collection");

  pinMode(LEDPIN,OUTPUT);
  
  char *ssid = "ms101";
  char *password = "sekitakovich";

  wifi_set_sleep_type(LIGHT_SLEEP_T);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000*1);
    Serial.print(".");
  }
  Serial.println("");

  bme.begin(0x76);
}

void loop() {
  float t = bme.readTemperature();
  float p = bme.readPressure() / 100.0F;
  float h = bme.readHumidity();
  float v = 0.0;

  digitalWrite(LEDPIN,HIGH);
  
  String nickname = "sekitakovich";
  String info = "t=" + String(t) + "&h=" +  String(h) + "&p=" + String(p) + "&v=" + String(v) + "&nickname=" + nickname;
  Serial.println(info);

  char *host = "www.klabo.co.jp";
  WiFiClient client;
  int httpPort = 80;
  if (client.connect(host, httpPort)) {
    String url = String("/tph.php?")  + info;
    client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");

    delay(10);
    
    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
  }

  digitalWrite(LEDPIN,LOW);
  
  delay(DELAYMS);
}
