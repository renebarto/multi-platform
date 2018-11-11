#include "osal/PlatformDefines.h"

#if defined(LINUX)

#include "osal/Strings.h"

using namespace std;
using namespace OSAL;

string OSAL::Strings::ToLower(const string & text)
{
    string result;
    for (size_t i = 0; i < text.length(); i++)
    {
#if defined(UNICODE) || defined(_UNICODE)
        result += towlower(text[i]);
#else
        result += tolower(text[i]);
#endif
    }
    return result;
}

string OSAL::Strings::ToUpper(const string & text)
{
    string result;
    for (size_t i = 0; i < text.length(); i++)
    {
#if defined(UNICODE) || defined(_UNICODE)
        result += towupper(text[i]);
#else
        result += toupper(text[i]);
#endif
    }
    return result;
}

#endif // defined(LINUX)
