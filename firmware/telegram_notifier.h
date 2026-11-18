#ifndef TELEGRAM_NOTIFIER_H
#define TELEGRAM_NOTIFIER_H

#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>
#include "secrets.h" 

WiFiClientSecure secured_client;
UniversalTelegramBot bot(TELEGRAM_BOT_TOKEN, secured_client);

unsigned long lastTelegramCheck = 0;
const unsigned long telegramCooldown = 60000; 
bool alertSent = false;

// Configures secure client for Telegram API
void setupTelegram() {
    secured_client.setInsecure(); 
}

// Non-blocking alert dispatcher
void handleAirQualityAlerts(String qualityStatus, float gasValue) {
    unsigned long currentMillis = millis();
    
    // Only send if status is critical and cooldown has passed
    if (qualityStatus == "CRITICO" && (currentMillis - lastTelegramCheck >= telegramCooldown || !alertSent)) {
        lastTelegramCheck = currentMillis;
        alertSent = true;
        
        String message = "🚨 AIR QUALITY CRITICAL 🚨\n";
        message += "Gas Level: " + String(gasValue) + "\n";
        message += "Please check the environment immediately!";
        
        // Fire and forget - prevents loop stalling
        bot.sendMessage(TELEGRAM_CHAT_ID, message, "");
    } 
    // Reset alert flag when air quality normalizes
    else if (qualityStatus == "BOM" && alertSent) {
        bot.sendMessage(TELEGRAM_CHAT_ID, "✅ Air quality has normalized.", "");
        alertSent = false;
    }
}

#endif