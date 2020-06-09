#include "memory.h"
using namespace std;

double cosine_similarity(float* A, float* B, int size){
	double numerator= 0;
	double a_denom = 0;
	double b_denom = 0;
	for(int i=0;i<size;i++){
		numerator += A[i] * B[i];
        a_denom += A[i] * A[i] ;
        b_denom += B[i] * B[i] ;
	}
	return numerator/(a_denom*b_denom);
}

double mergeMemoryValue(const double actualValue, const double memoryValue, const double lambda) {
  return ((1.0 - lambda)*actualValue) + (lambda*memoryValue);
}

Memory::Memory(int featureDim, int memorySize, int numNeighbors){
	this->featureDimension = featureDim;
	this->memorySize = memorySize;
	this->numNeighbors = numNeighbors;
}

void Memory::update(Hash128 hash, float* featureVector, MemoryNodeStats stats){
	Node newNode(hash, featureVector, this->featureDimension, stats);
	
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
	priority_queue<pair<double, double> > top_neighbours;
	double similarity;
	for(int i=0;i<memArray.size();i++){
		similarity = cosine_similarity(featureVector, memArray[i].feature, this->featureDimension);
		top_neighbours.push(make_pair(-similarity, i));
		if(top_neighbours.size() > this->numNeighbors){
			top_neighbours.pop();
		}
	}

	double utility = 0;
	double visits = 0;
	double similarity_sum = 0;
	int index;
	MemoryNodeStats stats = MemoryNodeStats();

	for(int i=0;i<this->numNeighbors;i++){
		pair<double, double> result = top_neighbours.top();
		top_neighbours.pop();
		
		similarity = result.first;
		index = result.second;
		similarity_sum += similarity;

		stats.winProb += similarity * memArray[index].stats.winProb;
		stats.noResultProb += similarity * memArray[index].stats.noResultProb;
		stats.scoreMean += similarity * memArray[index].stats.scoreMean;
		stats.scoreMeanSq += similarity * memArray[index].stats.scoreMeanSq;
		stats.lead += similarity * memArray[index].stats.lead;
		stats.utility += similarity * memArray[index].stats.utility;
		stats.utilitySum += similarity * memArray[index].stats.utilitySum;
		stats.utilitySqSum += similarity * memArray[index].stats.utilitySqSum;
		stats.visits += similarity * memArray[index].stats.visits;

	}

	stats.winProb /= similarity_sum;
	stats.noResultProb /= similarity_sum;
	stats.scoreMean /= similarity_sum;
	stats.scoreMeanSq /= similarity_sum;
	stats.lead /= similarity_sum;
	stats.utility /= similarity_sum;
	stats.utilitySum /= similarity_sum;
	stats.utilitySqSum /= similarity_sum;
	stats.visits /= similarity_sum;

	return stats;
}