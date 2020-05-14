#pragma once

#include "utils.h"

class MemoryEntry {
 public:
  MemoryEntry(const uint64_t &id, FeatureVector vec, const uint64_t &touch_stamp);

  uint64_t id_;
  uint64_t touch_stamp_;
  FeatureVector vec_;

  [[nodiscard]] std::string ToString() const;
};
