//
// Created by Zeyi Wang on 2020/5/4.
//

#include <boost/algorithm/string/join.hpp>
#include "memory.h"

Memory::Memory(
	const uint64_t &feature_dim,
	const uint64_t &memory_size,
	const uint64_t &num_trees,
	const uint64_t &num_neighbors,
	std::unique_ptr<Aggregator> &aggregator_ptr
)
	: featureDim{feature_dim},
	  memorySize{memory_size},
	  numTrees{num_trees},
	  numNeighbors(num_neighbors),
	  annoyPtr{nullptr},
	  annoyOutDated(true),
	  entries{},
	  touchCounter{0},
	  aggregatorPtr{std::move(aggregator_ptr)} {}

void Memory::Update(
	const EntryID &id,
	const FeatureVector &featureVector,
	const double &value,
	const uint64_t &numVisits
) {
  assert(featureVector.size() == featureDim);

  MemoryEntry entry(
	  id,
	  touchCounter++,
	  featureVector,
	  value,
	  numVisits
  );

  auto &index_by_id = entries.get<0>();
  auto found = index_by_id.find(id);
  if (found != index_by_id.end()) {
	index_by_id.replace(found, entry);
  } else {
	index_by_id.insert(entry);
	if (entries.size() > memorySize) {
	  auto &index_by_touch_stamp = entries.get<1>();
	  auto target = index_by_touch_stamp.begin();
	  index_by_touch_stamp.erase(target);
	}
	annoyOutDated = true;
  }
}

double Memory::Query(const FeatureVector &target) {
  if (annoyOutDated) {
	Build();
	annoyOutDated = false;
  }

  std::vector<EntryID> nnIDs;
  std::vector<double> distances;

  annoyPtr->get_nns_by_vector(target.data(), numNeighbors, -1, &nnIDs, &distances);
  const std::vector<std::shared_ptr<MemoryEntry>> &entryPtrs = GetEntriesByIDs(nnIDs);
  auto result = aggregatorPtr->Aggregate(entryPtrs, distances);
  return result;
}

void Memory::TouchEntriesByIDs(const vector<EntryID> &nn_ids) {
  auto &index_by_id = this->entries.get<0>();
  for (auto &id : nn_ids) {
	auto found = index_by_id.find(id);
	auto touch_counter = this->touchCounter++;
	index_by_id.modify(
		found,
		[&touch_counter](auto &entry) {
		  entry.touchStamp = touch_counter;
		}
	);
  }
}

void Memory::Build() {
  assert(entries.size() > 0);
  annoyPtr = std::move(std::make_unique<IndexType>(featureDim));
  auto &index_by_id = entries.get<0>();
  for (auto &entry : index_by_id) {
	annoyPtr->add_item(entry.id, entry.featureVector.data());
  }
  annoyPtr->build(numTrees);
}

const std::vector<std::shared_ptr<MemoryEntry>>
Memory::GetEntriesByIDs(const std::vector<EntryID> &ids) const {
  std::vector<std::shared_ptr<MemoryEntry>> results;

  auto &index_by_id = entries.get<0>();
  for (auto &id : ids) {
	auto candidate = index_by_id.find(id);
	results.push_back(std::make_shared<MemoryEntry>(*candidate));
  }
  return results;
}

std::string Memory::ToString() const {
  std::vector<std::string> strings;
  auto &index_by_id = entries.get<0>();
  for (const auto &entry : index_by_id) {
    strings.push_back(entry.ToString());
  }
  return boost::algorithm::join(strings, "\n");
}
