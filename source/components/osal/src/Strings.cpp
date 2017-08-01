#include "osal/Strings.h"
#include "osal/OSAL.h"

using namespace OSAL;
using namespace OSAL::Strings;

String OSAL::Trim(const String & value, const String & stripChars)
{
    const Char * find = stripChars.c_str();

    size_t indexLeft = 0;
    while ((indexLeft < value.length()) && Strings::strchr(find, value[indexLeft]))
        ++indexLeft;
    size_t indexRight = value.length();
    while ((indexRight > indexLeft) && Strings::strchr(find, value[indexRight - 1]))
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
    while ((indexLeft < value.length()) && OSAL::Strings::strchr(find, value[indexLeft]))
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
    while ((indexRight > 0) && OSAL::Strings::strchr(find, value[indexRight - 1]))
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
    return OSAL::Strings::strcmp(lhs, rhs) == 0;
}

bool OSAL::IsEqualIgnoreCase(const char * lhs, const char * rhs)
{
    return OSAL::Strings::strcasecmp(lhs, rhs) == 0;
}

bool OSAL::IsEqual(const wchar_t * lhs, const wchar_t * rhs)
{
    return OSAL::Strings::strcmp(lhs, rhs) == 0;
}

bool OSAL::IsEqualIgnoreCase(const wchar_t * lhs, const wchar_t * rhs)
{
    return OSAL::Strings::strcasecmp(lhs, rhs) == 0;
}

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
