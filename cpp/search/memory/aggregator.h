//
// Created by Zeyi Wang on 2020/5/5.
//

#pragma once

#include "utils.h"
#include "memory_entry.h"

class Aggregator {
public:
  [[nodiscard]] virtual std::pair<double, int> Aggregate(
      const std::vector<std::shared_ptr<MemoryEntry>> &vectors,
      const std::vector<double> &distances
  ) const;

};

class AverageAggregator : public Aggregator {
public:
  [[nodiscard]] std::pair<double, int> Aggregate(
      const std::vector<std::shared_ptr<MemoryEntry>> &vectors,
      const std::vector<double> &distances
  ) const override;
};

class WeightedAverageAggregator : public Aggregator {
public:
  [[nodiscard]] std::pair<double, int> Aggregate(
      const std::vector<std::shared_ptr<MemoryEntry>> &vectors,
      const std::vector<double> &distances
  ) const override;
};

class WeightedSoftmaxAggregator : public Aggregator {
public:
  [[nodiscard]] std::pair<double, int> Aggregate(
      const std::vector<std::shared_ptr<MemoryEntry>> &vectors,
      const std::vector<double> &distances
  ) const override;
};