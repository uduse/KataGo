#pragma once

#include <valarray>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <random>
#include <memory>
#include <functional>

#include "../../../../annoy/src/kissrandom.h"
#include "../../../../annoy/src/annoylib.h"

typedef std::vector<double> FeatureVector;
typedef AnnoyIndex<int, double, Angular, Kiss32Random> IndexType;
typedef std::unique_ptr<IndexType> AnnoyPtr;

namespace utils {

void TimeIt(const std::function<void()> &func, const std::string &desc = "Time");
std::string ToString(const FeatureVector &vector);
std::string ToString(const std::vector<int> &vector);
std::vector<double> Softmax(const std::vector<double> &vector);
std::function<double()> GetRandomDoubleFactory();

}
