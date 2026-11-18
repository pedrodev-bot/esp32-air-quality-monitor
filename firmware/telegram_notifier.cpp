#include "telegram_notifier.h"
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>
#include "secrets.h"

namespace TelegramNotifier {
    WiFiClientSecure secured_client;
    UniversalTelegramBot bot(TELEGRAM_BOT_TOKEN, secured_client);
    
    unsigned long lastAlertTime = 0;
    bool alertActive = false;

    void sendMessage(const String& message) {
        secured_client.setInsecure(); // Required for Telegram API without certificate
        bot.sendMessage(TELEGRAM_CHAT_ID, message, "");
    }

    void sendAlert(const char* level, const String& message, unsigned long cooldown) {
        unsigned long currentMillis = millis();
        
        // Fire only if no active alert or cooldown has expired
        if (!alertActive || (currentMillis - lastAlertTime >= cooldown)) {
            lastAlertTime = currentMillis;
            alertActive = true;
            
            secured_client.setInsecure();
            bot.sendMessage(TELEGRAM_CHAT_ID, message, "");
        }
    }

    void resetCooldown() {
        alertActive = false;
        lastAlertTime = 0;
    }
}