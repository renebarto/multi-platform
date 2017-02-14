#pragma once

#if defined(WIN_MSVC)
#include "osal/windows/path.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/path.h"
#elif defined(DARWIN)
#include "osal/darwin/path.h"
#elif defined(LINUX)
#include "osal/linux/path.h"
#endif
