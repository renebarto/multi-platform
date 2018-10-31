setlocal
mkdir cmake-msvc-debug
cd cmake-msvc-debug
del CMakeCache.txt
call "%VS140COMNTOOLS%..\..\VC\vcvarsall.bat"
cmake .. -DCMAKE_BUILD_TYPE=Debug -DSCRIPTS_DIR="F:/My_Projects/cmake-scripts" -DCMAKE_INSTALL_PREFIX="F:\My_Projects\install-msvc" -DPLATFORM_INSTALL="F:\My_Projects\install-msvc" -G "Visual Studio 14 2015 Win64"
cd ..
endlocal