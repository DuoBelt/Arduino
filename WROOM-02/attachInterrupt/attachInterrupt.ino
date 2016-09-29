unsigned int counter = 0;

void cyclePoint(){
  digitalWrite(13, counter%2);   // turn the LED on (HIGH is the voltage level)
  counter++;  
}

void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  pinMode(14,INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(14),cyclePoint,FALLING);
}

// the loop function runs over and over again forever
void loop() {
  Serial.println("HIGH");
//  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  Serial.println("LOW");
//  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
}
