## [0.8.0] - 2026-11-12
### Added
- Módulo `hardware_alerts.h` para gestão da interface de utilizador (HMI) física.
- Suporte para Buzzer Ativo com chamadas não-bloqueantes (`millis()`) para bipes de transição e alarmes críticos intermitentes.
- Suporte para LED RGB (configurável para Anodo ou Catodo Comum via flag `COMMON_ANODE`) indicando status da rede/aquecimento (fade azul) e qualidade do ar (verde, amarelo, vermelho piscante).