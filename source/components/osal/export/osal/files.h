#pragma once

#if defined(WIN_MSVC)
#include "osal/windows/files.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/files.h"
#elif defined(DARWIN)
#include "osal/darwin/files.h"
#elif defined(LINUX)
#include "osal/linux/files.h"
#endif
