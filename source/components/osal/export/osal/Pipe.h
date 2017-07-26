#pragma once

#include <osal/Exports.h>

#if defined(WIN_MSVC)
#include "osal/windows/Pipe.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/Pipe.h"
#elif defined(DARWIN)
#include "osal/darwin/Pipe.h"
#elif defined(LINUX)
#include "osal/linux/Pipe.h"
#endif
