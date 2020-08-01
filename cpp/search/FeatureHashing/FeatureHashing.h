#pragma once
#include <iostream>
#include <string.h>
#include <algorithm>
#include <cmath>
#include <random>
#include <time.h>
#include "MurmurHash3.h"
#include "utils.h"

using namespace std;

int hash1(int x);

int hash2(int x);

int E(int x);

void FeatureHashing(float* bigArray, float* smallArray, int bigDimension, int smallDimension);
