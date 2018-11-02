#include "testlib.h"

#include <iostream>

using namespace std;

#if defined(WIN_MSVC)
void Initialize()
{
    //    cerr << "Initializing..." << endl;
}

void UnInitialize()
{
    //    cerr << "UnInitializing..." << endl;
}
#else
__attribute__((constructor)) void Initialize()
{
//    cerr << "Initializing..." << endl;
}

__attribute__((constructor)) void UnInitialize()
{
//    cerr << "UnInitializing..." << endl;
}
#endif

int TheAnswer()
{
    return 42;
}