## [0.7.0] - 2026-11-12
### Changed
- Migração temporária do broker MQTT (de IP Local/Home Assistant para `io.adafruit.com`) via `mqtt_manager.h` para validação de PoC.
- Implementada rotina de autenticação com `AIO_USERNAME` e `AIO_KEY` no `mqttClient.connect()`.
- Ajuste dinâmico das strings de tópicos MQTT para suportar a arquitetura de Feeds do Adafruit IO (`username/feeds/...`).