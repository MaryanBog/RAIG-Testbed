#!/usr/bin/env bash
set -e
export PATH="/c/msys64/mingw64/bin:/c/msys64/usr/bin:$PATH"
rm -rf build
cmake -S . -B build -G Ninja -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
cmake --build build
ctest --test-dir build -V
