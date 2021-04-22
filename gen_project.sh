#!/bin/bash

mkdir build
cd build
cmake .. \
	-DCMAKE_BUILD_TYPE=Debug \
	-DBUILD_TESTING=ON \
	-DMUGGLE_BUILD_SHARED_LIB=ON
