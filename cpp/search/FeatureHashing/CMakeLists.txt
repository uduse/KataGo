cmake_minimum_required(VERSION 3.10)


# set the project name
project(FeatureHashing VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(FeatureHashing FeatureHashing.cpp 
							  MurmurHash3.cpp 
							  utils.cpp
							  FeatureHashing.h
							  MurmurHash3.h
							  utils.h
				)