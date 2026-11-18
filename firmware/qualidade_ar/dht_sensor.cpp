#include "dht_sensor.h"
#include <DHTesp.h>      // lib "DHT sensor library for ESPx"
#include "config.h"

namespace DhtSensor {

static DHTesp dht;

void begin() {
  dht.setup(PIN_DHT, DHT_MODEL);
}

bool read(float& temperatureC, float& humidityPct) {
  TempAndHumidity d = dht.getTempAndHumidity();
  if (dht.getStatus() != DHTesp::ERROR_NONE || isnan(d.temperature) || isnan(d.humidity)) {
    Serial.printf("[DHT] erro de leitura: %s\n", dht.getStatusString());
    return false;
  }
  temperatureC = d.temperature;
  humidityPct  = d.humidity;
  return true;
}

}  // namespace DhtSensor
