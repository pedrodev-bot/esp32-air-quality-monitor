#include "mq5_sensor.h"
#include "config.h"

namespace MQ5 {

static unsigned long bootTime = 0;

void begin() {
  analogReadResolution(12);
  analogSetPinAttenuation(PIN_MQ5, ADC_11db);   // faixa ~0-3,3 V
  pinMode(PIN_MQ5, INPUT);
  bootTime = millis();
}

int readRaw() {
  long sum = 0;
  for (int i = 0; i < MQ5_SAMPLES; i++) {
    sum += analogRead(PIN_MQ5);
    delayMicroseconds(200);
  }
  return (int)(sum / MQ5_SAMPLES);
}

bool isWarmedUp() { return millis() - bootTime >= MQ5_WARMUP_MS; }

}  // namespace MQ5
