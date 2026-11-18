#ifndef TELEGRAM_NOTIFIER_H
#define TELEGRAM_NOTIFIER_H

#include <Arduino.h>

namespace TelegramNotifier {
    void sendMessage(const String& message);
    void sendAlert(const char* level, const String& message, unsigned long cooldown);
    void resetCooldown();
}

#endif