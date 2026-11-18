// config.h -> parametros NAO sensiveis (vai pro git)
#pragma once

#define FW_VERSION                "0.1.0"

// 1 = rodando no simulador Wokwi | 0 = hardware real
#define SIMULACAO_WOKWI           1

// ---------- Pinos ----------
#define PIN_DHT                   4     // DATA do DHT
#define PIN_MQ5                   34    // AO do MQ-5 (ADC1, so entrada; ADC2 conflita com Wi-Fi)

// ---------- DHT ----------
// O Wokwi so tem o DHT22 (mesma biblioteca/protocolo, decodificacao diferente do DHT11).
// Em hardware real com DHT11, mude SIMULACAO_WOKWI para 0.
#if SIMULACAO_WOKWI
  #define DHT_MODEL               DHTesp::DHT22
#else
  #define DHT_MODEL               DHTesp::DHT11
#endif

// ---------- MQ-5 ----------
#define MQ5_SAMPLES               16    // media de N amostras por leitura (filtro simples)
#if SIMULACAO_WOKWI
  #define MQ5_WARMUP_MS           0UL   // simulador responde na hora
#else
  #define MQ5_WARMUP_MS           60000UL  // sensor real precisa pre-aquecer
#endif

// ---------- Rede ----------
#define WIFI_CONNECT_TIMEOUT_MS   15000UL
#define WIFI_RETRY_INTERVAL_MS    10000UL

// ---------- Ciclo de leitura / alertas ----------
#define SENSOR_READ_INTERVAL_MS   5000UL
#define ALERT_COOLDOWN_MS         300000UL   // 5 min entre alertas do mesmo nivel

// Limiares PROVISORIOS em valor bruto do ADC (0-4095).
// Serao calibrados na Sprint 6 (Aula 6).
#define MQ5_THRESHOLD_ATENCAO     1200
#define MQ5_THRESHOLD_CRITICO     2400

// ---------- OLED Display ----------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_I2C_ADDRESS 0x3C // Default I2C address for most 0.96" OLEDs