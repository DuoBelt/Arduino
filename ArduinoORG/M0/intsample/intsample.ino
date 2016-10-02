void setup() {
  SerialUSB.begin(115200);
  while(!SerialUSB);

  SerialUSB.println("hello");

  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);

  attachInterrupt(A0, onA0Change, CHANGE);
  attachInterrupt(A1, onA1Change, CHANGE);
  attachInterrupt(A2, onA2Change, CHANGE);
  attachInterrupt(A3, onA3Change, CHANGE);
  attachInterrupt(A4, onA4Change, CHANGE);
  attachInterrupt(A5, onA5Change, CHANGE);
}

void loop() {
}

void onA0Change() {
  SerialUSB.println("A0 change");
}

void onA1Change() {
  SerialUSB.println("A1 change");
}

void onA2Change() {
  SerialUSB.println("A2 change");
}

void onA3Change() {
  SerialUSB.println("A3 change");
}

void onA4Change() {
  SerialUSB.println("A4 change");
}

void onA5Change() {
  SerialUSB.println("A5 change");
}
