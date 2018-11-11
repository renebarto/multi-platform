mkdir cmake-linux-debug
pushd cmake-linux-debug
rm CMakeCache.txt
ProjectRoot=/home/rene/workspace/repos
cmake .. -DCMAKE_BUILD_TYPE=Debug -DSCRIPTS_DIR=${ProjectRoot}/cmake-scripts -DCMAKE_INSTALL_PREFIX=${ProjectRoot}/install -DPLATFORM_INSTALL=${ProjectRoot}/install
popd

