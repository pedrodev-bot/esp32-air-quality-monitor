#pragma once
#include <Arduino.h>

namespace MQ5 {
  void begin();
  int  readRaw();        // media de MQ5_SAMPLES leituras do ADC (0-4095)
  bool isWarmedUp();     // false enquanto o aquecimento nao terminou
}
