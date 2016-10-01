int uploadThis(char *host, char *url, int port)
{
  WiFiClient client;
  if (client.connect(host, port)) {
    digitalWrite(LEDPIN, HIGH);

    client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");

    delay(1000);

    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    digitalWrite(LEDPIN, LOW);
  }
  return (0);
}


