#pragma once

#include "utils.h"

class MemoryEntry {
 public:
  MemoryEntry(const unsigned int &id, FeatureVector vec, const unsigned int &touch_stamp);

  unsigned int id_;
  unsigned int touch_stamp_;
  FeatureVector vec_;

  [[nodiscard]] std::string ToString() const;
};
