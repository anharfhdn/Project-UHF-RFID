void fromGoogle(String data) {
  int ind = data.indexOf(":");
  String access = data.substring(0, ind);
  int nextInd = data.indexOf(":", ind + 1);
  String name = data.substring(ind + 1, nextInd);
  String text = data.substring(nextInd + 1, data.length());

  if (access == "Tidak terdaftar") {
    //mp3_play(2);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(String("Name: ") + name);

    lcd.setCursor(0, 1);
    lcd.print(String("APD: ") + text);
  }
  else if (access == "Terdaftar") {
    //mp3_play(1);
    openGate();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(String("Name: ") + name);

    lcd.setCursor(0, 1);
    lcd.print(String("APD: ") + text);
  }
}
