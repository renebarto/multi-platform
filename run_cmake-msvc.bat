mkdir cmake-msvc-debug
cd cmake-msvc-debug
del CMakeCache.txt
call "%VS140COMNTOOLS%..\..\VC\vcvarsall.bat"
cmake .. -DSCRIPTS_DIR="F:/VisualStudio2017Projects/cmake-scripts" -DCMAKE_INSTALL_PREFIX="F:\VisualStudio2017Projects\install-msvc" -DPLATFORM_INSTALL="F:\VisualStudio2017Projects\install-msvc" -G "Visual Studio 14 2015 Win64"
cd ..
