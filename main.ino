//#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
WiFiClientSecure client;

#include <SoftwareSerial.h>
SoftwareSerial nilai(18, 19);

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

#define ON_Board_LED 2
#define Relay 23

const char* ssid = "POCONGM3";
const char* password = "donatsiang";

const char* host = "script.google.com";
const int httpsPort = 443;
const String location = "Kontrakan";

String GAS_ID =  "AKfycbyPMtpgNJejsp8MpXKNlbP9efYg_PjZFTvGPjRo_ttReDiDbhG5Z6XxVcHlOAhlCc4h";

uint64_t openGateMillis = 0;
unsigned long previousMillis = 0;
unsigned long interval = 30000;

void fromGoogle();
void closeGate();
void openGate();

byte getTID[] = {'\n', 'R', '2', ',', '0', ',', '6', '\r'};    // 12byte
byte getEPC[] = {'\n', 'R', '1', ',', '2', ',', '6', '\r'};    // 12byte

#define RXD2 16
#define TXD2 17

void setup() {
  Serial.begin(9600);
  nilai.begin(38400);

  pinMode(ON_Board_LED, OUTPUT);
  pinMode(Relay, OUTPUT);
  digitalWrite(ON_Board_LED, HIGH);
  digitalWrite(Relay, HIGH);

  Wire.begin(21, 22);
  lcd.begin();
  lcd.setCursor(0, 0);
  lcd.print("--------------------");
  lcd.setCursor(2, 1);
  lcd.print("UHF RFID READER ");
  lcd.setCursor(5, 2);
  lcd.print("Loading...");
  lcd.setCursor(0, 3);
  lcd.print("--------------------");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(ON_Board_LED, LOW);
    delay(250);
    digitalWrite(ON_Board_LED, HIGH);
    delay(250);
  }
  digitalWrite(ON_Board_LED, HIGH);
  client.setInsecure();
}

void loop() {
  unsigned long currentMillis = millis();
  if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >= interval)) {
    WiFi.disconnect();
    WiFi.reconnect();
    previousMillis = currentMillis;
  }

  if (openGateMillis > 0 && openGateMillis < millis()) {
    //    servoPintu.write(0);
    //    if (servoPintu.read()) {
    //      millis() + 1000;
    closeGate();
  }


  lcd.setCursor(0, 0);
  lcd.print("--------------------");
  lcd.setCursor(2, 1);
  lcd.print("UHF RFID READER ");
  lcd.setCursor(5, 2);
  lcd.print("  Ready!   ");
  lcd.setCursor(0, 3);
  lcd.print("--------------------");

  nilai.write(getEPC, sizeof(getEPC));
  String rfidData;
  while (nilai.available()) {
    char in = nilai.read();
    rfidData += in;

  }
  if (rfidData.length() > 10) {
    rfidData.remove(0, 1);
    rfidData.toUpperCase();
    if (rfidData.substring(0, 1) == "R") {
      rfidData.remove(0, 1);
    }
    //Serial.println(rfidData);
    lcd.setCursor(0, 0);
    lcd.print("--------------------");
    lcd.setCursor(2, 1);
    lcd.print("Send to Database");
    lcd.setCursor(4, 2);
    lcd.print("Getting Data");
    lcd.setCursor(0, 3);
    lcd.print("--------------------");
    String getData = sendData("uid=" + rfidData + "&id=" + location, NULL);
    fromGoogle(getData);
  } else {
    //Serial.println();
  }
  delay(5000);
  lcd.clear();

}
