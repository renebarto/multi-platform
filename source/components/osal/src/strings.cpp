#include "osal/strings.h"

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
