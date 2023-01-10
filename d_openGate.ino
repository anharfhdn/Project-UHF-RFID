void openGate() {
  openGateMillis = millis() + 5000;
  digitalWrite(Relay, LOW);
  //servoPintu.write(90);
}
