void setup() {
  while (true) {
    if (Serial) break;
  }
  Serial.begin(115200);

  Serial1.begin(115200);

  Serial.println("Start!!!");

  Serial1.println((char *)"PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"); // only RMC
  Serial1.println((char *)"PMTK220,5000"); // 5 sec interval
  Serial1.println((char *)"PMTK250,1,0,9600"); // RTMC
  Serial1.println((char *)"PMTK301,1"); // RTMC
  Serial1.println((char *)"PMTK330,1"); // Tokyo - JAPAN

  //  Serial1.println((char *)"PMTK251,115200");
}

void loop() {
  int bytes;
  if ((bytes = Serial1.available()) > 0) {
    while (bytes--) {
      char c = Serial1.read();
      Serial.print(c);
    }
  }
  else {
    Serial.println("Waiting");
    delay(1000);
  }
}
