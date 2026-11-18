#ifndef OPENWEATHER_MANAGER_H
#define OPENWEATHER_MANAGER_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "secrets.h" 

// Variables for asynchronous non-blocking execution
unsigned long lastWeatherCheck = 0;
const unsigned long weatherInterval = 1800000; // 30 minutes in milliseconds
int regionalAQI = 0; // Air Quality Index: 1 (Good) to 5 (Very Poor)

// Non-blocking OpenWeather API fetch
void fetchRegionalAQIAsync() {
    if (WiFi.status() == WL_CONNECTED) {
        unsigned long currentMillis = millis();
        
        // Fetch on first boot (lastWeatherCheck == 0) or after interval
        if (currentMillis - lastWeatherCheck >= weatherInterval || lastWeatherCheck == 0) {
            lastWeatherCheck = currentMillis == 0 ? 1 : currentMillis; // Prevent re-triggering on 0
            
            HTTPClient http;
            // Build the URL using coordinates (e.g., Santo Andre/ABC Region)
            String url = "http://api.openweathermap.org/data/2.5/air_pollution?lat=" + String(LATITUDE) + "&lon=" + String(LONGITUDE) + "&appid=" + String(OPENWEATHER_API_KEY);
            
            http.begin(url);
            int httpCode = http.GET();
            
            if (httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                
                // Parse JSON payload dynamically
                StaticJsonDocument<512> doc;
                DeserializationError error = deserializeJson(doc, payload);
                
                if (!error) {
                    // Extract the AQI value from the OpenWeather JSON structure
                    regionalAQI = doc["list"][0]["main"]["aqi"]; 
                    Serial.println("[OpenWeather] Regional AQI updated: " + String(regionalAQI));
                } else {
                    Serial.println("[OpenWeather] JSON parsing failed");
                }
            } else {
                Serial.println("[OpenWeather] HTTP GET failed, error: " + http.errorToString(httpCode));
            }
            http.end();
        }
    }
}

// Getter for the current regional AQI
int getRegionalAQI() {
    return regionalAQI;
}

#endif