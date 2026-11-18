## [0.6.0] - 2026-11-12
### Added
- Módulo `openweather_manager.h` para coleta de dados macro-ambientais (Air Pollution API).
- Parsing de JSON assíncrono utilizando `ArduinoJson` para capturar o AQI regional a cada 30 minutos sem bloquear a leitura do MQ-5.
- Novas variáveis em `secrets.h` (`OPENWEATHER_API_KEY`, `LATITUDE`, `LONGITUDE`).