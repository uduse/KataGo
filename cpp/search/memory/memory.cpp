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
    std::unique_ptr<Aggregator> &aggregatorPtr,
    Logger &logger
)
    : featureDim{featureDim},
      memorySize{memorySize},
      numNeighbors(numNeighbors),
      entries{},
      touchCounter{0},
      aggregatorPtr{std::move(aggregatorPtr)},
      logger{logger} {}

void Memory::update(
    const Hash128 &hash,
    const FeatureVector &featureVector,
    const double &value,
    const uint64_t &numVisits
) {
  assert(featureVector.size() == featureDim);

  MemoryEntry entry(
      hash,
      touchCounter++,
      featureVector,
      value,
      numVisits
  );

  auto &indexByHash = entries.get<0>();
  auto found = indexByHash.find(hash);
  if (found != indexByHash.end()) {
    indexByHash.replace(found, entry);
  } else {
    indexByHash.insert(entry);
    if (entries.size() > memorySize) {
      auto &indexByTouchStamp = entries.get<1>();
      indexByTouchStamp.erase(indexByTouchStamp.begin());
    }
  }
}

std::pair<double, int> Memory::query(const FeatureVector &featureVector) {

  auto &indexByHash = entries.get<0>();

  std::priority_queue<std::pair<double, Hash128>> priorityQueue;
  for (const auto &entry : indexByHash) {
    double similarity = utils::cosineSimilarity(featureVector, entry.featureVector);
    priorityQueue.push(std::make_pair(-similarity, entry.hash));
    if (priorityQueue.size() > numNeighbors) {
      priorityQueue.pop();
    }
  }

  std::vector<Hash128> topNeighbors;
  std::vector<double> distances;
  while (!priorityQueue.empty()) {
    auto topItem = priorityQueue.top();
    topNeighbors.push_back(topItem.second);
    distances.push_back(topItem.first + 1);
    priorityQueue.pop();
  }

  std::vector<std::shared_ptr<MemoryEntry>> entryPtrs;
  for (const auto &hash : topNeighbors) {
    std::make_shared<MemoryEntry>(*indexByHash.find(hash));
  }

  touchEntriesByHashes(topNeighbors);

  auto result = aggregatorPtr->Aggregate(entryPtrs, distances);
  return result;
}

void Memory::touchEntriesByHashes(const std::vector<Hash128> &NNHashes) {
  auto &indexByHash = entries.get<0>();
  for (auto &hash : NNHashes) {
    auto found = indexByHash.find(hash);
    auto currTouchCounter = touchCounter++;
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
  auto &indexByHash = entries.get<0>();
  for (const auto &entry : indexByHash) {
    strings.push_back(entry.toString());
  }
  return boost::algorithm::join(strings, "\n");
}

bool Memory::isFull() const {
  return entries.size() == memorySize;
}

bool Memory::hasHash(const Hash128 &hash) {
  auto &indexByHash = entries.get<0>();
  auto found = indexByHash.find(hash);
  return found != indexByHash.end();
}

size_t Memory::size() const {
  return entries.size();
}

