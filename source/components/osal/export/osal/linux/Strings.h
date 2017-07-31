#pragma once

#include <cstring>

namespace OSAL {
namespace Strings {

inline int strcmp(const char * lhs, const char * rhs)
{
    return ::strcmp(lhs, rhs);
}
inline int strcmp(const wchar_t * lhs, const wchar_t * rhs)
{
    return ::wcscmp(lhs, rhs);
}
inline int strcmp(const char * lhs, const char * rhs, size_t size)
{
    return ::strncmp(lhs, rhs, size);
}
inline int strcmp(const wchar_t * lhs, const wchar_t * rhs, size_t size)
{
    return ::wcsncmp(lhs, rhs, size);
}
inline int strcasecmp(const char * lhs, const char * rhs)
{
    return ::strcasecmp(lhs, rhs);
}
inline int strcasecmp(const wchar_t * lhs, const wchar_t * rhs)
{
    return ::wcscasecmp(lhs, rhs);
}
inline int strcasecmp(const char * lhs, const char * rhs, size_t size)
{
    return ::strncasecmp(lhs, rhs, size);
}
inline int strcasecmp(const wchar_t * lhs, const wchar_t * rhs, size_t size)
{
    return ::wcsncasecmp(lhs, rhs, size);
}
inline const char * strchr(const char * str, const char character)
{
    return ::strchr(str, character);
}
inline const wchar_t * strchr(const wchar_t * str, const wchar_t character)
{
    return ::wcschr(str, character);
}
inline const size_t strlen(const char * str)
{
	return ::strlen(str);
}
inline const size_t strlen(const wchar_t * str)
{
	return ::wcslen(str);
}
inline int toupper(char c)
{
	return ::toupper(c);
}
inline int toupper(wchar_t c)
{
	return ::towupper(c);
}
inline int tolower(char c)
{
	return ::tolower(c);
}
inline int tolower(wchar_t c)
{
	return ::towlower(c);
}

} // namespace Strings
} // namespace OSAL