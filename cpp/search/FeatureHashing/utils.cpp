#include "utils.h"

float computeDotProduct(float* array1, float* array2, int dimension){
	float dot_product = 0;
	for(int i=0;i<dimension;i++){
		dot_product += array1[i] * array2[i];
	}
	return dot_product;
}

void normalizeArray(float* array, int dimension, float scalar){
	for(int i=0;i<dimension;i++){
		array[i] /= scalar;
	}
}