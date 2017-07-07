#pragma once

#if defined(WIN_MSVC)
#include "osal/windows/Path.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/Path.h"
#elif defined(DARWIN)
#include "osal/darwin/Path.h"
#elif defined(LINUX)
#include "osal/linux/Path.h"
#endif
