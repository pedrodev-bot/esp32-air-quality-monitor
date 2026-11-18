// Sistema de Deteccao da Qualidade do Ar - firmware ESP32
// v0.1.0 - MQ-5 + DHT + alertas Telegram
#include "config.h"
#include "wifi_manager.h"
#include "telegram_notifier.h"
#include "mq5_sensor.h"
#include "dht_sensor.h"
#include "air_quality.h"

static unsigned long lastRead = 0;
static AirQuality::Level lastLevel = AirQuality::Level::BOM;

static String formatMessage(const char* title, int mq5Raw, bool dhtOk, float t, float h) {
  String m = String(title) + "\nMQ-5 (bruto): " + String(mq5Raw);
  if (dhtOk) {
    m += "\nTemperatura: " + String(t, 1) + " °C";
    m += "\nUmidade: " + String(h, 0) + " %";
  } else {
    m += "\nTemperatura/umidade: indisponivel";
  }
  return m;
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.printf("\n=== Qualidade do Ar v%s ===\n", FW_VERSION);

  MQ5::begin();
  DhtSensor::begin();

  if (WifiManager::begin()) {
    TelegramNotifier::sendMessage(String("✅ Sensor de qualidade do ar online (v") + FW_VERSION + ")");
  }
}

void loop() {
  WifiManager::ensureConnected();

  if (millis() - lastRead < SENSOR_READ_INTERVAL_MS) return;
  lastRead = millis();

  if (!MQ5::isWarmedUp()) {
    Serial.println("[MQ5] aquecendo sensor...");
    return;
  }

  int mq5Raw = MQ5::readRaw();
  float t = NAN, h = NAN;
  bool dhtOk = DhtSensor::read(t, h);

  AirQuality::Level level = AirQuality::classify(mq5Raw);
  Serial.printf("[SENSOR] MQ5=%d | T=%.1fC | UR=%.0f%% | nivel=%s\n",
                mq5Raw, dhtOk ? t : NAN, dhtOk ? h : NAN, AirQuality::name(level));

  if (level == AirQuality::Level::CRITICO) {
    TelegramNotifier::sendAlert("CRITICO",
      formatMessage("🚨 Qualidade do ar CRÍTICA!", mq5Raw, dhtOk, t, h), ALERT_COOLDOWN_MS);
  } else if (level == AirQuality::Level::ATENCAO) {
    TelegramNotifier::sendAlert("ATENCAO",
      formatMessage("⚠️ Qualidade do ar em atenção", mq5Raw, dhtOk, t, h), ALERT_COOLDOWN_MS);
  } else if (lastLevel != AirQuality::Level::BOM) {
    TelegramNotifier::resetCooldown();
    TelegramNotifier::sendMessage(
      formatMessage("✅ Qualidade do ar normalizada", mq5Raw, dhtOk, t, h));
  }
  lastLevel = level;
}
