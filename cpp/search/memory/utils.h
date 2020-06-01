#pragma once

#include <valarray>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <random>
#include <memory>
#include <functional>

//typedef int32_t EntryID;
typedef std::vector<double> FeatureVector;

namespace utils {

void timeIt(const std::function<void()> &func, const std::string &desc = "Time");
std::string toString(const FeatureVector &vector);
std::vector<double> softmax(const std::vector<double> &vector);
std::function<double()> getRandomDoubleFactory();
double cosineSimilarity(std::vector<double> lhs, std::vector<double> rhs);

}
