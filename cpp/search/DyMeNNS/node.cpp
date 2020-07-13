#include "node.h"

MemoryNodeStats::MemoryNodeStats()
	:visits(0.0), winProb(0.0), noResultProb(0.0), scoreMean(0.0), scoreMeanSq(0.0),
	lead(0.0), utility(0.0), utilitySum(0.0), utilitySqSum(0.0)
{}

MemoryNodeStats::~MemoryNodeStats()
{}

Node::Node(Hash128 hash, float* features, int featureSize, MemoryNodeStats stats_){
	this->hash = hash;
	this->featureSize = featureSize;
	feature = (float*) malloc(featureSize * sizeof(float));
	float maxValue = *(max_element(features, features + featureSize));
	// cout << maxValue << endl;
	for(int i=0;i<featureSize;i++){
		this->feature[i] = features[i] / maxValue;
	}
	// copy(features, features + featureSize, this->feature);
	// cout << *(max_element(this->feature, this->feature + featureSize)) << endl;
	this->stats.winProb = stats_.winProb;
	this->stats.noResultProb = stats_.noResultProb;
	this->stats.scoreMean = stats_.scoreMean;
	this->stats.scoreMeanSq = stats_.scoreMeanSq;
	this->stats.lead = stats_.lead;
	this->stats.utility = stats_.utility;
	this->stats.visits = stats_.visits;
}

void Node::printStats(){
	cout << "winProb: " << this->stats.winProb << endl;
	cout << "noResultProb: " << this->stats.noResultProb << endl;
	cout << "scoreMean: " << this->stats.scoreMean << endl;
	cout << "scoreMeanSq: " << this->stats.scoreMeanSq << endl;
	cout << "lead: " << this->stats.lead << endl;
	cout << "utility: " << this->stats.utility << endl;
	cout << "visits: " << this->stats.visits << endl;
}