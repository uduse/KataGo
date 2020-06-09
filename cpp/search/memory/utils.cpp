//
// Created by Zeyi Wang on 2020/5/4.
//

#include "utils.h"

#include <boost/algorithm/string/join.hpp>
#include <chrono>

void utils::timeIt(const std::function<void()> &func, const std::string &desc) {
  std::chrono::high_resolution_clock::time_point t_start, t_end;
  t_start = std::chrono::high_resolution_clock::now();
  func();
  t_end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_start).count();
  std::cout << desc << std::to_string(duration) << std::endl;
}

std::string utils::toString(const FeatureVector &vector) {
  std::vector<std::string> strings;
  std::transform(
      std::begin(vector),
      std::end(vector),
      std::back_inserter(strings),
      [](auto &val) {
        return std::to_string(val);
      });
  return "[" + boost::algorithm::join(strings, ", ") + "]";
}

std::string utils::toString(const std::vector<double> &vector) {
  std::vector<std::string> strings;
  std::transform(
      std::begin(vector),
      std::end(vector),
      std::back_inserter(strings),
      [](auto &val) {
        return std::to_string(val);
      });
  return "[" + boost::algorithm::join(strings, ", ") + "]";
}

std::vector<double> utils::softmax(const std::vector<double> &vector, const double &temperature) {
  assert(0 < temperature && temperature < 10);
  std::vector<double> exps;
  std::transform(
      vector.begin(), vector.end(), std::back_inserter(exps),
      [&temperature](const double &val) {
        return std::exp(val / temperature);
      }
  );
  double exp_sum = std::accumulate(exps.begin(), exps.end(), 0.0);
  std::vector<double> result;
  std::transform(
      vector.begin(), vector.end(), std::back_inserter(result),
      [&exp_sum, &temperature](const double &val) {
        return std::exp(val / temperature) / exp_sum;
      });
  return result;
}

std::function<double()> utils::getRandomDoubleFactory() {
  static std::random_device random_device;
  static std::mt19937 engine{random_device()};
  static std::normal_distribution<double> distribution{0.0, 1.0};
//  static std::uniform_real_distribution<double> distribution{0.0, 1.0};
  return []() { return distribution(engine); };
}

double utils::cosineSimilarity(FeatureVector lhs, FeatureVector rhs) {
  assert(lhs.size() == rhs.size());
  float numerator = 0;
  float lhs_denom = 0;
  float rhs_denom = 0;
  for (int i = 0; i < lhs.size(); i++) {
    numerator += lhs[i] * rhs[i];
    lhs_denom += lhs[i] * lhs[i];
    rhs_denom += rhs[i] * rhs[i];
  }
  return numerator / (lhs_denom * rhs_denom);
}


