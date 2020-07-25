#include "FeatureHashing.h"
// #include "smartgame/SgHash.h"
#include <algorithm>
#include <cmath>

int h(int x){
	return x;
}

// int h1Hash(int x){
// 	SgHash<32> hash(x);	
// 	return hash.Code1();
// }

int E(int x){
	return ((x % 2 == 1) ? 1 : -1);
	// if(x%2 == 0){
	// 	return 1;
	// }
	// return -1;
}

void FeatureHashing(float* bigArray, float* smallArray, int bigDimension, int smallDimension){
	memset(smallArray, 0, smallDimension);
	for(int i=0;i<bigDimension;i++){
		int hashValue = h(i);
		smallArray[hashValue % smallDimension] += E(hashValue) * bigArray[i];
	}


	float norm = 0;		
	for(int j=0;j<smallDimension;j++){
		norm += smallArray[j] * smallArray[j];
	}
	norm = sqrt(norm);

	for(int i=0;i<smallDimension;i++){
		smallArray[i] = smallArray[i] / norm;
	}

}

/*
int main(){
	// float* bigArray = new float[12];
	float bigArray[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

	float norm = 0;

	for(int i=0;i<12;i++){
		norm += bigArray[i] * bigArray[i];
	}

	norm = sqrt(norm);

	cout << "norm: " << norm << endl;

	for(int i=0;i<12;i++){
		bigArray[i] /= norm;
	}

	norm = sqrt(norm);



	float* smallArray = new float[4];

	FeatureHashing(bigArray, smallArray, 12, 4);

	for(int i=0;i<4;i++){
		cout << smallArray[i] << " ";
	}
	cout << endl;

	return 0;
}
*/