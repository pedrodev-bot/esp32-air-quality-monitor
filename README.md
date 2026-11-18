# IPE: Sistema de Detecção da Qualidade do Ar

Firmware ESP32 (MQ-5 + DHT) com alertas via Telegram. Desenvolvido e testado no Wokwi.

## 1. Bot do Telegram
1. @BotFather → `/newbot` → guarde o **token**.
2. Abra seu bot e envie `/start`.
3. Abra `https://api.telegram.org/bot<TOKEN>/getUpdates` e copie o `chat.id`.
4. Teste: `https://api.telegram.org/bot<TOKEN>/sendMessage?chat_id=<ID>&text=teste`
5. Token vazado (print, chat)? `/revoke` no BotFather.

## 2. Segredos (local)
    cp firmware/qualidade_ar/secrets.example.h firmware/qualidade_ar/secrets.h
Preencha token e chat_id (o Wi-Fi do Wokwi é gerado pelo script). `secrets.h` é ignorado pelo git.

## 3. Rodar no Wokwi
    bash tools/gerar_wokwi.sh
Gera `build_wokwi/` (estrutura plana, `sketch.ino`, `diagram.json`, `libraries.txt`, `secrets.h` com Wokwi-GUEST).
No Wokwi: crie um projeto ESP32 e copie o conteúdo de cada arquivo (ou use o upload de arquivos, se disponível).

## 4. Hardware real
Em `config.h`: `SIMULACAO_WOKWI 0` (usa DHT11 e aquecimento de 60 s do MQ-5).
Instale a biblioteca **DHT sensor library for ESPx**. O MQ-5 é alimentado com 5 V, então o AO precisa de divisor
de tensão antes do GPIO34 (o ADC do ESP32 suporta no máximo 3,3 V).

## Estrutura
    firmware/qualidade_ar/   código (qualidade_ar.ino + módulos .h/.cpp)
    wokwi/                   diagram.json e libraries.txt
    tools/gerar_wokwi.sh     exporta o projeto no formato plano do Wokwi
    CHANGELOG.md
