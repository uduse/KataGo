//
// Created by Zeyi Wang on 2020/5/5.
//

#include <numeric>
#include "aggregator.h"

double Aggregator::Aggregate(
    const std::vector<std::shared_ptr<MemoryEntry>> &entries,
    const std::vector<double> &distances
) const {
  return 0;
}

double AverageAggregator::Aggregate(
    const std::vector<std::shared_ptr<MemoryEntry>> &entries,
    const std::vector<double> &distances
) const {
  double valueSum = 0;
  for (const auto &entry : entries) {
    valueSum += entry->value;
  }
  return valueSum / entries.size();
}

double WeightedAverageAggregator::Aggregate(
    const std::vector<std::shared_ptr<MemoryEntry>> &entries,
    const std::vector<double> &distances
) const {
  std::vector<double> norm_weights;
  double min = *std::min_element(distances.begin(), distances.end());
  double max = *std::max_element(distances.begin(), distances.end());
  double range = max - min;

  // squash to [0, 1] range
  std::transform(
      distances.begin(), distances.end(), std::back_inserter(norm_weights),
      [&](const auto &val) {
        return (val - min) / range;
      }
  );

  // normalize sum to 1
  double sum = std::accumulate(norm_weights.begin(), norm_weights.end(), double{0});
  std::transform(
      norm_weights.begin(), norm_weights.end(), norm_weights.begin(),
      [&](const auto &val) {
        return val / sum;
      }
  );

  // TODO: implement
  return 0;
}

double WeightedSoftmaxAggregator::Aggregate(
    const std::vector<std::shared_ptr<MemoryEntry>> &entries,
    const std::vector<double> &distances
) const {
  // TODO: implement
  return 0;
}
