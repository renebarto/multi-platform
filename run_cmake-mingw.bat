mkdir cmake-mingw-debug
cd cmake-mingw-debug
del CMakeCache.txt
cmake .. -DCMAKE_BUILD_TYPE=Debug -DSCRIPTS_DIR="F:/My_Projects/cmake-scripts" -DCMAKE_INSTALL_PREFIX="F:\My_Projects\install-mingw" -DPLATFORM_INSTALL="F:\My_Projects\install-mingw" -G "MinGW Makefiles"
cd ..
