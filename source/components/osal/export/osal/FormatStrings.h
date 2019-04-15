#pragma once

#include "PlatformDefines.h"
#if defined(WIN_MSVC)
#include "osal/windows/FormatStrings.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/FormatStrings.h"
#elif defined(DARWIN)
#include "osal/darwin/FormatStrings.h"
#elif defined(LINUX)
#include "osal/linux/FormatStrings.h"
#endif
