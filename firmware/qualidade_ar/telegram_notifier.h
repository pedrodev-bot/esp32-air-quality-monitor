#pragma once
#include <Arduino.h>

namespace TelegramNotifier {
  // Envia mensagem imediatamente. Retorna true se a API respondeu 200.
  bool sendMessage(const String& text);

  // Envia respeitando um cooldown por "chave" de alerta (ex.: "CRITICO").
  // Retorna true somente se a mensagem foi de fato enviada.
  bool sendAlert(const String& key, const String& text, unsigned long cooldownMs);
}
