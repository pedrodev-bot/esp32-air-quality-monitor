#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <PubSubClient.h>
#include <WiFi.h>
#include "secrets.h" 

// Adafruit IO MQTT Broker configuration
const char* mqtt_server = "io.adafruit.com";
const int mqtt_port = 1883;

// MQTT Topics (Adafruit IO requires the username/feeds/feed-name format)
String topic_temp = String(AIO_USERNAME) + "/feeds/temperature";
String topic_hum  = String(AIO_USERNAME) + "/feeds/humidity";
String topic_gas  = String(AIO_USERNAME) + "/feeds/mq5-gas";
String topic_aqi  = String(AIO_USERNAME) + "/feeds/aqi-regional";

WiFiClient espClient;
PubSubClient mqttClient(espClient);

unsigned long lastMqttPublish = 0;
const unsigned long mqttInterval = 10000; // Publish every 10 seconds

// Initializes MQTT connection
void setupMQTT() {
    mqttClient.setServer(mqtt_server, mqtt_port);
}

// Non-blocking MQTT reconnect function with Adafruit IO Authentication
void handleMQTTConnection() {
    if (!mqttClient.connected()) {
        // Connect using Adafruit IO Username and Key
        if (mqttClient.connect("ESP32_AirQualityClient", AIO_USERNAME, AIO_KEY)) {
            Serial.println("MQTT Connected to Adafruit IO");
        }
    }
    mqttClient.loop();
}

// Asynchronously publish sensor data
void publishSensorData(float temp, float hum, int gasLevel, int aqi) {
    if (mqttClient.connected()) {
        unsigned long currentMillis = millis();
        if (currentMillis - lastMqttPublish >= mqttInterval) {
            lastMqttPublish = currentMillis;
            
            // Publish data to Adafruit IO feeds (feeds are auto-created if they don't exist)
            mqttClient.publish(topic_temp.c_str(), String(temp).c_str());
            mqttClient.publish(topic_hum.c_str(), String(hum).c_str());
            mqttClient.publish(topic_gas.c_str(), String(gasLevel).c_str());
            
            if (aqi > 0) {
                mqttClient.publish(topic_aqi.c_str(), String(aqi).c_str());
            }
        }
    }
}

#endif