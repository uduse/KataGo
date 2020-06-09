#include "utils.h"

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
  return ((1.0 - lambda) * actualValue) + (lambda * memoryValue);
}

