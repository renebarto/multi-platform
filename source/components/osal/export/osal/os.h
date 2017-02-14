#pragma once

#if defined(WIN_MSVC)
#include "osal/windows/os.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/os.h"
#elif defined(DARWIN)
#include "osal/darwin/os.h"
#elif defined(LINUX)
#include "osal/linux/os.h"
#endif
