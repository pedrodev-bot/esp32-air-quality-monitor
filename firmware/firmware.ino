// ESP32 Air Quality Detection System firmware
// v0.7.0 - MQ-5 Filtered + DHT + Telegram alerts + MQTT (Home Assistant)
#include "config.h"
#include "wifi_manager.h"
#include "telegram_notifier.h"
#include "mq5_sensor.h"
#include "dht_sensor.h"
#include "air_quality.h"
#include "display_manager.h"
#include "mqtt_manager.h"
#include "openweather_manager.h"
#include "hardware_alerts.h"

static unsigned long lastRead = 0;
static AirQuality::Level lastLevel = AirQuality::Level::BOM;

// Store last known sensor values for continuous display rendering
static float lastTemp = NAN;
static float lastHum = NAN;
static bool lastDhtOk = false;

// Moving average variables for MQ-5 filtering
const int numReadings = 10;
int readings[numReadings];
int readIndex = 0;
long total = 0;
bool filterInitialized = false;

// Format Telegram messages with sensor data
static String formatMessage(const char* title, int mq5Filtered, bool dhtOk, float t, float h, int aqi) {
  String m = String(title) + "\nMQ-5 Local: " + String(mq5Filtered);
  
  if (dhtOk) {
    m += "\nTemp: " + String(t, 1) + " °C | Hum: " + String(h, 0) + " %";
  }

  // Cross-reference data logic for Telegram
  if (aqi > 0) {
    m += "\nRegional AQI (OpenWeather): " + String(aqi);
    if (aqi <= 2 && mq5Filtered > 300) { 
      // Example threshold: Local gas is high, but external air is good
      m += "\n⚠️ EXTERNAL AIR IS CLEAN. Possible internal gas leak or smoke!";
    } else if (aqi >= 4) {
      m += "\n🌫️ External pollution is currently high in your region.";
    }
  } else {
    m += "\nRegional AQI: Fetching data...";
  }
  
  return m;
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.printf("\n=== Air Quality v%s ===\n", "0.7.0");

  // Initialize sensors and display
  MQ5::begin();
  DhtSensor::begin();
  DisplayManager::begin();
  setupMQTT();
  setupHardwareAlerts();

  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }

  // Connect to Wi-Fi and send startup alert
  if (WifiManager::begin()) {
    TelegramNotifier::sendMessage(String("✅ Air quality sensor online (v0.5.0)"));
  }
}

void loop() {
  // Maintain Wi-Fi and MQTT connection (non-blocking)
  WifiManager::ensureConnected();
  handleMQTTConnection();
  fetchRegionalAQIAsync();
  DisplayManager::update(lastTemp, lastHum);
  bool isWarmingUp = !MQ5::isWarmedUp();
  handleHardwareAlerts(lastLevel, isWarmingUp);

  // Non-blocking delay using millis() - Syntax error fixed
  if (millis() - lastRead < SENSOR_READ_INTERVAL_MS) {
    return;
  }
  lastRead = millis();

  // Wait for MQ-5 warm-up period
  if (!MQ5::isWarmedUp()) {
    Serial.println("[MQ5] Warming up sensor...");
    return;
  }

  // MQ-5 Filter Routine (Moving Average)
  int mq5Raw = MQ5::readRaw();
  if (!filterInitialized) {
    // Fill the array on the first valid read to avoid false positives starting at 0
    for (int i = 0; i < numReadings; i++) {
      readings[i] = mq5Raw;
      total += mq5Raw;
    }
    filterInitialized = true;
  }
  
  total = total - readings[readIndex];
  readings[readIndex] = mq5Raw;
  total = total + readings[readIndex];
  readIndex = (readIndex + 1) % numReadings;
  int mq5Filtered = total / numReadings;

  // DHT Sensor Read (updates global variables for the display)
  lastDhtOk = DhtSensor::read(lastTemp, lastHum);
  int currentAQI = getRegionalAQI();

  // Publish data to Home Assistant via MQTT
  publishSensorData(lastDhtOk ? lastTemp : NAN, lastDhtOk ? lastHum : NAN, mq5Filtered, currentAQI);

  // Classify air quality level based on filtered data
  AirQuality::Level level = AirQuality::classify(mq5Filtered);
  Serial.printf("[SENSOR] MQ5_Raw=%d | MQ5_Filtered=%d | T=%.1fC | UR=%.0f%% | level=%s\n",
                mq5Raw, mq5Filtered, lastDhtOk ? lastTemp : NAN, lastDhtOk ? lastHum : NAN, AirQuality::name(level));

  // Handle alerts based on quality level
  if (level == AirQuality::Level::CRITICO) {
    TelegramNotifier::sendAlert("CRITICO",
      formatMessage("🚨 CRITICAL air quality!", mq5Filtered, lastDhtOk, lastTemp, lastHum, currentAQI), ALERT_COOLDOWN_MS);
  } else if (level == AirQuality::Level::ATENCAO) {
    TelegramNotifier::sendAlert("ATENCAO",
      formatMessage("⚠️ Air quality warning", mq5Filtered, lastDhtOk, lastTemp, lastHum, currentAQI), ALERT_COOLDOWN_MS);
  } else if (lastLevel != AirQuality::Level::BOM) {
    TelegramNotifier::resetCooldown();
    TelegramNotifier::sendMessage(
      formatMessage("✅ Air quality normalized", mq5Filtered, lastDhtOk, lastTemp, lastHum, currentAQI));
  }
  lastLevel = level;
}