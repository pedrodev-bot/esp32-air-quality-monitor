#include "wifi_manager.h"
#include <WiFi.h>
#include "secrets.h"
#include "config.h"

namespace WifiManager {

static unsigned long lastRetry = 0;

bool begin() {
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("[WiFi] Conectando");
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < WIFI_CONNECT_TIMEOUT_MS) {
    delay(300);
    Serial.print(".");
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("[WiFi] OK, IP: %s\n", WiFi.localIP().toString().c_str());
    return true;
  }
  Serial.println("[WiFi] Falha na conexao inicial");
  return false;
}

void ensureConnected() {
  if (WiFi.status() == WL_CONNECTED) return;
  if (millis() - lastRetry < WIFI_RETRY_INTERVAL_MS) return;
  lastRetry = millis();
  Serial.println("[WiFi] Reconectando...");
  WiFi.disconnect();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

bool isConnected() { return WiFi.status() == WL_CONNECTED; }

}  // namespace WifiManager
