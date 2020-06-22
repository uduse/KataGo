#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <cmath>
#include <string.h>
#include "../../core/hash.h"
#include "node.h"
#include "utils.h"

enum aggregators {weighted_average, weighted_softmax};

class Memory{
public:
	int featureDimension;
	int memorySize;
	int numNeighbors;
	vector<Node> memArray;

	Memory(int featureDim, int memorySize_, int numNeighbors_);

	void update(Hash128 hash, float* featureVector, MemoryNodeStats stats);	

	MemoryNodeStats query(float* featureVector);

	aggregators aggregator;

	double tau;

	void setTau(double tau_);

	void setAggregator(const char* aggregator);

	void printMemory();
 
};