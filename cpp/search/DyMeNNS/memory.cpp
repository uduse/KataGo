#include "memory.h"
using namespace std;

Memory::Memory(int featureDim, int memorySize_, int numNeighbors_){
	this->featureDimension = featureDim;
	this->memorySize = memorySize_;
	this->numNeighbors = numNeighbors_;
	this->aggregator = weighted_average;
	this->tau = 0.1;
}

void Memory::setTau(double tau_){
	this->tau = tau_;
}

void Memory::setAggregator(const char* aggregator_){
	char* weighted_average_string = "weighted_average";
	char* weighted_softmax_string = "weighted_softmax";
	if(strcmp(weighted_average_string, aggregator_) == 0){
		this->aggregator = weighted_average;
		cout << "Aggregator set to weighted_average" << endl;
	}
	
	if(strcmp(weighted_softmax_string, aggregator_) == 0){
		this->aggregator = weighted_softmax;
		cout << "Aggregator set to weighted_softmax" << endl;
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
	// cout << stats.utility << endl;
	if(memArray.size() < this->memorySize){
		for(int i=0;i<memArray.size();i++){
			if(memArray[i].hash == hash){
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

		if(this->aggregator == weighted_average){
			weight = -result.first;
			weightSum += weight;
		}

		if(this->aggregator == weighted_softmax){
			weight = exp(-result.first/this->tau);
			weightSum += weight;
		}

		index = result.second;

		stats.winProb += weight * memArray[index].stats.winProb;
		stats.noResultProb += weight * memArray[index].stats.noResultProb;
		stats.scoreMean += weight * memArray[index].stats.scoreMean;
		stats.scoreMeanSq += weight * memArray[index].stats.scoreMeanSq;
		stats.lead += weight * memArray[index].stats.lead;
		stats.utility += weight * memArray[index].stats.utility;
		stats.utilitySum += weight * memArray[index].stats.utilitySum;
		stats.utilitySqSum += weight * memArray[index].stats.utilitySqSum;
		stats.visits += weight * memArray[index].stats.visits;
	}

	stats.winProb /= weightSum;
	stats.noResultProb /= weightSum;
	stats.scoreMean /= weightSum;
	stats.scoreMeanSq /= weightSum;
	stats.lead /= weightSum;
	stats.utility /= weightSum;
	stats.utilitySum /= weightSum;
	stats.utilitySqSum /= weightSum;
	stats.visits /= weightSum;

	return stats;
}