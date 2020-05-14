//
// Created by Zeyi Wang on 2020/5/4.
//

#include <boost/algorithm/string/join.hpp>
#include "memory.h"

Memory::Memory(
    const int &feature_dim,
    const int &memory_size,
    const int &num_trees,
    const int &num_neighbors,
    std::unique_ptr<Aggregator> &aggregator_ptr
)
    : feature_dim_{feature_dim},
      memory_size_{memory_size},
      num_trees_{num_trees},
      num_neighbors_(num_neighbors),
      annoy_ptr_{nullptr},
      annoy_outdated_(false),
      entries_{},
      touch_counter_{0},
      aggregator_ptr_{std::move(aggregator_ptr)} {}

void Memory::Update(const unsigned int &id, const FeatureVector &vec) {
  MemoryEntry entry{id, vec, touch_counter_++};
  auto &index_by_id = entries_.get<0>();
  auto found = index_by_id.find(id);
  if (found != index_by_id.end()) {
    index_by_id.replace(found, entry);
  } else {
    index_by_id.insert(entry);
    if (entries_.size() > memory_size_) {
      auto &index_by_touch_stamp = entries_.get<1>();
      auto target = index_by_touch_stamp.begin();
      spdlog::debug("Erasing: {}", target->ToString());
      index_by_touch_stamp.erase(target);
    }
    annoy_outdated_ = true;
  }
}

FeatureVector Memory::Query(const FeatureVector &target) {
  if (annoy_outdated_) {
    Build();
    annoy_outdated_ = false;
  }

  std::vector<int> nn_ids;
  std::vector<double> distances;

  annoy_ptr_->get_nns_by_vector(target.data(), num_neighbors_, -1, &nn_ids, &distances);

  auto vectors = GetFeatureVectors(nn_ids);
  auto result = aggregator_ptr_->Aggregate(vectors, distances);
  TouchEntriesByIDs(nn_ids);

  spdlog::debug("Neighbor IDs: {}", utils::ToString(nn_ids));
  spdlog::debug("Distances: {}", utils::ToString(distances));
  spdlog::debug("Aggregated Result: {}", utils::ToString(result));

  return result;
}

void Memory::TouchEntriesByIDs(const vector<int> &nn_ids) {
  auto &index_by_id = this->entries_.get<0>();
  for (auto &id : nn_ids) {
    auto found = index_by_id.find(id);
    auto touch_counter = this->touch_counter_++;
    index_by_id.modify(
        found,
        [&touch_counter](auto &entry) {
          entry.touch_stamp_ = touch_counter;
        }
    );
  }
}

void Memory::Build() {
  annoy_ptr_ = std::move(std::make_unique<IndexType>(feature_dim_));
  auto &index_by_id = entries_.get<0>();
  for (auto &entry : index_by_id) {
    annoy_ptr_->add_item(entry.id_, entry.vec_.data());
  }
  annoy_ptr_->build(num_trees_);
}

std::vector<FeatureVector> Memory::GetFeatureVectors(const std::vector<int> &ids) const {
  std::vector<FeatureVector> results;

  auto &index_by_id = entries_.get<0>();
  for (auto &id : ids) {
    auto candidate = index_by_id.find(id)->vec_;
    results.push_back(candidate);
  }
  return results;
}

//FeatureVector Memory::AggregateVectors(
//	const std::vector<FeatureVector> &vectors,
//	const std::vector<double> &distances
//) {
//  FeatureVector result((double)0, vectors[0].size());
//
//  return result;
//}

std::string Memory::ToString() const {
  std::vector<std::string> strings;
  auto &index_by_id = entries_.get<0>();
  for (const auto &entry : index_by_id) {
    strings.push_back(entry.ToString());
  }
  return boost::algorithm::join(strings, "\n");
}
