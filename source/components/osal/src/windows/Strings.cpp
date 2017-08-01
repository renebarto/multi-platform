#include "osal/Strings.h"
#include "osal/OSAL.h"

WARNING_PUSH
WARNING_DISABLE(4668)
#include "windows.h"
WARNING_POP

using namespace OSAL;
using namespace OSAL::Strings;

WARNING_PUSH
WARNING_DISABLE(4242)
WARNING_DISABLE(4365)
String OSAL::ToLower(const String & text)
{
    String result;
    for (size_t i = 0; i < text.length(); i++)
    {
        result += CharLower(reinterpret_cast<LPTSTR>(text[i]));
    }
    return result;
}

String OSAL::ToUpper(const String & text)
{
    String result;
    for (size_t i = 0; i < text.length(); i++)
    {
        result += CharUpper(reinterpret_cast<LPTSTR>(text[i]));
    }
    return result;
}
WARNING_POP

