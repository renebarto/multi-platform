#pragma once

#include <cstring>
#include <wchar.h>
#include <wctype.h>

namespace OSAL {
namespace Strings {

inline int wcscasecmp(const wchar_t * s1, const wchar_t * s2)
{
    while ((*s1 != '\0') && towlower(static_cast<wint_t>(*s1)) == towlower(static_cast<wint_t>(*s2)))
    {
        s1++;
        s2++;
    }
    return towlower(static_cast<wint_t>(*s1)) - towlower(static_cast<wint_t>(*s2));
}

inline int wcsncasecmp(const wchar_t * s1, const wchar_t * s2, size_t size)
{
    size_t count = size;
    while ((count-- > 0) && (*s1 != '\0') && towlower(static_cast<wint_t>(*s1)) == towlower(static_cast<wint_t>(*s2)))
    {
        s1++;
        s2++;
    }
    return towlower(static_cast<wint_t>(*s1)) - towlower(static_cast<wint_t>(*s2));
}

inline int StrCmp(const char * lhs, const char * rhs)
{
    return ::strcmp(lhs, rhs);
}
inline int StrCmp(const wchar_t * lhs, const wchar_t * rhs)
{
    return ::wcscmp(lhs, rhs);
}
inline int StrCmp(const char * lhs, const char * rhs, size_t size)
{
    return ::strncmp(lhs, rhs, size);
}
inline int StrCmp(const wchar_t * lhs, const wchar_t * rhs, size_t size)
{
    return ::wcsncmp(lhs, rhs, size);
}
inline int StrCaseCmp(const char * lhs, const char * rhs)
{
    return ::strcasecmp(lhs, rhs);
}
inline int StrCaseCmp(const wchar_t * lhs, const wchar_t * rhs)
{
    return wcscasecmp(lhs, rhs);
}
inline int StrCaseCmp(const char * lhs, const char * rhs, size_t size)
{
    return ::strncasecmp(lhs, rhs, size);
}
inline int StrCaseCmp(const wchar_t * lhs, const wchar_t * rhs, size_t size)
{
    return wcsncasecmp(lhs, rhs, size);
}
inline const char * StrChr(const char * str, const char character)
{
    return ::strchr(str, character);
}
inline const wchar_t * StrChr(const wchar_t * str, const wchar_t character)
{
    return ::wcschr(str, character);
}

} // namespace Strings
} // namespace OSAL