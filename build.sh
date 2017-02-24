#!/bin/bash

if [ ! -d build ]; then
    mkdir build;
fi
cd build
if [ -f CMakeCache.txt ]; then
    rm CMakeCache.txt;
fi
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_INSTALL_PREFIX=/tmp
make
cd ..

