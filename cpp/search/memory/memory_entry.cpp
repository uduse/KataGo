//
// Created by Zeyi Wang on 2020/5/4.
//

#include "memory_entry.h"

MemoryEntry::MemoryEntry(
    const EntryID &id,
    const uint64_t &touchStamp,
    FeatureVector featureVector,
    const double &value,
    const uint64_t &numVisits
)
    : id{id},
      featureVector{std::move(featureVector)},
      touchStamp{touchStamp},
      value{value},
      numVisits{numVisits} {}

std::string MemoryEntry::ToString() const {
  return "Entry #" + std::to_string(id) + " (" + std::to_string(touchStamp)
      + "): " + utils::toString(featureVector);
}

