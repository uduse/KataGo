#include "node.h"

MemoryNodeStats::MemoryNodeStats()
	:visits(0.0), winProb(0.0), noResultProb(0.0), scoreMean(0.0), scoreMeanSq(0.0),
	lead(0.0), utility(0.0), utilitySum(0.0), utilitySqSum(0.0)
{}

MemoryNodeStats::~MemoryNodeStats()
{}

Node::Node(Hash128 hash, float* features, int featureSize, MemoryNodeStats stats){
	this->hash = hash;
	this->featureSize = featureSize;
	feature = (float*) malloc(featureSize * sizeof(float));
	copy(features, features + featureSize, this->feature);
	this->stats.winProb = stats.winProb;
	this->stats.noResultProb = stats.noResultProb;
	this->stats.scoreMean = stats.scoreMean;
	this->stats.scoreMeanSq = stats.scoreMeanSq;
	this->stats.lead = stats.lead;
	this->stats.utility = stats.utility;
	this->stats.utilitySum = stats.utilitySum;
	this->stats.utilitySqSum = stats.utilitySqSum;
	this->stats.visits = stats.visits;
}