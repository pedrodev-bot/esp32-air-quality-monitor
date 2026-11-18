#ifndef HARDWARE_ALERTS_H
#define HARDWARE_ALERTS_H

#include <Arduino.h>
#include "air_quality.h"

// ================= PINS CONFIGURATION =================
const int PIN_BUZZER = 15; // Ajuste para o pino do seu Buzzer
const int PIN_LED_R  = 25; // Ajuste para o pino Vermelho
const int PIN_LED_G  = 26; // Ajuste para o pino Verde
const int PIN_LED_B  = 27; // Ajuste para o pino Azul

// ================= LED TYPE CONFIGURATION =============
// Se o LED acender com as cores trocadas ou apagar quando devia acender,
// mude esta variável para true (significa que ele é Anodo Comum)
const bool COMMON_ANODE = false; 

// ================= INTERNAL VARIABLES =================
unsigned long lastBuzzerToggle = 0;
unsigned long buzzerStateStart = 0;
bool buzzerIsOn = false;
int buzzerBeepsRemaining = 0;
int buzzerIntervalOn = 0;
int buzzerIntervalOff = 0;

unsigned long lastLedFade = 0;
int ledFadeValue = 0;
int ledFadeDirection = 5;

AirQuality::Level prevHwLevel = AirQuality::Level::BOM;

// Helper function to set LED color
void setRGB(int r, int g, int b) {
    if (COMMON_ANODE) {
        r = 255 - r;
        g = 255 - g;
        b = 255 - b;
    }
    analogWrite(PIN_LED_R, r);
    analogWrite(PIN_LED_G, g);
    analogWrite(PIN_LED_B, b);
}

// Setup pins
void setupHardwareAlerts() {
    pinMode(PIN_BUZZER, OUTPUT);
    digitalWrite(PIN_BUZZER, LOW);
    
    // PWM configuration for LED pins is handled automatically by analogWrite on ESP32 (Arduino core 3.x)
    setRGB(0, 0, 0); // Desliga tudo no início
}

// Configures an asynchronous beep sequence
void triggerBeep(int beeps, int onTime, int offTime) {
    buzzerBeepsRemaining = beeps * 2; // Each beep has an ON and OFF state
    buzzerIntervalOn = onTime;
    buzzerIntervalOff = offTime;
    buzzerStateStart = millis();
    buzzerIsOn = true;
    digitalWrite(PIN_BUZZER, HIGH);
}

// Main handler to be called inside loop()
void handleHardwareAlerts(AirQuality::Level currentLevel, bool isWarmingUp) {
    unsigned long currentMillis = millis();

    // -----------------------------------------------------
    // 1. LED LOGIC
    // -----------------------------------------------------
    if (isWarmingUp) {
        // Pulsing Blue while warming up
        if (currentMillis - lastLedFade >= 30) {
            lastLedFade = currentMillis;
            ledFadeValue += ledFadeDirection;
            if (ledFadeValue <= 0 || ledFadeValue >= 255) {
                ledFadeDirection = -ledFadeDirection;
            }
            setRGB(0, 0, ledFadeValue);
        }
    } else {
        // Fixed colors based on Air Quality
        if (currentLevel == AirQuality::Level::BOM) {
            setRGB(0, 255, 0); // Green
        } else if (currentLevel == AirQuality::Level::ATENCAO) {
            setRGB(255, 128, 0); // Yellow/Orange
        } else if (currentLevel == AirQuality::Level::CRITICO) {
            // Blinking Red
            if ((currentMillis / 250) % 2 == 0) {
                setRGB(255, 0, 0);
            } else {
                setRGB(0, 0, 0);
            }
        }
    }

    // -----------------------------------------------------
    // 2. BUZZER LOGIC (STATE TRANSITIONS)
    // -----------------------------------------------------
    if (currentLevel != prevHwLevel && !isWarmingUp) {
        if (currentLevel == AirQuality::Level::ATENCAO && prevHwLevel == AirQuality::Level::BOM) {
            triggerBeep(2, 100, 100); // 2 short beeps
        } else if (currentLevel == AirQuality::Level::BOM && prevHwLevel != AirQuality::Level::BOM) {
            triggerBeep(1, 1000, 0); // 1 long beep
        }
        prevHwLevel = currentLevel;
    }

    // -----------------------------------------------------
    // 3. BUZZER EXECUTION (NON-BLOCKING)
    // -----------------------------------------------------
    if (currentLevel == AirQuality::Level::CRITICO && !isWarmingUp) {
        // Continuous emergency pulse
        if (currentMillis - lastBuzzerToggle >= 500) {
            lastBuzzerToggle = currentMillis;
            buzzerIsOn = !buzzerIsOn;
            digitalWrite(PIN_BUZZER, buzzerIsOn ? HIGH : LOW);
        }
    } else if (buzzerBeepsRemaining > 0) {
        // Execute finite beep sequences
        int currentInterval = buzzerIsOn ? buzzerIntervalOn : buzzerIntervalOff;
        if (currentMillis - buzzerStateStart >= currentInterval) {
            buzzerStateStart = currentMillis;
            buzzerBeepsRemaining--;
            
            if (buzzerBeepsRemaining > 0) {
                buzzerIsOn = !buzzerIsOn;
                digitalWrite(PIN_BUZZER, buzzerIsOn ? HIGH : LOW);
            } else {
                buzzerIsOn = false;
                digitalWrite(PIN_BUZZER, LOW);
            }
        }
    } else {
        // Ensure buzzer is off if not critical and no beeps remaining
        digitalWrite(PIN_BUZZER, LOW);
    }
}

#endif