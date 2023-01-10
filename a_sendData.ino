String sendData(String parameter, char* domain) {
  //client.setNoDelay(true);
  bool needRedir = false;
  if (domain == NULL) {
    domain = (char*)host;
    needRedir = true;
    parameter = "https://script.google.com/macros/s/" + GAS_ID + "/exec?" + parameter;
  }

  String result = "";
  client.setInsecure();
  client.print(String("GET ") + parameter + " HTTP/1.1\r\n" +
               "Host: " + domain + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  if (!client.connect(host, httpsPort)) {
    return "";
  }
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (needRedir) {
      int ind = line.indexOf("/macros/echo?user");
      if (ind > 0) {
        line = line.substring(ind);
        ind = line.lastIndexOf("\r");
        line = line.substring(0, ind);
        result = line;
      }
    }

    if (line == "\r") {
      break;
    }
  }
  while (client.available()) {
    String line = client.readStringUntil('\n');
    if (!needRedir)
      if (line.length() > 5)
        result = line;
  }
  if (needRedir)
    return sendData(result, "script.googleusercontent.com");
  else return result;
}
