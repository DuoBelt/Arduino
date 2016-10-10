Uart Serial2 (&sercom1, 11, 10, SERCOM_RX_PAD_0, UART_TX_PAD_2);

void SERCOM1_Handler()
{
  Serial2.IrqHandler();
}

void setup() {
  Serial.begin(115200);

  Serial2.begin(115200);
}

uint8_t i=0;
void loop() {
  Serial.println(i);
  Serial2.write(i++);
  delay(10);
}
