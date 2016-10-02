#define PIN_SERIAL2_RX 39  // D12
#define PIN_SERIAL2_TX 41  // D10

Uart Serial2(&sercom1, PIN_SERIAL2_RX, PIN_SERIAL2_TX);

void setup() {
  Serial2.begin(115200);
}

void loop() {
  if (Serial2.available())
  {
    byte byteRead = Serial2.read();
    Serial2.write(byteRead);
  }
}

void SERCOM1_Handler() {
  Serial2.IrqHandler();
}

