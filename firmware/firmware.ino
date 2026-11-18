// ESP32 Air Quality Detection System firmware
// v0.3.1 - MQ-5 Filtered + DHT + Telegram alerts
#include "config.h"
#include "wifi_manager.h"
#include "telegram_notifier.h"
#include "mq5_sensor.h"
#include "dht_sensor.h"
#include "air_quality.h"

static unsigned long lastRead = 0;
static AirQuality::Level lastLevel = AirQuality::Level::BOM;

// Moving average variables for MQ-5 filtering
const int numReadings = 10;
int readings[numReadings];
int readIndex = 0;
long total = 0;
bool filterInitialized = false;

// Format Telegram messages with sensor data
static String formatMessage(const char* title, int mq5Filtered, bool dhtOk, float t, float h) {
  String m = String(title) + "\nMQ-5 (filtered): " + String(mq5Filtered);
  if (dhtOk) {
    m += "\nTemperature: " + String(t, 1) + " °C";
    m += "\nHumidity: " + String(h, 0) + " %";
  } else {
    m += "\nTemperature/humidity: unavailable";
  }
  return m;
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.printf("\n=== Air Quality v%s ===\n", FW_VERSION);

  // Initialize sensors and filter array
  MQ5::begin();
  DhtSensor::begin();
  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }

  // Connect to Wi-Fi and send startup alert
  if (WifiManager::begin()) {
    TelegramNotifier::sendMessage(String("✅ Air quality sensor online (v") + FW_VERSION + ")");
  }
}

void loop() {
  // Maintain Wi-Fi connection (non-blocking)
  WifiManager::ensureConnected();

  // Non-blocking delay using millis()
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

  // DHT Sensor Read
  float t = NAN, h = NAN;
  bool dhtOk = DhtSensor::read(t, h);

  // Classify air quality level based on filtered data
  AirQuality::Level level = AirQuality::classify(mq5Filtered);
  Serial.printf("[SENSOR] MQ5_Raw=%d | MQ5_Filtered=%d | T=%.1fC | UR=%.0f%% | level=%s\n",
                mq5Raw, mq5Filtered, dhtOk ? t : NAN, dhtOk ? h : NAN, AirQuality::name(level));

  // Handle alerts based on quality level
  if (level == AirQuality::Level::CRITICO) {
    TelegramNotifier::sendAlert("CRITICO",
      formatMessage("🚨 CRITICAL air quality!", mq5Filtered, dhtOk, t, h), ALERT_COOLDOWN_MS);
  } else if (level == AirQuality::Level::ATENCAO) {
    TelegramNotifier::sendAlert("ATENCAO",
      formatMessage("⚠️ Air quality warning", mq5Filtered, dhtOk, t, h), ALERT_COOLDOWN_MS);
  } else if (lastLevel != AirQuality::Level::BOM) {
    TelegramNotifier::resetCooldown();
    TelegramNotifier::sendMessage(
      formatMessage("✅ Air quality normalized", mq5Filtered, dhtOk, t, h));
  }
  lastLevel = level;
}