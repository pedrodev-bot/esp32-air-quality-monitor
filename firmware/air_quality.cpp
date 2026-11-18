#include "air_quality.h"
#include "config.h"

namespace AirQuality {

Level classify(int mq5Raw) {
  if (mq5Raw >= MQ5_THRESHOLD_CRITICO) return Level::CRITICO;
  if (mq5Raw >= MQ5_THRESHOLD_ATENCAO) return Level::ATENCAO;
  return Level::BOM;
}

const char* name(Level level) {
  switch (level) {
    case Level::CRITICO: return "CRITICO";
    case Level::ATENCAO: return "ATENCAO";
    default:             return "BOM";
  }
}

}  // namespace AirQuality
