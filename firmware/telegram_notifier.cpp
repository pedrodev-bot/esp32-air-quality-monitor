#include "telegram_notifier.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "secrets.h"

namespace TelegramNotifier {

static String lastKey = "";
static unsigned long lastSent = 0;

static String urlEncode(const String& s) {
  String out;
  const char* hex = "0123456789ABCDEF";
  for (size_t i = 0; i < s.length(); i++) {
    uint8_t c = (uint8_t)s[i];
    if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
      out += (char)c;
    } else {
      out += '%';
      out += hex[c >> 4];
      out += hex[c & 0x0F];
    }
  }
  return out;
}

bool sendMessage(const String& text) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[TG] Sem Wi-Fi, mensagem nao enviada");
    return false;
  }

  WiFiClientSecure client;
  // TODO (Sprint 9): trocar por client.setCACert(<cert raiz do Telegram>)
  client.setInsecure();

  HTTPClient http;
  http.setTimeout(8000);
  String url = String("https://api.telegram.org/bot") + TELEGRAM_BOT_TOKEN + "/sendMessage";
  if (!http.begin(client, url)) {
    Serial.println("[TG] Falha em http.begin");
    return false;
  }

  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String body = String("chat_id=") + TELEGRAM_CHAT_ID + "&text=" + urlEncode(text);

  int code = http.POST(body);
  bool ok = (code == 200);
  if (ok) {
    Serial.println("[TG] Mensagem enviada");
  } else {
    // 401 = token errado | 400 = chat_id errado / faltou /start no bot
    Serial.printf("[TG] Erro HTTP %d: %s\n", code, http.getString().c_str());
  }
  http.end();
  return ok;
}

bool sendAlert(const String& key, const String& text, unsigned long cooldownMs) {
  bool sameKey = (key == lastKey);
  if (sameKey && lastSent != 0 && millis() - lastSent < cooldownMs) return false;

  if (sendMessage(text)) {
    lastKey = key;
    lastSent = millis();
    return true;
  }
  return false;
}

void resetCooldown() {
  lastKey = "";
  lastSent = 0;
}

}  // namespace TelegramNotifier
