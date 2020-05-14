//
// Created by Zeyi Wang on 2020/5/2.
//

#include <iostream>
#include <algorithm>
#include <random>
#include <functional>

#include "memory.h"
#include "utils.h"

int main(int argc, char **argv) {

  spdlog::set_level(spdlog::level::debug);

  const unsigned int feature_dim = 4;
  const unsigned int memory_size = 32;
  const unsigned int num_tree = 32;
  const unsigned int num_neighbors = 4;

  std::unique_ptr <Aggregator> aggregator_ptr = std::make_unique<AverageAggregator>();

  Memory memory{feature_dim, memory_size, num_tree, num_neighbors, aggregator_ptr};

  for (unsigned int id = 0; id < memory_size; id++) {
    FeatureVector vector(feature_dim);
    std::generate(std::begin(vector), std::end(vector), utils::GetRandomDoubleFactory());
    memory.Update(id, vector);
  }

  spdlog::debug(memory.ToString());

  for (unsigned int id = 0; id < 10; id++) {
    FeatureVector vector(feature_dim);
    std::generate(std::begin(vector), std::end(vector), utils::GetRandomDoubleFactory());
    spdlog::debug("Query: {}", utils::ToString(vector));
    memory.Query(vector);
    spdlog::debug(memory.ToString());
  }

  spdlog::debug(memory.ToString());

  for (unsigned int i = 0; i < memory_size / 2; i++) {
    FeatureVector vec(feature_dim);
    std::generate(std::begin(vec), std::end(vec), utils::GetRandomDoubleFactory());
    memory.Update(i + memory_size, vec);
  }

  spdlog::debug(memory.ToString());

  utils::TimeIt([&memory]() {
    memory.Build();
  }, "Build memory.");

  return EXIT_SUCCESS;
}
