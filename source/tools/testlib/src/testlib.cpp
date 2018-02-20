#include "testlib.h"

#include <iostream>

using namespace std;

__attribute__((constructor)) void Initialize()
{
//    cerr << "Initializing..." << endl;
}

__attribute__((constructor)) void UnInitialize()
{
//    cerr << "UnInitializing..." << endl;
}

int TheAnswer()
{
    return 42;
}