#pragma once

#include <cstring>

namespace OSAL {
namespace Strings {

inline int StrCmp(const char * lhs, const char * rhs)
{
    return ::strcmp(lhs, rhs);
}
inline int StrCmp(const wchar_t * lhs, const wchar_t * rhs)
{
    return ::wcscmp(lhs, rhs);
}
inline int StrCaseCmp(const char * lhs, const char * rhs)
{
    return ::strcasecmp(lhs, rhs);
}
inline int StrCaseCmp(const wchar_t * lhs, const wchar_t * rhs)
{
    return ::wcscasecmp(lhs, rhs);
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