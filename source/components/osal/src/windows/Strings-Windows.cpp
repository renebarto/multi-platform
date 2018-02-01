#include "osal/Strings.h"

WARNING_PUSH
WARNING_DISABLE(4668)
#include "windows.h"
WARNING_POP

using namespace OSAL;

WARNING_PUSH
WARNING_DISABLE(4242)
WARNING_DISABLE(4302)
WARNING_DISABLE(4365)
string OSAL::ToLower(const string & text)
{
    string result;
    for (size_t i = 0; i < text.length(); i++)
    {
        // This is dirty stuff. If the character pointer is a single charactor (high order word equals 0), CharLower converts a single character.
        // We then need to convert back to a single characters.
        result += reinterpret_cast<char>(CharLower(reinterpret_cast<LPTSTR>(text[i])));
    }
    return result;
}

string OSAL::ToUpper(const string & text)
{
    string result;
    for (size_t i = 0; i < text.length(); i++)
    {
        // This is dirty stuff. If the character pointer is a single charactor (high order word equals 0), CharUpper converts a single character.
        // We then need to convert back to a single characters.
        result += reinterpret_cast<char>(CharUpper(reinterpret_cast<LPTSTR>(text[i])));
    }
    return result;
}
WARNING_POP

