#include "FeatureHashing.h"
#include <string.h>

int h(int x){
	return x;
}

int E(int x){
	if(x%2 == 0){
		return 1;
	}
	return -1;
}

void FeatureHashing(float* bigArray, float* smallArray, int bigDimension, int smallDimension){
	memset(smallArray, 0, smallDimension);
	for(int i=0;i<bigDimension;i++){
		smallArray[h(i) % smallDimension] += E(i) * bigArray[i];
	}
	free(bigArray);
}







/*

int main(){
	// float* bigArray = new float[12];
	float bigArray[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
	float* smallArray = new float[4];

	FeatureHashing(bigArray, smallArray, 12, 4);

	for(int i=0;i<4;i++){
		cout << smallArray[i] << " ";
	}
	cout << endl;

	return 0;
}
*/