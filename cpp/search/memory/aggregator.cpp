//
// Created by Zeyi Wang on 2020/5/5.
//

#include <numeric>
#include "aggregator.h"

static const double EPSILON = std::numeric_limits<double>::epsilon();

std::pair<double, double> Aggregator::Aggregate(
    const std::vector<std::shared_ptr<MemoryEntry>> &entries,
    const std::vector<double> &distances
) const {
  assert(entries.size() == distances.size());
  return std::make_pair(0.0, 0);
}

std::pair<double, double> AverageAggregator::Aggregate(
    const std::vector<std::shared_ptr<MemoryEntry>> &entries,
    const std::vector<double> &distances
) const {
  assert(entries.size() == distances.size());
  double valueSum = 0;
  double numVisitsSum = 0;
  for (const auto &entry : entries) {
    valueSum += entry->value;
    numVisitsSum += entry->numVisits;
  }
  return std::make_pair(valueSum / entries.size(), numVisitsSum / entries.size());
}

std::pair<double, double> WeightedAverageAggregator::Aggregate(
    const std::vector<std::shared_ptr<MemoryEntry>> &entries,
    const std::vector<double> &distances
) const {
  assert(entries.size() == distances.size());
  double distSum = 0;
  for (const auto &dist : distances) {
    distSum += dist;
  }
  double value = 0;
  double numVisits = 0;
  for (size_t i = 0; i < entries.size(); i++) {
    double weight = (distances[i] + EPSILON) / (distSum + EPSILON);
    value += entries[i]->value * weight;
    numVisits += static_cast<double>(entries[i]->numVisits * weight);
  }
  assert(0.0 <= value && value <= 1.0);
  return std::make_pair(value, numVisits);
}

std::pair<double, double> WeightedSoftmaxAggregator::Aggregate(
    const std::vector<std::shared_ptr<MemoryEntry>> &entries,
    const std::vector<double> &distances
) const {
  assert(entries.size() == distances.size());
  auto neighborWeights = utils::softmax(distances, temperature_);
  assert(entries.size() == neighborWeights.size());
  double value = 0;
  double weight = 0;
  for (size_t i = 0; i < entries.size(); i++) {
    value += neighborWeights[i] * entries[i]->value;
    weight += neighborWeights[i] * entries[i]->numVisits;
  }
  return std::make_pair(value, weight);
}
WeightedSoftmaxAggregator::WeightedSoftmaxAggregator(double temperature) : temperature_(temperature) {}
