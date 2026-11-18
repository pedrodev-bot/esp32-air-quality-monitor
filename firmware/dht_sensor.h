#pragma once
#include <Arduino.h>

namespace DhtSensor {
  void begin();
  // Retorna true se a leitura e valida. Em caso de erro, t e h nao sao alterados.
  bool read(float& temperatureC, float& humidityPct);
}
