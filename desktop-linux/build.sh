#!/bin/bash
#cd ./build && cmake ./ && make && ./JnRMaker

#rm -rf build
mkdir build
#mkdir bin
cd build
mkdir debug
cd debug
cmake ../.. -DCMAKE_BUILD_TYPE="Debug"
cmake --build .
#cp JnRMaker ../../
