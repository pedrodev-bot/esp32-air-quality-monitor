#pragma once

namespace AirQuality {
  enum class Level { BOM, ATENCAO, CRITICO };

  Level       classify(int mq5Raw);
  const char* name(Level level);
}
