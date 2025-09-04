#!/bin/bash

clear 

rm -fr build 

if ! cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DMAX_LOG_LEVEL=3
then
  echo "####### fail to configure! #####"
  exit 1
fi

if ! cmake --build build --target all
then
  echo "####### fail to build! #####"
  exit 1
fi

