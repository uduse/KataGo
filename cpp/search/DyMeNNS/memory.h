#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include "../../core/hash.h"
#include "node.h"

double cosine_similarity(float* A, float* B, int size);

class Memory{
public:
	int featureDimension;
	int memorySize;
	int numNeighbors;
	vector<Node> memArray;

	Memory(int featureDim, int memorySize, int numNeighbors);

	void update(Hash128 hash, float* featureVector, MemoryNodeStats stats);	

	MemoryNodeStats query(float* featureVector);
 
};