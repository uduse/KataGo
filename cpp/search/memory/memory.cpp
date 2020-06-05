//
// Created by Zeyi Wang on 2020/5/4.
//

#include <boost/algorithm/string/join.hpp>
#include <queue>

#include "memory.h"

Memory::Memory(
    const uint64_t &featureDim,
    const uint64_t &memorySize,
    const uint64_t &numNeighbors,
    std::unique_ptr<Aggregator> &aggregatorPtr
)
    : featureDim_{featureDim},
      memorySize_{memorySize},
      numNeighbors_(numNeighbors),
      aggregatorPtr_{std::move(aggregatorPtr)},
      touchCounter_{0},
      entries_{} {}

void Memory::update(
    const Hash128 &hash,
    const FeatureVector &featureVector,
    const double &value,
    const uint64_t &numVisits,
    Logger *logger
) {
  assert(featureVector.size() == featureDim_);

  if (logger && touchCounter_ % 1000 == 0) {
    logger->write(toString());
  }

  MemoryEntry entry(
      hash,
      touchCounter_++,
      featureVector,
      value,
      numVisits
  );

  auto &indexByHash = entries_.get<0>();
  auto found = indexByHash.find(hash);
  if (found != indexByHash.end()) {
    indexByHash.replace(found, entry);
  } else {
    indexByHash.insert(entry);
    if (entries_.size() > memorySize_) {
      auto &indexByTouchStamp = entries_.get<1>();
      indexByTouchStamp.erase(indexByTouchStamp.begin());
    }
  }
  if (logger) {
    logger->write("Update - Size: " + std::to_string(entries_.size()));
  }
}

std::pair<double, int> Memory::query(const FeatureVector &featureVector, Logger *logger) {

  auto &indexByHash = entries_.get<0>();

  std::priority_queue<std::pair<double, Hash128>> priorityQueue;
  for (const auto &entry : indexByHash) {
    double similarity = utils::cosineSimilarity(featureVector, entry.featureVector);
    priorityQueue.push(std::make_pair(-similarity, entry.hash));
    if (priorityQueue.size() > numNeighbors_) {
      priorityQueue.pop();
    }
  }

  std::vector<Hash128> topNeighbors;
  std::vector<double> distances;
  while (!priorityQueue.empty()) {
    auto topItem = priorityQueue.top();
    topNeighbors.push_back(topItem.second);
    // normalize to [0, 1] range
    distances.push_back(topItem.first + 1);
    priorityQueue.pop();
  }

  std::vector<std::shared_ptr<MemoryEntry>> entryPtrs;
  entryPtrs.reserve(topNeighbors.size());
  for (const auto &hash : topNeighbors) {
    entryPtrs.push_back(std::make_shared<MemoryEntry>(*indexByHash.find(hash)));
  }

  touchEntriesByHashes(topNeighbors);

//  for (const auto &entryPtr : entryPtrs) {
//    logger->write("entryPtr->value" + std::to_string(entryPtr->value));
//  }

  auto result = aggregatorPtr_->Aggregate(entryPtrs, distances);

  if (logger) {
    logger->write("Query - ");
  }

  return result;
}

void Memory::touchEntriesByHashes(const std::vector<Hash128> &NNHashes) {
  auto &indexByHash = entries_.get<0>();
  for (const auto &hash : NNHashes) {
    auto found = indexByHash.find(hash);
    auto currTouchCounter = touchCounter_++;
    indexByHash.modify(
        found,
        [&currTouchCounter](auto &entry) {
          entry.touchStamp = currTouchCounter;
        }
    );
  }
}

std::string Memory::toString() const {
  std::vector<std::string> strings;
  auto &indexByHash = entries_.get<0>();
  for (const auto &entry : indexByHash) {
    strings.push_back(entry.toString());
  }
  return boost::algorithm::join(strings, "\n");
}

bool Memory::isFull() const {
  return entries_.size() == memorySize_;
}

bool Memory::hasHash(const Hash128 &hash) {
  auto &indexByHash = entries_.get<0>();
  auto found = indexByHash.find(hash);
  return found != indexByHash.end();
}

size_t Memory::size() const {
  return entries_.size();
}
uint64_t Memory::getFeatureDim() const {
  return featureDim_;
}

void Memory::clear() {
  entries_.clear();
}

