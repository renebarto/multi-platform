#include <osal/osal.h>
WARNING_PUSH_N(0)
WARNING_DISABLE(4365)
WARNING_DISABLE(5045)
WARNING_POP
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
    for (int index = 1; index < argc; index++)
    {
        cout << argv[index] << endl;
        cerr << argv[index] << endl;
    }
    string buffer;
    while (!cin.eof() && getline(cin, buffer))
    {
        cout << buffer << endl;
    }
}

