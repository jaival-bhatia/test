#include <Arduino.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

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

// ---------- Sensor Thresholds ----------
#define TEMP_THRESHOLD 30.0
#define LIGHT_THRESHOLD 2000
#define GAS_THRESHOLD 3000
#define DIST_THRESHOLD 10

// ---------- Objects ----------
DHT dht(DHTPIN, DHTTYPE);
OneWire oneWire(DS18B20_PIN);
DallasTemperature sensors(&oneWire);
Adafruit_SSD1306 display(128, 64, &Wire, -1);

// ---------- Setup ----------
void setup() {
  Serial.begin(115200);
  dht.begin();
  sensors.begin();
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

  digitalWrite(RELAY_TEMP, LOW);
  digitalWrite(RELAY_LDR, LOW);
  digitalWrite(RELAY_PIR, LOW);

  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

// ---------- Functions ----------
long readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // timeout 30ms
  return duration * 0.034 / 2;
}

void setRGB(bool r, bool g, bool b) {
  digitalWrite(RED_PIN, r);
  digitalWrite(GREEN_PIN, g);
  digitalWrite(BLUE_PIN, b);
}

void updateOLED(float tempDHT, float tempDS, int ldr, bool motion, int gas, long dist) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.printf("Temp: %.1f/%.1fC\n", tempDHT, tempDS);
  display.printf("LDR : %d\n", ldr);
  display.printf("Motion: %s\n", motion ? "Yes" : "No");
  display.printf("Gas: %d\n", gas);
  display.printf("Dist: %ldcm\n", dist);
  display.display();
}

void printSensorData(float tempDHT, float tempDS, int ldr, bool motion,
                     int gas, long dist, bool r1, bool r2, bool r3) {
  Serial.println(F("\n================= SENSOR DATA ================="));
  Serial.printf("DHT Temp     : %.1f °C\n", tempDHT);
  Serial.printf("DS18B20 Temp : %.1f °C\n", tempDS);
  Serial.printf("LDR Value    : %d\n", ldr);
  Serial.printf("Motion       : %s\n", motion ? "Detected" : "None");
  Serial.printf("Gas Value    : %d\n", gas);
  Serial.printf("Distance     : %ld cm\n", dist);
  Serial.println(F("---------------- RELAY STATES ----------------"));
  Serial.printf("Relay Temp   : %s\n", r1 ? "ON" : "OFF");
  Serial.printf("Relay LDR    : %s\n", r2 ? "ON" : "OFF");
  Serial.printf("Relay PIR    : %s\n", r3 ? "ON" : "OFF");
  Serial.println(F("=============================================\n"));
}

void loop() {
  // Read sensors
  float tempDHT = dht.readTemperature();
  sensors.requestTemperatures();
  float tempDS = sensors.getTempCByIndex(0);
  int ldr = analogRead(LDR_PIN);
  bool motion = digitalRead(PIR_PIN);
  int gas = analogRead(MQ2_PIN);
  long dist = readDistanceCM();

  // Relay logic
  bool relay1 = tempDHT > TEMP_THRESHOLD || tempDS > TEMP_THRESHOLD;
  bool relay2 = ldr < LIGHT_THRESHOLD;
  bool relay3 = motion;

  digitalWrite(RELAY_TEMP, relay1);
  digitalWrite(RELAY_LDR, relay2);
  digitalWrite(RELAY_PIR, relay3);

  // Buzzer for gas
  digitalWrite(BUZZER_PIN, gas > GAS_THRESHOLD ? HIGH : LOW);

  // RGB LED Logic
  if (gas > GAS_THRESHOLD || dist < DIST_THRESHOLD)
    setRGB(HIGH, LOW, LOW); // 🔴 Red
  else if (motion)
    setRGB(LOW, LOW, HIGH); // 🔵 Blue
  else if (relay1)
    setRGB(HIGH, HIGH, LOW); // 🟠 Yellow
  else
    setRGB(LOW, HIGH, LOW); // 🟢 Green

  // OLED & Serial output
  updateOLED(tempDHT, tempDS, ldr, motion, gas, dist);
  printSensorData(tempDHT, tempDS, ldr, motion, gas, dist, relay1, relay2, relay3);

  delay(2000);
}
