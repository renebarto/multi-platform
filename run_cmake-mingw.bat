mkdir cmake-mingw-debug
cd cmake-mingw-debug
del CMakeCache.txt
cmake .. -DSCRIPTS_DIR="F:/VisualStudio2017Projects/cmake-scripts" -DCMAKE_INSTALL_PREFIX="F:\VisualStudio2017Projects\install-mingw" -DPLATFORM_INSTALL="F:\VisualStudio2017Projects\install-mingw" -G "MinGW Makefiles"
cd ..
