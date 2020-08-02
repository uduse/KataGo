#include "memory.h"
using namespace std;

Memory::Memory(int featureDim, int memorySize_, int numNeighbors_){
	this->featureDimension = featureDim;
	this->memorySize = memorySize_;
	this->numNeighbors = numNeighbors_;
	this->aggregator = weighted_average;
	this->tau = 0.1;
}

Memory::~Memory(){
	this->memArray.clear();
}

void Memory::setTau(double tau_){
	this->tau = tau_;
}

void Memory::setAggregator(const char* aggregator_){
	char* weighted_average_string = "weighted_average";
	char* weighted_softmax_string = "weighted_softmax";
	if(strcmp(weighted_average_string, aggregator_) == 0){
		this->aggregator = weighted_average;
		// cout << "Aggregator set to weighted_average" << endl;
	}
	
	if(strcmp(weighted_softmax_string, aggregator_) == 0){
		this->aggregator = weighted_softmax;
		// cout << "Aggregator set to weighted_softmax" << endl;
	}
	return;
}

void Memory::printMemory(){
	cout << "Memory Size: " << this->memorySize << endl;
	if(this->aggregator == weighted_average){
		cout << "Aggregator: weighted_average" << endl;
	}
	else{
		cout << "Aggregator: weighted_softmax" << endl;	
		cout << "tau: " << tau << endl;
	}
	cout << "numNeighbors: " << numNeighbors << endl;

	for(int i=0;i<memArray.size();i++){
		cout << "Entry: " << (i+1) << endl;
		memArray[i].printStats();
	}
}

void Memory::update(Hash128 hash, float* featureVector, MemoryNodeStats stats){
	Node newNode(hash, featureVector, this->featureDimension, stats);
	if(memArray.size() < this->memorySize){
		for(int i=0;i<memArray.size();i++){
			if(memArray[i].hash == hash){
				int existingVisits = memArray[i].stats.visits;
				int newVisits = newNode.stats.visits;
				newNode.stats.visits += memArray[i].stats.visits;
				newNode.stats.winProb = memArray[i].stats.winProb + ((newNode.stats.winProb - memArray[i].stats.winProb) / newNode.stats.visits);
				newNode.stats.noResultProb = memArray[i].stats.noResultProb + ((newNode.stats.noResultProb - memArray[i].stats.noResultProb) / newNode.stats.visits);
				newNode.stats.scoreMean = memArray[i].stats.scoreMean + ((newNode.stats.scoreMean - memArray[i].stats.scoreMean) / newNode.stats.visits);
				newNode.stats.scoreMeanSq = memArray[i].stats.scoreMeanSq + ((newNode.stats.scoreMeanSq - memArray[i].stats.scoreMeanSq) / newNode.stats.visits);
				newNode.stats.lead = memArray[i].stats.lead + ((newNode.stats.lead - memArray[i].stats.lead) / newNode.stats.visits);
				newNode.stats.utility = memArray[i].stats.utility + ((newNode.stats.utility - memArray[i].stats.utility) / newNode.stats.visits);

				memArray.erase(memArray.begin()+i);
				break;
			}
		}
		memArray.push_back(newNode);
	}

	else{
		bool isPresent = false;
		for(int i=0; i < this->memorySize; i++){
			if(memArray[i].hash == hash){
				isPresent = true;
				int existingVisits = memArray[i].stats.visits;
				int newVisits = newNode.stats.visits;
				newNode.stats.visits += memArray[i].stats.visits;
				newNode.stats.winProb = memArray[i].stats.winProb + ((newNode.stats.winProb - memArray[i].stats.winProb) / newNode.stats.visits);
				newNode.stats.noResultProb = memArray[i].stats.noResultProb + ((newNode.stats.noResultProb - memArray[i].stats.noResultProb) / newNode.stats.visits);
				newNode.stats.scoreMean = memArray[i].stats.scoreMean + ((newNode.stats.scoreMean - memArray[i].stats.scoreMean) / newNode.stats.visits);
				newNode.stats.scoreMeanSq = memArray[i].stats.scoreMeanSq + ((newNode.stats.scoreMeanSq - memArray[i].stats.scoreMeanSq) / newNode.stats.visits);
				newNode.stats.lead = memArray[i].stats.lead + ((newNode.stats.lead - memArray[i].stats.lead) / newNode.stats.visits);
				newNode.stats.utility = memArray[i].stats.utility + ((newNode.stats.utility - memArray[i].stats.utility) / newNode.stats.visits);

				memArray.erase(memArray.begin()+i);
				break;
			}
		}
		if(!isPresent){
			memArray.erase(memArray.begin());
		}
		memArray.push_back(newNode);
	}
}


