//
// Created by Zeyi Wang on 2020/5/4.
//

#pragma once

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <unordered_map>

#include "utils.h"
#include "core/hash.h"
#include "core/logger.h"
#include "memory_entry.h"
#include "aggregator.h"

using namespace boost::multi_index;

typedef multi_index_container<
    MemoryEntry,
    indexed_by<
        // index 0, unique by id, used to constant time lookup
        hashed_unique<member<MemoryEntry, Hash128, &MemoryEntry::hash>>,

        // index 1, ordered by touch stamp, used to kick outdated entries
        ordered_non_unique<member<MemoryEntry, uint64_t, &MemoryEntry::touchStamp>, std::less<>>
    >
> EntryContainer;

class Memory {
public:
  Memory(
      const uint64_t &feature_dim,
      const uint64_t &memory_size,
      const uint64_t &num_neighbors,
      std::unique_ptr<Aggregator> &aggregator_ptr
  );

  void update(const Hash128 &hash, const FeatureVector &featureVector, const double &value,
              const uint64_t &numVisits, Logger *logger = nullptr);
  std::pair<double, int> query(const FeatureVector &featureVector, Logger *logger = nullptr);

  bool isFull() const;
  bool hasHash(const Hash128 &hash);

  [[nodiscard]] size_t size() const;
  [[nodiscard]] std::string toString() const;

  const uint64_t getFeatureDim() const;

private:
  const uint64_t featureDim;
  const uint64_t memorySize;
  const uint64_t numNeighbors;
  std::unique_ptr<Aggregator> aggregatorPtr;

  EntryContainer entries;
  uint64_t touchCounter;

  void touchEntriesByHashes(const std::vector<Hash128> &NNHashes);
};


