#pragma once

#include <string>
#include <osal/exports.h>

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

} // namespace OSAL

inline OSAL::String _(const OSAL::String & value) { return value; }
inline OSAL::String _(const OSAL::Char * value) { return value; }
inline OSAL::Char _(const OSAL::Char value) { return value; }

#if defined(WIN_MSVC)
#include "osal/windows/strings.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/strings.h"
#elif defined(DARWIN)
#include "osal/darwin/strings.h"
#elif defined(LINUX)
#include "osal/linux/strings.h"
#endif
