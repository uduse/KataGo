#pragma once

#include "utils.h"

class MemoryEntry {
 public:
  MemoryEntry(const EntryID &id, FeatureVector vec, const uint64_t &touch_stamp);

  EntryID id_;
  uint64_t touch_stamp_;
  FeatureVector vec_;

  [[nodiscard]] std::string ToString() const;
};
