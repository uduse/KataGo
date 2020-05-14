//
// Created by Zeyi Wang on 2020/5/5.
//

#include <numeric>
#include "aggregator.h"

std::vector<double> Aggregator::Aggregate(
    const std::vector<FeatureVector> &vectors,
    const std::vector<double> &distances
) const {
  return std::vector<double>();
}

std::vector<double> AverageAggregator::Aggregate(
    const std::vector<FeatureVector> &vectors,
    const std::vector<double> &distances
) const {
  std::vector<std::valarray<double>> val_arrays;

  // transpose
  std::transform(
      vectors.begin(), vectors.end(), std::back_inserter(val_arrays),
      [](const auto &vector) {
        return std::valarray<double>(vector.data(), vector.size());
      }
  );
  std::valarray<double> zero = std::valarray<double>(double{0}, val_arrays[0].size());
  std::valarray<double> sums = std::accumulate(
      val_arrays.begin(), val_arrays.end(), zero
  );
  std::valarray<double> average = sums / static_cast<double>(vectors.size());
  std::vector<double> result;
  for (const auto &val : average) {
    result.push_back(val);
  }
  return result;
}

std::vector<double> WeightedAverageAggregator::Aggregate(
    const std::vector<FeatureVector> &vectors,
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
  return std::vector<double>();
}

std::vector<double> WeightedSoftmaxAggregator::Aggregate(
    const std::vector<FeatureVector> &vectors,
    const std::vector<double> &distances
) const {
  // TODO: implement
  return std::vector<double>();
}
