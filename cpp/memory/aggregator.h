//
// Created by Zeyi Wang on 2020/5/5.
//

#pragma once

#include "utils.h"

class Aggregator {
 public:
  [[nodiscard]] virtual std::vector<double> Aggregate(
      const std::vector<FeatureVector> &vectors,
      const std::vector<double> &distances
  ) const;

};

class AverageAggregator : public Aggregator {
 public:
  [[nodiscard]] std::vector<double> Aggregate(
      const std::vector<FeatureVector> &vectors,
      const std::vector<double> &distances
  ) const override;
};

class WeightedAverageAggregator : public Aggregator {
 public:
  [[nodiscard]] std::vector<double> Aggregate(
      const std::vector<FeatureVector> &vectors,
      const std::vector<double> &distances
  ) const override;
};

class WeightedSoftmaxAggregator : public Aggregator {
 public:
  [[nodiscard]] std::vector<double> Aggregate(
      const std::vector<FeatureVector> &vectors,
      const std::vector<double> &distances
  ) const override;
};