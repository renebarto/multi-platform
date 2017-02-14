#pragma once

#if defined(WIN_MSVC)
#include "osal/windows/time.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/time.h"
#elif defined(DARWIN)
#include "osal/darwin/time.h"
#elif defined(LINUX)
#include "osal/linux/time.h"
#endif
