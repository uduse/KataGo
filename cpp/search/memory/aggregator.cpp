//
// Created by Zeyi Wang on 2020/5/5.
//

#include <numeric>
#include "aggregator.h"

static const double EPSILON = std::numeric_limits<double>::epsilon();

std::pair<double, int> Aggregator::Aggregate(
  const std::vector<std::shared_ptr<MemoryEntry>> &entries,
  const std::vector<double> &distances
) const {
  return std::make_pair(0.0, 0);
}

std::pair<double, int> AverageAggregator::Aggregate(
  const std::vector<std::shared_ptr<MemoryEntry>> &entries,
  const std::vector<double> &distances
) const {
  double valueSum = 0;
  int numVisitsSum = 0;
  for (const auto &entry : entries) {
    valueSum += entry->value;
    numVisitsSum += entry->numVisits;
  }
  return std::make_pair(valueSum / entries.size(), numVisitsSum / entries.size());
}

std::pair<double, int> WeightedAverageAggregator::Aggregate(
  const std::vector<std::shared_ptr<MemoryEntry>> &entries,
  const std::vector<double> &distances
) const {
  double distSum = 0;
  for (const auto &dist : distances) {
    distSum += dist;
  }
  double value = 0;
  int numVisits = 0;
  for (size_t i = 0; i < entries.size(); i++) {
    double weight = (distances[i] + EPSILON) / (distSum + EPSILON);
    value += entries[i]->value * weight;
    numVisits += static_cast<int>(entries[i]->numVisits * weight);
  }
  assert(0.0 <= value && value <= 1.0);
  return std::make_pair(value, numVisits);
}

std::pair<double, int> WeightedSoftmaxAggregator::Aggregate(
  const std::vector<std::shared_ptr<MemoryEntry>> &entries,
  const std::vector<double> &distances
) const {
  auto weights = utils::softmax(distances);
  assert(entries.size() == weights.size());
  double value = 0;
  int numVisits = 0;
  for (size_t i = 0; i < entries.size(); i++) {
    value += weights[i] * entries[i]->value;
    numVisits += static_cast<int>(entries[i]->numVisits * weights[i]);
  }
  assert(0.0 <= value && value <= 1.0);
  return std::make_pair(value, numVisits);
}
