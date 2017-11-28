#include <iomanip>
#include <iostream>

#include "staruml/Document.h"

using namespace std;
using namespace StarUML;

#if defined(UNICODE) || defined(_UNICODE)
int wmain(int argc, const wchar_t * argv[])
#else
int main(int UNUSED(argc), const char * UNUSED(argv[]))
#endif
{
    Document document;

    document.Serialize(cout);

    return EXIT_SUCCESS;
}
