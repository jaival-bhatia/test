#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <DallasTemperature.h>
#include <OneWire.h>

// ---------- WiFi Credentials ----------
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ---------- Firebase ----------
const String firebase_host = "https://test-acc41-default-rtdb.firebaseio.com/";
const String firebase_path = "esp32_data.json"; // will post to .../esp32_data.json

// ---------- Pin Definitions ----------
#define DHTPIN 18
#define DHTTYPE DHT22
#define DS18B20_PIN 5
#define LDR_PIN 34
#define PIR_PIN 32
#define RELAY_TEMP 2
#define RELAY_LDR 4
#define RELAY_PIR 19
#define MQ2_PIN 33
#define TRIG_PIN 14
#define ECHO_PIN 27
#define BUZZER_PIN 25
#define RED_PIN 26
#define GREEN_PIN 13
#define BLUE_PIN 12
#define OLED_SDA 21
#define OLED_SCL 22

// ---------- Sensor & Display Setup ----------
DHT dht(DHTPIN, DHTTYPE);
OneWire oneWire(DS18B20_PIN);
DallasTemperature ds18b20(&oneWire);

void setup() {
  Serial.begin(115200);

  // ---------- Setup Pins ----------
  pinMode(LDR_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(MQ2_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RELAY_TEMP, OUTPUT);
  pinMode(RELAY_LDR, OUTPUT);
  pinMode(RELAY_PIR, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // ---------- Start Sensors ----------
  dht.begin();
  ds18b20.begin();

  // ---------- Connect WiFi ----------
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
}

void sendDataToFirebase(String jsonData) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = firebase_host + firebase_path;
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.PUT(jsonData);

    Serial.print("Firebase Response: ");
    Serial.println(httpResponseCode);
    http.end();
  } else {
    Serial.println("WiFi not connected");
  }
}

long readUltrasonicCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2;
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  ds18b20.requestTemperatures();
  float dsTemp = ds18b20.getTempCByIndex(0);
  int ldrVal = analogRead(LDR_PIN);
  int pirState = digitalRead(PIR_PIN);
  int mq2Val = analogRead(MQ2_PIN);
  long distance = readUltrasonicCM();

  // Control Relays
  digitalWrite(RELAY_TEMP, t > 30);
  digitalWrite(RELAY_LDR, ldrVal < 1000);
  digitalWrite(RELAY_PIR, pirState);

  // RGB LED (simple indicator based on gas sensor)
  if (mq2Val < 1500) {
  // SAFE — GREEN ON
  digitalWrite(RED_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BUZZER_PIN, LOW);
}
else if (mq2Val >= 1500 && mq2Val <= 2500) {
  // WARNING — YELLOW (GREEN + RED ON)
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
}
else {
  // DANGER — RED ON + BUZZER
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
  digitalWrite(BUZZER_PIN, HIGH);
}

  // Firebase JSON Data
  String data = "{";
  data += "\"humidity\":" + String(h) + ",";
  data += "\"temperature_dht\":" + String(t) + ",";
  data += "\"temperature_ds\":" + String(dsTemp) + ",";
  data += "\"ldr\":" + String(ldrVal) + ",";
  data += "\"motion\":" + String(pirState) + ",";
  data += "\"mq2\":" + String(mq2Val) + ",";
  data += "\"distance\":" + String(distance) + ",";
  data += "\"relay_temp\":" + String(digitalRead(RELAY_TEMP)) + ",";
  data += "\"relay_ldr\":" + String(digitalRead(RELAY_LDR)) + ",";
  data += "\"relay_pir\":" + String(digitalRead(RELAY_PIR)) + ",";
  data += "\"led_red\":" + String(digitalRead(RED_PIN)) + ",";
  data += "\"led_green\":" + String(digitalRead(GREEN_PIN)) + ",";
  data += "\"led_blue\":" + String(digitalRead(BLUE_PIN));
  data += "}";

  Serial.println(data);
  sendDataToFirebase(data);

  delay(1000); // Update every 5 seconds
}
