#define PIN_PIR (3)
#define PIN_RELAY (4)

#define LUX_OK (500)

volatile bool isAnyone = false;
volatile bool lampOn = false;

int getLUX(){
  return(0);
}

void lampSwitch(bool mode){
  digitalWrite(PIN_RELAY,mode);
  lampOn = mode;  
}

void thisISR(){
  bool isAnyone = digitalRead(PIN_PIR);  
}

void setup() {
  pinMode(PIN_PIR,INPUT);
  pinMode(PIN_RELAY,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_PIR), thisISR, CHANGE);
}

void loop() {
  int lux = getLUX();
  if(lampOn == true){
    if(lux >= LUX_OK){
      lampSwitch(false);
    }
    else if(isAnyone == false){
    }
  }
  else{
    if(lux < LUX_OK){
      if(isAnyone == true){
        lampSwitch(true);
      }
    }    
  }
  delayWDT(8);
}
