#!/usr/bin/env bash
# Gera build_wokwi/ com a estrutura PLANA que o Wokwi espera.
# Uso (Git Bash, na raiz do repo):  bash tools/gerar_wokwi.sh
set -e
cd "$(dirname "$0")/.."

SRC=firmware/qualidade_ar
OUT=build_wokwi
rm -rf "$OUT"; mkdir "$OUT"

cp "$SRC"/*.h "$SRC"/*.cpp "$OUT"/
rm -f "$OUT"/secrets.example.h "$OUT"/secrets.h
cp "$SRC/qualidade_ar.ino" "$OUT/sketch.ino"
cp wokwi/diagram.json wokwi/libraries.txt "$OUT"/

# secrets do Wokwi: usa seu secrets.h local (token/chat_id) e troca o Wi-Fi pela rede virtual
SEC="$SRC/secrets.h"; [ -f "$SEC" ] || SEC="$SRC/secrets.example.h"
sed -e 's|^#define WIFI_SSID .*|#define WIFI_SSID           "Wokwi-GUEST"|' \
    -e 's|^#define WIFI_PASSWORD .*|#define WIFI_PASSWORD       ""|' \
    "$SEC" > "$OUT/secrets.h"

echo "Pronto: $OUT/ ($(ls "$OUT" | wc -l) arquivos). Origem dos segredos: $SEC"
