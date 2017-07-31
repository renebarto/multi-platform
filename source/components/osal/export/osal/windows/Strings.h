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
    return ::_stricmp(lhs, rhs);
}
inline int strcasecmp(const wchar_t * lhs, const wchar_t * rhs)
{
    return ::_wcsicmp(lhs, rhs);
}
inline int strcasecmp(const char * lhs, const char * rhs, size_t size)
{
    return ::_strnicmp(lhs, rhs, size);
}
inline int strcasecmp(const wchar_t * lhs, const wchar_t * rhs, size_t size)
{
    return ::_wcsnicmp(lhs, rhs, size);
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
inline long strtol(const char * nptr, char ** endptr, int base)
{
	return ::strtol(nptr, endptr, base);
}
inline long strtol(const wchar_t * nptr, wchar_t ** endptr, int base)
{
	return ::wcstol(nptr, endptr, base);
}
inline long long strtoll(const char * nptr, char ** endptr, int base)
{
	return ::strtoll(nptr, endptr, base);
}
inline long long strtoll(const wchar_t * nptr, wchar_t ** endptr, int base)
{
	return ::wcstoll(nptr, endptr, base);
}
inline unsigned long long strtoull(const char * nptr, char ** endptr, int base)
{
	return ::strtoull(nptr, endptr, base);
}
inline unsigned long long strtoull(const wchar_t * nptr, wchar_t ** endptr, int base)
{
	return ::wcstoull(nptr, endptr, base);
}

} // namespace Strings
} // namespace OSAL