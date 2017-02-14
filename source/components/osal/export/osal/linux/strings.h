#pragma once

#include <cstring>

namespace OSAL {
namespace Strings {

inline int StrCmp(const char * lhs, const char * rhs)
{
    return ::strcmp(lhs, rhs);
}
inline int WcsCmp(const wchar_t * lhs, const wchar_t * rhs)
{
    return ::wcscmp(lhs, rhs);
}
inline int StrCaseCmp(const char * lhs, const char * rhs)
{
    return ::strcasecmp(lhs, rhs);
}
inline int WcsCaseCmp(const wchar_t * lhs, const wchar_t * rhs)
{
    return ::wcscasecmp(lhs, rhs);
}

} // namespace Strings
} // namespace OSAL