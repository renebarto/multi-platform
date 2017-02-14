#pragma once

#include <string>

namespace OSAL
{
#if defined(UNICODE) || defined(_UNICODE)
using Char=wchar_t;
#else
using Char=char;
#endif

using String=std::basic_string<Char>;

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
