// Settings
#define BLYNK_PRINT Serial // Blynk Output
#define TYPE_DHT DHT22 // DHT Sensor Type
#define LCD LiquidCrystal_I2C // LCD Type

// Blynk virtual pins
#define PIN_TEMP V0 // Temperature
#define PIN_HMDT V1 // Humidity
#define PIN_MOIS V2 // Soil Moisture
#define PIN_LGHT V3 // Light Value

// ESP32 pins
#define PIN_DHT 0   // DHT sensor (Temperature & Humidity)
#define PIN_LDR 1   // LDR sensor (Lightness)
#define PIN_SMT 2   // SMt sensor (Soil Moisture)

// Include secrets
#include "secrets.h"

// Include libraries
#include <DHT.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_I2C.h>

// LDR Characteristics
const float GAMMA = 0.7;
const float RL10 = 50;

DHT dht(PIN_DHT, TYPE_DHT); // Setup DHT sensor
LCD lcd(0x27, 16, 2); // Setup LCD screen
BlynkTimer timer; // Setup Blynk timer

// Data measured from sensors
struct {
  float humidity;
  float temperature;
  float lightness;
  int moisture;
} measure;

// Convert light sensor data to light value
float readLightness(int analogValue) {
  float voltage = analogValue / 4096. * 5;
  float resistance = 2000 * voltage / (1 - voltage / 5);
  float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));
  // Serial.print(F("A: ")); Serial.println(analogValue);
  // Serial.print(F("V: ")); Serial.println(voltage);
  // Serial.print(F("R: ")); Serial.println(resistance);
  // Serial.print(F("L: ")); Serial.println(lux);
  return lux;
}

void setup() {
  Wire.begin(7, 6);
  Serial.begin(9600);
  pinMode(PIN_LDR, INPUT);

  // Startup LCD screen
  lcd.init();
  lcd.setCursor(0, 0); lcd.print(F("PflanzenMonitor"));
  lcd.setCursor(5, 1); lcd.print(F("by TrietNTM"));

  // Connect and login to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);

  // Show LCD user interface guide
  lcd.clear(); lcd.backlight();
  lcd.setCursor(0, 0); lcd.print(F("Temprt  Moisture"));
  lcd.setCursor(0, 1); lcd.print(F("Humidt  Lightnss"));

  // Service routines
  timer.setInterval(500L, readValues);    // Read values every 0.5 sec
  timer.setInterval(800L, displayValues); // Display on LCD every 0.8 sec
  timer.setInterval(1000L, sendValues);   // Send data to Blynk every 1 sec

  // Deep sleep after 60 secs
  timer.setTimeout(60000L, esp_deep_sleep_start);
}

void loop() {
  // Run Blynk and Blynk timer every 0.1 seconds
  Blynk.run(); timer.run(); delay(100);
}

void readValues() {
  // Gather raw light sensor data
  int lightSensor = analogRead(PIN_LDR);

  // Gather measured values
  measure.temperature = dht.readTemperature();
  measure.humidity = dht.readHumidity();
  measure.lightness = readLightness(lightSensor);
  measure.moisture = analogRead(PIN_SMT);
}

// Display measured values on LCD
void displayValues() {
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print(F("T ")); lcd.print(measure.temperature);
  lcd.setCursor(0, 1); lcd.print(F("H ")); lcd.print(measure.humidity);
  lcd.setCursor(8, 0); lcd.print(F("M ")); lcd.print(measure.moisture);
  lcd.setCursor(8, 1); lcd.print(F("L ")); lcd.print(measure.lightness);
}

// Send measured values to Blynk
void sendValues() {
  Blynk.beginGroup();
  Blynk.virtualWrite(PIN_TEMP, measure.temperature);
  Blynk.virtualWrite(PIN_HMDT, measure.humidity);
  Blynk.virtualWrite(PIN_LGHT, measure.lightness);
  Blynk.virtualWrite(PIN_MOIS, measure.moisture);
  Blynk.endGroup();
}
