#pragma once

#include <codecvt>
#include <locale>
#include <string>
#include <osal/osal.h>

namespace OSAL
{
OSAL_EXPORT std::string Trim(const std::string & value, const std::string & stripChars);

OSAL_EXPORT std::string TrimSpaces(const std::string & value);

OSAL_EXPORT std::string TrimLeft(const std::string & value, const std::string & stripChars);

OSAL_EXPORT std::string TrimSpacesLeft(const std::string & value);

OSAL_EXPORT std::string TrimRight(const std::string & value, const std::string & stripChars);

OSAL_EXPORT std::string TrimSpacesRight(const std::string & value);

OSAL_EXPORT bool IsEqual(const std::string & lhs, const std::string & rhs);

OSAL_EXPORT bool IsEqualIgnoreCase(const std::string & lhs, const std::string & rhs);

OSAL_EXPORT bool IsEqual(const char * lhs, const char * rhs);

OSAL_EXPORT bool IsEqualIgnoreCase(const char * lhs, const char * rhs);

OSAL_EXPORT bool IsEqual(const wchar_t * lhs, const wchar_t * rhs);

OSAL_EXPORT bool IsEqualIgnoreCase(const wchar_t * lhs, const wchar_t * rhs);

OSAL_EXPORT std::string ToLower(const std::string & text);

OSAL_EXPORT std::string ToUpper(const std::string & text);

OSAL_EXPORT std::string Quote(const std::string & text);

OSAL_EXPORT std::string UnQuote(const std::string & text);

inline std::wstring StringToWString(const std::string & value)
{
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;

    return converter.from_bytes(value);
}

inline std::string WStringToString(const std::wstring & value)
{
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;

    return converter.to_bytes(value);
}

inline std::string ToString(const std::string & value)
{
    return value;
}

inline std::string ToString(const std::wstring & value)
{
    return WStringToString(value);
}

inline std::wstring ToWideString(const std::string & value)
{
#if defined(UNICODE) || defined(_UNICODE)
    return value;
#else
    return StringToWString(value);
#endif
}

inline std::string ToNarrowString(const std::string & value)
{
#if defined(UNICODE) || defined(_UNICODE)
    return WStringToString(value);
#else
    return value;
#endif
}

} // namespace OSAL

#if defined(WIN_MSVC)
#include "osal/windows/Strings.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/Strings.h"
#elif defined(DARWIN)
#include "osal/darwin/Strings.h"
#elif defined(LINUX)
#include "osal/linux/Strings.h"
#endif
