void setup() {
  while(true){
    if(Serial){
      break;
    }
    else delay(10);
  }
  Serial.begin(115200);

  Serial1.begin(115200);

}

void loop() {
  while(Serial1.available()){
    char c = Serial1.read();
    Serial.print(c);
  }
}
