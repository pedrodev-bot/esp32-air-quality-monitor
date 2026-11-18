#pragma once
#include <Arduino.h>

namespace WifiManager {
  bool begin();             // conexão inicial (bloqueante com timeout)
  void ensureConnected();   // chamar no loop(): reconecta sem bloquear
  bool isConnected();
}
