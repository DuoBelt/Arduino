#include <ESP8266WiFi.h>

void setup() {
  unsigned char MAC_STA[6];
  Serial.begin(115200);

  Serial.println("");
  Serial.print("MAC --- ");
  unsigned char *MAC  = WiFi.macAddress(MAC_STA);
  int a;
  for (a = 0; a < 6; a++) {
    char buffer[2 + 1];
    sprintf(buffer, "%02X", MAC[a]);
    Serial.print(buffer);
    if(a<5){
      Serial.print(":");
    }
  }
  Serial.println();

  char *ssid = "ms101";
  char *password = "sekitakovich";

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000 * 1);
    Serial.print(".");
  }
  Serial.println("");

}

void loop() {

}

