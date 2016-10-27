#define PIN_LED 9

#define AIN1 4
#define AIN2 5
#define PWMA 10

int counter = 0;

void setup() {
  while (true) {
    if (Serial) break;
  }
  Serial.begin(115200);
  Serial.println("Start");
  pinMode(PIN_LED, OUTPUT);

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);

}

void loop() {
  int a;
  digitalWrite(AIN1, counter % 2 ? LOW : HIGH);
  digitalWrite(AIN2, counter % 2 ? HIGH : LOW);

  digitalWrite(PIN_LED, HIGH);

  for (a = 0; a < 0x100; a++) {
    Serial.println(a);
    analogWrite(PWMA, a);
    delay(10);
  }
  counter++;
  digitalWrite(PIN_LED, LOW);

  delay(1000);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
}
