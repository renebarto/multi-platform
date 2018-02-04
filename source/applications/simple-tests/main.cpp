#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <osal/PlatformDefines.h>

using namespace std;

int main()
{
    cout << "bool:        " << sizeof(bool) << endl;
    cout << "char:        " << sizeof(char) << endl;
    cout << "wchar_t:     " << sizeof(wchar_t) << endl;
    cout << "short:       " << sizeof(short) << endl;
    cout << "int:         " << sizeof(int) << endl;
    cout << "long:        " << sizeof(long) << endl;
    cout << "long long:   " << sizeof(long long) << endl;
    cout << "float:       " << sizeof(float) << endl;
    cout << "double:      " << sizeof(double) << endl;
    cout << "long double: " << sizeof(long double) << endl;

    cout << "uint8_t:     " << sizeof(uint8_t) << endl;
    cout << "uint16_t:    " << sizeof(uint16_t) << endl;
    cout << "uint32_t:    " << sizeof(uint32_t) << endl;
    cout << "uint64_t:    " << sizeof(uint64_t) << endl;
    cout << "__int128_t:  " << sizeof(__int128_t) << endl;

    return EXIT_SUCCESS;
}