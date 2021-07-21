#!/bin/bash

mkdir -p bin
cd bin

cmake -DCMAKE_BUILD_TYPE=Debug ../src
make

#cd ..
#rm -f lib/libchai.so
#cp ./bin/chai/libchai.so ./lib
