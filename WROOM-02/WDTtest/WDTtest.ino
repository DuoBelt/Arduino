#include <ESP8266WiFi.h>

// WDT 全然動いてくんないし ...

static int counter = 0;

void setup() {
  Serial.begin(115200);
  ESP.wdtEnable(WDTO_8S);
}

void loop() {
  if(counter<15){
    Serial.println(counter++);
    delay(1000);
  }
  else{
    ESP.reset();
  }
}
