#include "display_manager.h"
#include "config.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

static unsigned long lastSwitchTime = 0;
static bool showingTemp = true;

namespace DisplayManager {

bool begin() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
    Serial.println("[OLED] Allocation failed");
    return false;
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Starting IPE...");
  display.display();
  
  lastSwitchTime = millis();
  return true;
}

void update(float temp, float hum) {
  unsigned long currentMillis = millis();
  
  // Determine interval based on current state (5000ms for Temp, 3000ms for Hum)
  unsigned long interval = showingTemp ? 5000 : 3000;
  
  // Switch state if interval has passed
  if (currentMillis - lastSwitchTime >= interval) {
    showingTemp = !showingTemp;
    lastSwitchTime = currentMillis;
  }

  // Draw current state to display
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 20);

  if (showingTemp) {
    if (isnan(temp)) {
      display.println("Temp: ERR");
    } else {
      display.print("T: ");
      display.print(temp, 1);
      display.println(" C");
    }
  } else {
    if (isnan(hum)) {
      display.println("Hum: ERR");
    } else {
      display.print("U: ");
      display.print(hum, 0);
      display.println(" %");
    }
  }
  
  display.display();
}

} // namespace DisplayManager