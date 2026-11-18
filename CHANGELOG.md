## [0.5.0] - 2026-11-12
### Added
- Módulo `mqtt_manager.h` para integração IoT via MQTT com o Home Assistant.
- Envio periódico (10s) de temperatura, umidade e níveis do gás MQ-5 para tópicos customizados.
- Refatoração do módulo `telegram_notifier.h` para operação totalmente não-bloqueante baseada em `millis()`.
- Sistema de cooldown de 60 segundos para evitar flood de mensagens no Telegram em caso de picos de gás.

### Changed
- Evolução do `qualidade_ar.ino` para orquestrar simultaneamente conectividade (Wi-Fi/MQTT) e requisições HTTPS do Telegram sem travar as interrupções de hardware e UI (OLED).