MemoryNodeStats Memory::query(float* featureVector){
	priority_queue<pair<double, int> > top_neighbours;
	double similarity;
	for(int i=0;i<memArray.size();i++){
		similarity = cosine_similarity(featureVector, memArray[i].feature, this->featureDimension);
		// cout << similarity << endl;
		
		// cout << getNorm(featureVector, this->featureDimension) << " " << getNorm(memArray[i].feature, featureDimension) << endl;
/*
		int smallFeatureDimension = 4000;
		float* smallVector1 = new float[smallFeatureDimension];
		float* smallVector2 = new float[smallFeatureDimension];
		
		FeatureHashing(featureVector, smallVector1, 15552, smallFeatureDimension);
		FeatureHashing(memArray[i].feature, smallVector2, 15552, smallFeatureDimension);
		
		float dot_product = 0; 
		for(int j=0;j<smallFeatureDimension;j++){
			dot_product += smallVector1[j] * smallVector2[j];
		}

		cout << similarity << " " << dot_product << endl;
*/
		top_neighbours.push(make_pair(-similarity, i));
		if(top_neighbours.size() > this->numNeighbors){
			top_neighbours.pop();
		}

	}

	
/*	
	cout << "-------------------------------------------------------------------------------------" << endl; 
	priority_queue<pair<double, int> > temp = top_neighbours;
	while (!temp.empty()) {
        cout << temp.top().first << " ";
        temp.pop();
    }
    cout << endl;
    temp = top_neighbours;
	while (!temp.empty()) {
        cout << memArray[temp.top().second].stats.utility << " ";
        // cout << temp.top().second << " ";
        temp.pop();
    }
    cout << endl;
    
    cout << "-------------------------------------------------------------------------------------" << endl;
*/

	int index;
	MemoryNodeStats stats = MemoryNodeStats();
	double weight = 0;
	double weightSum = 0;
	
	for(int i=0;i<this->numNeighbors;i++){
		pair<double, int> result = top_neighbours.top();
		top_neighbours.pop();

		index = result.second;

		if(this->aggregator == weighted_average){
			weight = -result.first;
			weightSum += weight;
		}

		if(this->aggregator == weighted_softmax){
			// cout << memArray[index].stats.visits << endl;
			weight = memArray[index].stats.visits * exp(-(1 + result.first)/this->tau);
			weightSum += weight;
		}

		stats.winProb += weight * memArray[index].stats.winProb;
		stats.noResultProb += weight * memArray[index].stats.noResultProb;
		stats.scoreMean += weight * memArray[index].stats.scoreMean;
		stats.scoreMeanSq += weight * memArray[index].stats.scoreMeanSq;
		stats.lead += weight * memArray[index].stats.lead;
		stats.utility += weight * memArray[index].stats.utility;
		stats.visits += weight * memArray[index].stats.visits;
	}

	stats.winProb /= weightSum;
	stats.noResultProb /= weightSum;
	stats.scoreMean /= weightSum;
	stats.scoreMeanSq /= weightSum;
	stats.lead /= weightSum;
	stats.utility /= weightSum;
	stats.visits /= weightSum;

	return stats;
}