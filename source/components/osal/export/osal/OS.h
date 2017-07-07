#pragma once

#if defined(WIN_MSVC)
#include "osal/windows/OS.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/OS.h"
#elif defined(DARWIN)
#include "osal/darwin/OS.h"
#elif defined(LINUX)
#include <osal/linux/OS.h>
#endif
