// config.h -> parâmetros NÃO sensíveis (pode ir pro git)
#pragma once

#define WIFI_CONNECT_TIMEOUT_MS   15000UL
#define WIFI_RETRY_INTERVAL_MS    10000UL

#define SENSOR_READ_INTERVAL_MS   5000UL
#define ALERT_COOLDOWN_MS         300000UL   // 5 min entre alertas iguais (evita spam)

// Limiares provisórios (ajustar na etapa de calibração - Sprint 6)
#define THRESHOLD_ATENCAO         100        // ex.: AQI / ppm do sensor
#define THRESHOLD_CRITICO         200
