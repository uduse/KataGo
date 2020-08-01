#include "FeatureHashing.h"

int hash1(int x){
	return x;
}

int hash2(int x){
	uint32_t seed = 0;
	uint32_t hash_output[1] = {0};
	const int *key = &x;
	MurmurHash3_x86_32(key, sizeof(int), seed, hash_output); // 0xb6d99cf8
	return hash_output[0];
}

int E(int x){
	return ((x % 2 == 1) ? 1 : -1);
}

void FeatureHashing(float* bigArray, float* smallArray, int bigDimension, int smallDimension){
	memset(smallArray, 0, sizeof(float) * smallDimension);

	for(int i=0;i<bigDimension;i++){
		int hashValue = hash2(i);
		smallArray[hashValue % smallDimension >=0 ? hashValue % smallDimension : (hashValue % smallDimension) + smallDimension] += E(i) * bigArray[i];
	}

	float norm = sqrt(computeDotProduct(smallArray, smallArray, smallDimension));
	normalizeArray(smallArray, smallDimension, norm);
}
/*

int main(){
	int bigDimension = 15552;
	float bigArray1[bigDimension], bigArray2[bigDimension];
	srand(time(0)); 

	for(int i=0;i<bigDimension;i++){
		bigArray1[i] = ((float) rand()) / (float) RAND_MAX;
		bigArray2[i] = ((float) rand()) / (float) RAND_MAX;
	}

	float norm1 = sqrt(computeDotProduct(bigArray1, bigArray1, bigDimension));
	float norm2 = sqrt(computeDotProduct(bigArray2, bigArray2, bigDimension));

	normalizeArray(bigArray1, bigDimension, norm1);
	normalizeArray(bigArray2, bigDimension, norm2);

	norm1 = sqrt(computeDotProduct(bigArray1, bigArray1, bigDimension));
	norm2 = sqrt(computeDotProduct(bigArray2, bigArray2, bigDimension));

	int smallDimension = 2000;
	float smallArray1[smallDimension], smallArray2[smallDimension];
	FeatureHashing(bigArray1, smallArray1, bigDimension, smallDimension);
	FeatureHashing(bigArray2, smallArray2, bigDimension, smallDimension);

	cout << computeDotProduct(bigArray1, bigArray2, bigDimension) << " " << computeDotProduct(smallArray1, smallArray2, smallDimension) << endl;

	return 0;
}
*/