#pragma once
#include <Arduino.h>

namespace DisplayManager {
  bool begin();
  
  // Updates the display asynchronously (5s temp, 3s hum)
  void update(float temp, float hum);
}