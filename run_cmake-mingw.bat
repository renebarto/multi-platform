mkdir cmake-mingw-debug
pushd cmake-mingw-debug
del CMakeCache.txt
setlocal
set ProjectRoot=F:/My_Projects

cmake .. -DCMAKE_BUILD_TYPE=Debug -DSCRIPTS_DIR="F:/My_Projects/cmake-scripts" -DCMAKE_INSTALL_PREFIX="F:\My_Projects\install-mingw" -DPLATFORM_INSTALL="F:\My_Projects\install-mingw" -G "MinGW Makefiles"
endlocal
popd
