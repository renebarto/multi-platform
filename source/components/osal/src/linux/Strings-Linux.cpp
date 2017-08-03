#include "osal/Strings.h"
#include "osal/OSAL.h"

using namespace OSAL;
using namespace OSAL::Strings;

String OSAL::ToLower(const String & text)
{
    String result;
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

String OSAL::ToUpper(const String & text)
{
    String result;
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
