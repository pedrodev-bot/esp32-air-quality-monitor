// Sistema de Detecção da Qualidade do Ar - firmware ESP32
// v0.1.0 - scaffolding: Wi-Fi + notificações Telegram
#include "config.h"
#include "wifi_manager.h"
#include "telegram_notifier.h"

static unsigned long lastRead = 0;

// Placeholder: substituir pela leitura real do sensor (Sprint 5/6)
static int readSensorRaw() {
  return random(0, 300);
}

void setup() {
  Serial.begin(115200);
  delay(500);

  if (WifiManager::begin()) {
    TelegramNotifier::sendMessage("✅ Sensor de qualidade do ar online.");
  }
}

void loop() {
  WifiManager::ensureConnected();

  if (millis() - lastRead >= SENSOR_READ_INTERVAL_MS) {
    lastRead = millis();
    int value = readSensorRaw();
    Serial.printf("[SENSOR] leitura = %d\n", value);

    if (value >= THRESHOLD_CRITICO) {
      TelegramNotifier::sendAlert("CRITICO",
        "🚨 Qualidade do ar CRÍTICA! Valor: " + String(value), ALERT_COOLDOWN_MS);
    } else if (value >= THRESHOLD_ATENCAO) {
      TelegramNotifier::sendAlert("ATENCAO",
        "⚠️ Qualidade do ar em atenção. Valor: " + String(value), ALERT_COOLDOWN_MS);
    }
  }
}
