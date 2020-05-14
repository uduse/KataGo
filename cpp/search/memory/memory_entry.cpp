//
// Created by Zeyi Wang on 2020/5/4.
//

#include "memory_entry.h"

MemoryEntry::MemoryEntry(
    const EntryID &id,
    FeatureVector vec,
    const uint64_t &touch_stamp
)
    : id_{id},
      vec_{std::move(vec)},
      touch_stamp_{touch_stamp} {}

std::string MemoryEntry::ToString() const {
  return "Entry #" + std::to_string(id_) + " (" + std::to_string(touch_stamp_)
      + "): " + utils::ToString(vec_);
}

