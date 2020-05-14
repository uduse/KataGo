//
// Created by Zeyi Wang on 2020/5/4.
//

#pragma once

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "utils.h"
#include "memory_entry.h"
#include "aggregator.h"

using namespace boost::multi_index;

typedef multi_index_container<
    MemoryEntry,
    indexed_by<
        // index 0, unique by id_, used to constant time lookup
        hashed_unique<member<MemoryEntry, unsigned int, &MemoryEntry::id_>>,

        // index 1, ordered by touch stamp, used to kick outdated entries
        ordered_non_unique<member<MemoryEntry, unsigned int, &MemoryEntry::touch_stamp_>, std::less<>>
    >
> EntryContainer;

class Memory {
 public:
  Memory(
      const int &feature_dim,
      const int &memory_size,
      const int &num_trees,
      const int &num_neighbors,
      std::unique_ptr<Aggregator> &aggregator_ptr
  );

  void Update(const unsigned int &id, const FeatureVector &vec);
  FeatureVector Query(const FeatureVector &target);
  void Build();
  void TouchEntriesByIDs(const vector<int> &nn_ids);

  [[nodiscard]] std::vector<FeatureVector> GetFeatureVectors(const std::vector<int> &ids) const;
  [[nodiscard]] std::string ToString() const;

 private:
  const int feature_dim_;
  const int memory_size_;
  const int num_trees_;
  const int num_neighbors_;

  EntryContainer entries_;
  std::unique_ptr<Aggregator> aggregator_ptr_;
  unsigned int touch_counter_;
  AnnoyPtr annoy_ptr_;
  bool annoy_outdated_;
};


