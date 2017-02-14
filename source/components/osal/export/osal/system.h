#pragma once

#if defined(WIN_MSVC)
#include "osal/windows/system.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/system.h"
#elif defined(DARWIN)
#include "osal/darwin/system.h"
#elif defined(LINUX)
#include "osal/linux/system.h"
#endif
