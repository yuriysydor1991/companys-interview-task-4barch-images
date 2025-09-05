#!/bin/bash

clear 

rm -fr build 

# for thread sanitizers - ENABLE_SANITIZERS_THREADS
if ! cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_CLANGFORMAT=ON \
  -DENABLE_CPPCHECK=ON -DENABLE_COMPONENT_TESTS=ON \
  -DENABLE_UNIT_TESTS=ON -DMAX_LOG_LEVEL=6 \
  -DENABLE_SANITIZERS=ON
then
  echo "####### fail to configure! #####"
  exit 1
fi

if ! cmake --build build --target clang-format all
then
  echo "####### fail to build! #####"
  exit 1
fi

