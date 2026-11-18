#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <PubSubClient.h>
#include <WiFi.h>

// Home Assistant MQTT Broker configuration
const char* mqtt_server = "SEU_IP_DO_HOME_ASSISTANT";
const int mqtt_port = 1883;

// MQTT Topics for Home Assistant sensors
const char* topic_temp = "ipe/sensor/temperature";
const char* topic_hum = "ipe/sensor/humidity";
const char* topic_gas = "ipe/sensor/gas";

WiFiClient espClient;
PubSubClient mqttClient(espClient);

unsigned long lastMqttPublish = 0;
const unsigned long mqttInterval = 10000; // Publish every 10 seconds

// Initializes MQTT connection
void setupMQTT() {
    mqttClient.setServer(mqtt_server, mqtt_port);
}

// Non-blocking MQTT reconnect function
void handleMQTTConnection() {
    if (!mqttClient.connected()) {
        // Attempt to connect without blocking the main loop with while()
        if (mqttClient.connect("ESP32_AirQualityClient")) {
            Serial.println("MQTT Connected to Home Assistant");
        }
    }
    mqttClient.loop();
}

// Asynchronously publish sensor data
void publishSensorData(float temp, float hum, int gasLevel) {
    if (mqttClient.connected()) {
        unsigned long currentMillis = millis();
        if (currentMillis - lastMqttPublish >= mqttInterval) {
            lastMqttPublish = currentMillis;
            
            mqttClient.publish(topic_temp, String(temp).c_str());
            mqttClient.publish(topic_hum, String(hum).c_str());
            mqttClient.publish(topic_gas, String(gasLevel).c_str());
        }
    }
}

#endif