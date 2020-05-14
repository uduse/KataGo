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
        // index 0, unique by id_, used to constant time lookup
        hashed_unique<member<MemoryEntry, EntryID, &MemoryEntry::id_>>,

        // index 1, ordered by touch stamp, used to kick outdated entries
        ordered_non_unique<member<MemoryEntry, uint64_t, &MemoryEntry::touch_stamp_>, std::less<>>
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

  void Update(const EntryID &id, const FeatureVector &vec);
  FeatureVector Query(const FeatureVector &target);
  void Build();
  void TouchEntriesByIDs(const vector<EntryID> &nn_ids);

  [[nodiscard]] std::vector<FeatureVector> GetFeatureVectors(const std::vector<EntryID> &ids) const;
  [[nodiscard]] std::string ToString() const;

 private:
  const uint64_t feature_dim_;
  const uint64_t memory_size_;
  const uint64_t num_trees_;
  const uint64_t num_neighbors_;

  EntryContainer entries_;
  std::unique_ptr<Aggregator> aggregator_ptr_;
  uint64_t touch_counter_;
  AnnoyPtr annoy_ptr_;
  bool annoy_outdated_;
};


