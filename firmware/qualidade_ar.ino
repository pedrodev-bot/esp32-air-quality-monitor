// ESP32 Air Quality Detection System firmware
// v0.3.0 - MQ-5 + DHT + Telegram alerts
#include "config.h"
#include "wifi_manager.h"
#include "telegram_notifier.h"
#include "mq5_sensor.h"
#include "dht_sensor.h"
#include "air_quality.h"

static unsigned long lastRead = 0;
static AirQuality::Level lastLevel = AirQuality::Level::BOM;

// Format Telegram messages with sensor data
static String formatMessage(const char* title, int mq5Raw, bool dhtOk, float t, float h) {
  String m = String(title) + "\nMQ-5 (raw): " + String(mq5Raw);
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

  // Initialize sensors
  MQ5::begin();
  DhtSensor::begin();

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

  // Read sensors
  int mq5Raw = MQ5::readRaw();
  float t = NAN, h = NAN;
  bool dhtOk = DhtSensor::read(t, h);

  // Classify air quality level
  AirQuality::Level level = AirQuality::classify(mq5Raw);
  Serial.printf("[SENSOR] MQ5=%d | T=%.1fC | UR=%.0f%% | level=%s\n",
                mq5Raw, dhtOk ? t : NAN, dhtOk ? h : NAN, AirQuality::name(level));

  // Handle alerts based on quality level
  if (level == AirQuality::Level::CRITICO) {
    TelegramNotifier::sendAlert("CRITICO",
      formatMessage("🚨 CRITICAL air quality!", mq5Raw, dhtOk, t, h), ALERT_COOLDOWN_MS);
  } else if (level == AirQuality::Level::ATENCAO) {
    TelegramNotifier::sendAlert("ATENCAO",
      formatMessage("⚠️ Air quality warning", mq5Raw, dhtOk, t, h), ALERT_COOLDOWN_MS);
  } else if (lastLevel != AirQuality::Level::BOM) {
    TelegramNotifier::resetCooldown();
    TelegramNotifier::sendMessage(
      formatMessage("✅ Air quality normalized", mq5Raw, dhtOk, t, h));
  }
  lastLevel = level;
}