# IPE: Sistema de Detecção da Qualidade do Ar (Firmware ESP32)

Este repositório contém o firmware do microcontrolador ESP32 desenvolvido para o projeto de Inovações para Engenharia (ESMA002). O sistema realiza a leitura contínua e assíncrona de gases poluentes e condições climáticas, exibindo os dados localmente e enviando alertas remotos.

**Versão Atual:** v0.3.2

## 🚀 Funcionalidades (v0.3.2)

*   **Leitura Assíncrona Não-Bloqueante:** Loop principal operando inteiramente via `millis()`, garantindo concorrência entre leitura de sensores, atualização de tela e comunicação de rede.
*   **Monitoramento de Gás (MQ-5):** Leitura de gases com filtro digital de Média Móvel (Moving Average) embutido para estabilização de ruídos e prevenção de falsos positivos.
*   **Clima (DHT11/DHT22):** Leitura integrada de temperatura e umidade relativa do ar.
*   **Interface Local (OLED SSD1306):** Display I2C que alterna automaticamente as informações na tela de forma não-bloqueante (Temperatura visível por 5 segundos, Umidade visível por 3 segundos).
*   **Alertas via Telegram:** Notificações disparadas automaticamente em caso de qualidade do ar "Atenção" ou "Crítica", com período de resfriamento (cooldown) configurável.

## 🛠️ Requisitos de Hardware

*   Placa de Desenvolvimento: **ESP32**
*   Sensor de Qualidade do Ar: **MQ-5** (Analógico)
*   Sensor de Temperatura e Umidade: **DHT11** ou **DHT22** (Digital)
*   Display OLED: **0.96" SSD1306** (Comunicação I2C)

## 📚 Dependências e Bibliotecas

Para compilar este código no Arduino IDE ou PlatformIO, instale as seguintes bibliotecas:
*   `Adafruit GFX Library` (Para o display OLED)
*   `Adafruit SSD1306` (Para o display OLED)
*   `DHT sensor library` by Adafruit (Para os sensores de temperatura/umidade)
*   `UniversalTelegramBot` ou similar (Utilizada no `telegram_notifier.h`)

## ⚙️ Configuração do Ambiente

1. **Clonar/Baixar** o projeto na pasta correspondente à `v0.3.2`.
2. **Configuração de Rede e API:** Crie um arquivo `secrets.h` na raiz do projeto (não versione este arquivo!) com as credenciais da sua rede e do bot do Telegram:
   ```cpp
   #define WIFI_SSID "NOME_DA_REDE"
   #define WIFI_PASSWORD "SENHA_DA_REDE"
   #define TELEGRAM_BOT_TOKEN "SEU_TOKEN_AQUI"
   #define TELEGRAM_CHAT_ID "SEU_CHAT_ID"
   ```
3. **Pinos e Calibração:** Ajuste os pinos de conexão de hardware no arquivo `config.h` de acordo com a sua montagem física na protoboard.

## 🗂️ Arquitetura do Software

O código foi construído sob uma arquitetura estritamente modular:
*   `qualidade_ar.ino`: Main loop e orquestração assíncrona.
*   `config.h`: Constantes, temporizadores, limites (thresholds) e mapeamento de pinos.
*   `mq5_sensor`: Leitura bruta e controle de aquecimento (warm-up).
*   `dht_sensor`: Leitura de temperatura e umidade.
*   `display_manager`: Controle de IHM (OLED) e timer visual (5s/3s).
*   `air_quality`: Regras de negócios e classificação da qualidade.
*   `telegram_notifier`: Disparo de alertas e gestão de tempo de cooldown.
*   `wifi_manager`: Conexão e auto-reconexão de rede.

---
*Desenvolvedor de Código Geral: Pedro Augusto dos Santos Lopes*