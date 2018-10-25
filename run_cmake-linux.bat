mkdir -p cmake-linux-debug
pushd cmake-linux-debug
del CMakeCache.txt
cmake .. -DCMAKE_BUILD_TYPE=Debug -DSCRIPTS_DIR="~/workspace/cmake-scripts" -DCMAKE_INSTALL_PREFIX="~/install-multi-platform" -DPLATFORM_INSTALL="~/install-multi-platform" -G "Unix Makefiles"
popd
