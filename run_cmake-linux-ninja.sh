#!/bin/bash
mkdir cmake-linux-ninja-debug
pushd cmake-linux-ninja-debug
rm -rf CMakeCache.txt
ProjectRoot=/home/rene/workspace/repos
cmake .. -DCMAKE_BUILD_TYPE=Debug -DSCRIPTS_DIR=${ProjectRoot}/cmake-scripts -DCMAKE_INSTALL_PREFIX=${ProjectRoot}/install -DPLATFORM_INSTALL=${ProjectRoot}/install -G Ninja
popd

