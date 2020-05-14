#pragma once

#include "utils.h"

class MemoryEntry {
public:
  MemoryEntry(
      const EntryID &id,
      const uint64_t &touchStamp,
      FeatureVector featureVector,
      const double &value,
      const uint64_t &numVisits
  );

  EntryID id;

  uint64_t touchStamp;

  FeatureVector featureVector;
  double value;
  uint64_t numVisits;

  [[nodiscard]] std::string ToString() const;
};

//typedef std::unique_ptr<MemoryEntry> MemoryEntryPtr;
