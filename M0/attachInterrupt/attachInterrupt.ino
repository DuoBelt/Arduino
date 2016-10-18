#define PIN_TSW (3)

volatile unsigned int counter = 0;

void cyclePoint(){
  counter++;
}

void setup()
{
  SerialUSB.begin(115200);

  pinMode(PIN_TSW,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_TSW),cyclePoint,FALLING);
}

void loop()
{
  SerialUSB.println(counter);
  delay(1000);
}
