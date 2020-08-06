#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include "../../core/hash.h"

using namespace std;

class MemoryNodeStats{
public:
	double winProb;
	double noResultProb;
	double scoreMean;
	double scoreMeanSq;
	double lead;
	double utility;
	double utilitySum;
	double utilitySqSum;
	double visits;
  	MemoryNodeStats();
  	~MemoryNodeStats();
};

class Node{
public:
	Hash128 hash;
	float* feature;
	int featureSize;
	MemoryNodeStats* stats;
	void printStats();
	Node(Hash128 hash_, float* features, int featureSize, MemoryNodeStats* stats_);
	~Node();
};