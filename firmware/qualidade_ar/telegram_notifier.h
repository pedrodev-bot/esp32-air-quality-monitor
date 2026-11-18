#pragma once
#include <Arduino.h>

namespace TelegramNotifier {
  // Envia imediatamente. Retorna true se a API respondeu 200.
  bool sendMessage(const String& text);

  // Envia respeitando cooldown por "chave" (ex.: "CRITICO").
  // Chave diferente da ultima enviada passa direto.
  bool sendAlert(const String& key, const String& text, unsigned long cooldownMs);

  // Zera o cooldown (usar quando a situacao normaliza).
  void resetCooldown();
}
