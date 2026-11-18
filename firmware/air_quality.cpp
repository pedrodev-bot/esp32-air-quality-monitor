#include "air_quality.h"
#include <Arduino.h>

namespace AirQuality {

    Level classify(int gasLevel) {
        // ==================================================
        // MODO DE TESTE HMI (LED e Buzzer)
        // Alterna o estado a cada 5 segundos ignorando o MQ-5
        // ==================================================
        unsigned long tempo = millis() / 5000; 
        int ciclo = tempo % 3;

        if (ciclo == 0) {
            return Level::BOM;      // 0 a 5s: LED Verde (Toca bipe de normalização se estava crítico)
        } 
        else if (ciclo == 1) {
            return Level::ATENCAO;  // 5 a 10s: LED Amarelo e toca Bipe Duplo
        } 
        else {
            return Level::CRITICO;  // 10 a 15s: LED Vermelho Piscante e Sirene intermitente
        }
    }

    const char* name(Level level) {
        switch (level) {
            case Level::BOM: return "BOM";
            case Level::ATENCAO: return "ATENCAO";
            case Level::CRITICO: return "CRITICO";
            default: return "UNKNOWN";
        }
    }
}