#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

ESP8266WiFiMulti WiFiMulti;

void setup() {
  const char ssid[] = "ms101";
  const char password[] = "sekitakovich";

  Serial.begin(115200);

  WiFiMulti.addAP(ssid,password);
}

void loop() {
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    http.begin("www.klabo.co.jp", 80, "~/k-seki/robots.txt"); //HTTP
    int httpCode = http.GET();
    if (httpCode) {
      if (httpCode == 200) {
        String payload = http.getString();
        Serial.println(payload);
      }
    }
    else {
      Serial.print("[HTTP] GET... failed, no connection or no HTTP server\n");
    }
  }
  delay(1000*10);
}



