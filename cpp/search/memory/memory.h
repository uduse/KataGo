//
// Created by Zeyi Wang on 2020/5/4.
//

#pragma once

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>

#include "utils.h"
#include "memory_entry.h"
#include "aggregator.h"

using namespace boost::multi_index;

typedef multi_index_container<
    MemoryEntry,
    indexed_by<
        // index 0, unique by id, used to constant time lookup
        hashed_unique<member<MemoryEntry, EntryID, &MemoryEntry::id>>,

        // index 1, ordered by touch stamp, used to kick outdated entries
        ordered_non_unique<member<MemoryEntry, uint64_t, &MemoryEntry::touchStamp>, std::less<>>
    >
> EntryContainer;

class Memory {
public:
  Memory(
      const uint64_t &feature_dim,
      const uint64_t &memory_size,
      const uint64_t &num_trees,
      const uint64_t &num_neighbors,
      std::unique_ptr<Aggregator> &aggregator_ptr
  );

  void Update(const EntryID &id, const FeatureVector &featureVector, const double &value, const uint64_t &numVisits);
//  FeatureVector Query(const FeatureVector &target);
  double Query(const FeatureVector &target);
  void Build();
  void TouchEntriesByIDs(const vector<EntryID> &nn_ids);

  [[nodiscard]] const std::vector<std::shared_ptr<MemoryEntry>> GetEntriesByIDs(const std::vector<EntryID> &ids) const;
  [[nodiscard]] std::string ToString() const;

private:
  const uint64_t featureDim;
  const uint64_t memorySize;
  const uint64_t numTrees;
  const uint64_t numNeighbors;

  EntryContainer entries;
  std::unique_ptr<Aggregator> aggregatorPtr;
  uint64_t touchCounter;
  AnnoyPtr annoyPtr;
  bool annoyOutDated;
};


