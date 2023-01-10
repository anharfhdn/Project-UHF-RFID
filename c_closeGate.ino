void closeGate(){
  openGateMillis = 0;
  digitalWrite(Relay, HIGH);
}
