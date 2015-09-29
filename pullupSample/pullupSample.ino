const int buttonON = LOW;    // ボタンが押されているとピンの値はLOW
const int buttonOFF = HIGH;  // ボタンが押されていないとピンの値はHIGH

const int buttonPin = 3;
const int ledPin =  13;
int buttonState = 0;

void setup() {
  pinMode(ledPin, OUTPUT);      
  pinMode(buttonPin, INPUT_PULLUP);     
}

void loop(){
  buttonState = digitalRead(buttonPin);
  if (buttonState == buttonON) {     // ボタンが押されていたら
    digitalWrite(ledPin, HIGH);  
  } 
  else {
    digitalWrite(ledPin, LOW); 
  }
}

