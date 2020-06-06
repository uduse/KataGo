//
// Created by Zeyi Wang on 2020/5/4.
//

#include "memory_entry.h"

MemoryEntry::MemoryEntry(
    const Hash128 &hash,
    const uint64_t &touchStamp,
    FeatureVector featureVector,
    const double &value,
    const double &numVisits
)
    : hash{hash},
      touchStamp{touchStamp},
      featureVector{std::move(featureVector)},
      value{value},
      numVisits{numVisits} {}

std::string MemoryEntry::toString() const {
//  return "Entry #" + std::to_string(hash.hash0) + " (" + std::to_string(touchStamp)
//      + "): " + utils::toString(featureVector);
  return "Entry #" + std::to_string(hash.hash0)
      + " touch: " + std::to_string(touchStamp)
      + " value: " + std::to_string(value)
      + " numVisits: " + std::to_string(numVisits);
}

