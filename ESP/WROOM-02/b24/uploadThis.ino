int uploadThis(char *host, char *url, int port)
{
  WiFiClient client;
  if (client.connect(host, port)) {
    digitalWrite(LEDPIN, HIGH);

  Serial.println(url);

    client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");

    delay(500);

    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    digitalWrite(LEDPIN, LOW);
  }
  return (0);
}


