#include "osal/Strings.h"

using namespace std;
using namespace OSAL;
using namespace OSAL::Strings;

string OSAL::Strings::Trim(const string & value, const string & stripChars)
{
    const char * find = stripChars.c_str();

    size_t indexLeft = 0;
    while ((indexLeft < value.length()) && Strings::strchr(find, value[indexLeft]))
        ++indexLeft;
    size_t indexRight = value.length();
    while ((indexRight > indexLeft) && Strings::strchr(find, value[indexRight - 1]))
        --indexRight;
    return value.substr(indexLeft, indexRight - indexLeft);
}

string OSAL::Strings::TrimSpaces(const string & value)
{
    size_t indexLeft = 0;
    while ((indexLeft < value.length()) && isspace(value[indexLeft]))
        ++indexLeft;
    size_t indexRight = value.length();
    while ((indexRight > indexLeft) && isspace(value[indexRight - 1]))
        --indexRight;
    return value.substr(indexLeft, indexRight - indexLeft);
}

string OSAL::Strings::TrimLeft(const string & value, const string & stripChars)
{
    const char * find = stripChars.c_str();

    size_t indexLeft = 0;
    while ((indexLeft < value.length()) && OSAL::Strings::strchr(find, value[indexLeft]))
        ++indexLeft;
    return value.substr(indexLeft, value.length() - indexLeft);
}

string OSAL::Strings::TrimSpacesLeft(const string & value)
{
    size_t indexLeft = 0;
    while ((indexLeft < value.length()) && isspace(value[indexLeft]))
        ++indexLeft;
    return value.substr(indexLeft, value.length() - indexLeft);
}

string OSAL::Strings::TrimRight(const string & value, const string & stripChars)
{
    const char * find = stripChars.c_str();

    size_t indexRight = value.length();
    while ((indexRight > 0) && OSAL::Strings::strchr(find, value[indexRight - 1]))
        --indexRight;
    return value.substr(0, indexRight);
}

string OSAL::Strings::TrimSpacesRight(const string & value)
{
    size_t indexRight = value.length();
    while ((indexRight > 0) && isspace(value[indexRight - 1]))
        --indexRight;
    return value.substr(0, indexRight - 0);
}

bool OSAL::Strings::IsEqual(const string & lhs, const string & rhs)
{
    return (lhs == rhs);
}

bool OSAL::Strings::IsEqualIgnoreCase(const string & lhs, const string & rhs)
{
    if (lhs.size() != rhs.size())
        return false;

    size_t size = rhs.size();
    for (size_t n = 0; n < size; n++)
        if (tolower(lhs[n]) != tolower(rhs[n]))
            return false;

    return true;
}

bool OSAL::Strings::IsEqual(const char * lhs, const char * rhs)
{
    return OSAL::Strings::strcmp(lhs, rhs) == 0;
}

bool OSAL::Strings::IsEqualIgnoreCase(const char * lhs, const char * rhs)
{
    return OSAL::Strings::strcasecmp(lhs, rhs) == 0;
}

bool OSAL::Strings::IsEqual(const wchar_t * lhs, const wchar_t * rhs)
{
    return OSAL::Strings::strcmp(lhs, rhs) == 0;
}

bool OSAL::Strings::IsEqualIgnoreCase(const wchar_t * lhs, const wchar_t * rhs)
{
    return OSAL::Strings::strcasecmp(lhs, rhs) == 0;
}

string OSAL::Strings::Quote(const string & text)
{
    if ((text[0] == '\"') && (text[text.length() - 1] == '\"'))
    {
        return text;
    }
    return "\"" + text + "\"";
}

string OSAL::Strings::UnQuote(const string & text)
{
    if ((text.length() >= 2) && (text[0] == '\"') && (text[text.length() - 1] == '\"'))
    {
        return text.substr(1, text.length() - 2);
    }
    return text;
}
