#pragma once

#include <codecvt>
#include <locale>
#include <string>
#include <osal/Exports.h>

namespace OSAL
{
#if defined(UNICODE) || defined(_UNICODE)
using Char=wchar_t;
#else
using Char=char;
#endif

using String=std::basic_string<Char>;

OSAL_EXPORT String Trim(const String & value, const String & stripChars);
OSAL_EXPORT String TrimSpaces(const String & value);
OSAL_EXPORT String TrimLeft(const String & value, const String & stripChars);
OSAL_EXPORT String TrimSpacesLeft(const String & value);
OSAL_EXPORT String TrimRight(const String & value, const String & stripChars);
OSAL_EXPORT String TrimSpacesRight(const String & value);
OSAL_EXPORT bool IsEqual(const String & lhs, const String & rhs);
OSAL_EXPORT bool IsEqualIgnoreCase(const String & lhs, const String & rhs);
OSAL_EXPORT bool IsEqual(const char * lhs, const char * rhs);
OSAL_EXPORT bool IsEqualIgnoreCase(const char * lhs, const char * rhs);
OSAL_EXPORT bool IsEqual(const wchar_t * lhs, const wchar_t * rhs);
OSAL_EXPORT bool IsEqualIgnoreCase(const wchar_t * lhs, const wchar_t * rhs);
OSAL_EXPORT String ToLower(const String & text);
OSAL_EXPORT String ToUpper(const String & text);
OSAL_EXPORT String Quote(const String & text);
OSAL_EXPORT String UnQuote(const String & text);

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

inline OSAL::String ToString(const std::string & value)
{
#if defined(UNICODE) || defined(_UNICODE)
	return StringToWString(value);
#else
	return value;
#endif
}

inline OSAL::String ToString(const std::wstring & value)
{
#if defined(UNICODE) || defined(_UNICODE)
	return value;
#else
	return WStringToString(value);
#endif
}

inline std::wstring ToWideString(const OSAL::String & value)
{
#if defined(UNICODE) || defined(_UNICODE)
	return value;
#else
	return StringToWString(value);
#endif
}

inline std::string ToNarrowString(const OSAL::String & value)
{
#if defined(UNICODE) || defined(_UNICODE)
	return WStringToString(value);
#else
	return value;
#endif
}

} // namespace OSAL

#if defined (UNICODE) || defined(_UNICODE)
#define _(a) L##a
#else
#define _(a) a
#endif

#if defined(WIN_MSVC)
#include "osal/windows/Strings.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/Strings.h"
#elif defined(DARWIN)
#include "osal/darwin/Strings.h"
#elif defined(LINUX)
#include "osal/linux/Strings.h"
#endif
