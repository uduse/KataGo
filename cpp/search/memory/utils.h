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

typedef int32_t EntryID;
typedef std::vector<double> FeatureVector;
typedef AnnoyIndex<EntryID, double, Angular, Kiss32Random> IndexType;
typedef std::unique_ptr<IndexType> AnnoyPtr;

namespace utils {

void timeIt(const std::function<void()> &func, const std::string &desc = "Time");
std::string toString(const FeatureVector &vector);
std::string ToString(const std::vector<uint64_t> &vector);
std::vector<double> softmax(const std::vector<double> &vector);
std::function<double()> GetRandomDoubleFactory();

}
