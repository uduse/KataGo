#pragma once

#include "utils.h"
#include "core/hash.h"

class MemoryEntry {
public:
  MemoryEntry(
      const Hash128 &hash,
      const uint64_t &touchStamp,
      FeatureVector featureVector,
      const double &value,
      const double &numVisits
  );

  Hash128 hash;
  uint64_t touchStamp;

  FeatureVector featureVector;
  double value;
  double numVisits;

  [[nodiscard]] std::string toString() const;
};

//typedef std::unique_ptr<MemoryEntry> MemoryEntryPtr;
