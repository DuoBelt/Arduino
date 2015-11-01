/*
 *  Simple HTTP get webclient test
 */

#include "Ticker.h"
Ticker ticker;
unsigned int counter = 0;

#include <ESP8266WiFi.h>
extern "C" {
#include "user_interface.h"
}

const char* ssid     = "ms101";
const char* password = "sekitakovich";

const char* host = "www.klabo.co.jp";

void cyclePoint(){
  counter++;
  digitalWrite(4,counter%2);
}

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  pinMode(4, OUTPUT);
  ticker.attach(1.0, cyclePoint);
  
//    wifi_set_sleep_type(NONE_SLEEP_T);
//    wifi_set_sleep_type(MODEM_SLEEP_T);
  wifi_set_sleep_type(LIGHT_SLEEP_T);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

int value = 0;

void loop() {
  delay(5000);
  ++value;

  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "/index.html";
  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  delay(10);

  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");
}
