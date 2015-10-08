void setup() {
  Serial.begin(115200);
  pinMode(9,INPUT);
  pinMode(17,OUTPUT);
}

bool lastStatus = false;

void loop() {
  bool thisStatus = digitalRead(9);
  if(thisStatus != lastStatus){
    Serial.println(thisStatus? "On":"Off");
    digitalWrite(17,thisStatus? LOW:HIGH);
    lastStatus = thisStatus;
    delay(100);
  }
}
