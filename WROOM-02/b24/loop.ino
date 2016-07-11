void loop() {

  float shuntvoltage = ina219.getShuntVoltage_mV();
  float busvoltage = ina219.getBusVoltage_V();
  float current_mA = ina219.getCurrent_mA();
  float loadvoltage = busvoltage + (shuntvoltage / 1000);

  float t = bme.readTemperature();
  float p = bme.readPressure() / 100.0F;
  float h = bme.readHumidity();

  static char host[] = "www.klabo.co.jp";
  char thisURL[0x100];

  delayMS = 1000 * (((loadvoltage > lastSendV) || (current_mA > lastSendA)) ? 5 : 60);

  String getOption = "t=" + String(t) + "&h=" +  String(h) + "&p=" + String(p) + "&mac=" + thisMAC + "&up=" + upCount++ + "&spv=" + String(loadvoltage, 3) + "&spa=" + String(current_mA, 3);
  String getString = String("/tph.php?")  + getOption;

  getString.toCharArray(thisURL, sizeof(thisURL));

  uploadThis(host, thisURL, 80);

  Serial.println("onto sleep (" + String(delayMS) + ")");
  delay(delayMS);
  lastSendV = loadvoltage;
  lastSendA = current_mA;
}

