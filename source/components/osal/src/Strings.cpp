#include "osal/Strings.h"

#if WIN_MSVC
#pragma warning(disable : 4668)
#endif

#include "windows.h"

#if WIN_MSVC
#pragma warning(default : 4668)
#endif

using namespace OSAL;
using namespace OSAL::Strings;

String OSAL::Trim(const String & value, const String & stripChars)
{
    const Char * find = stripChars.c_str();

    size_t indexLeft = 0;
    while ((indexLeft < value.length()) && StrChr(find, value[indexLeft]))
        ++indexLeft;
    size_t indexRight = value.length();
    while ((indexRight > indexLeft) && StrChr(find, value[indexRight - 1]))
        --indexRight;
    return value.substr(indexLeft, indexRight - indexLeft);
}

String OSAL::TrimSpaces(const String & value)
{
    size_t indexLeft = 0;
    while ((indexLeft < value.length()) && isspace(value[indexLeft]))
        ++indexLeft;
    size_t indexRight = value.length();
    while ((indexRight > indexLeft) && isspace(value[indexRight - 1]))
        --indexRight;
    return value.substr(indexLeft, indexRight - indexLeft);
}

String OSAL::TrimLeft(const String & value, const String & stripChars)
{
    const Char * find = stripChars.c_str();

    size_t indexLeft = 0;
    while ((indexLeft < value.length()) && StrChr(find, value[indexLeft]))
        ++indexLeft;
    return value.substr(indexLeft, value.length() - indexLeft);
}

String OSAL::TrimSpacesLeft(const String & value)
{
    size_t indexLeft = 0;
    while ((indexLeft < value.length()) && isspace(value[indexLeft]))
        ++indexLeft;
    return value.substr(indexLeft, value.length() - indexLeft);
}

String OSAL::TrimRight(const String & value, const String & stripChars)
{
    const Char * find = stripChars.c_str();

    size_t indexRight = value.length();
    while ((indexRight > 0) && StrChr(find, value[indexRight - 1]))
        --indexRight;
    return value.substr(0, indexRight);
}

String OSAL::TrimSpacesRight(const String & value)
{
    size_t indexRight = value.length();
    while ((indexRight > 0) && isspace(value[indexRight - 1]))
        --indexRight;
    return value.substr(0, indexRight - 0);
}

bool OSAL::IsEqual(const String & lhs, const String & rhs)
{
    return (lhs == rhs);
}

bool OSAL::IsEqualIgnoreCase(const String & lhs, const String & rhs)
{
    if (lhs.size() != rhs.size())
        return false;

    size_t size = rhs.size();
    for (size_t n = 0; n < size; n++)
        if (tolower(lhs[n]) != tolower(rhs[n]))
            return false;

    return true;
}

bool OSAL::IsEqual(const char * lhs, const char * rhs)
{
    return OSAL::Strings::StrCmp(lhs, rhs) == 0;
}

bool OSAL::IsEqualIgnoreCase(const char * lhs, const char * rhs)
{
    return OSAL::Strings::StrCaseCmp(lhs, rhs) == 0;
}

bool OSAL::IsEqual(const wchar_t * lhs, const wchar_t * rhs)
{
    return OSAL::Strings::StrCmp(lhs, rhs) == 0;
}

bool OSAL::IsEqualIgnoreCase(const wchar_t * lhs, const wchar_t * rhs)
{
    return OSAL::Strings::StrCaseCmp(lhs, rhs) == 0;
}

#pragma warning(disable : 4242 4365)
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
#pragma warning(default : 4242 4365)

String OSAL::Quote(const String & text)
{
    if ((text[0] == '\"') && (text[text.length() - 1] == '\"'))
    {
        return text;
    }
    return _("\"") + text + _("\"");
}

String OSAL::UnQuote(const String & text)
{
    if ((text.length() >= 2) && (text[0] == '\"') && (text[text.length() - 1] == '\"'))
    {
        return text.substr(1, text.length() - 2);
    }
    return text;
}
