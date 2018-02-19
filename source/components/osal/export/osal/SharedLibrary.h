#pragma once

#include "osal/exports.h"
#if defined(WIN_MSVC)
#include "osal/windows/SharedLibrary.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/SharedLibrary.h"
#elif defined(DARWIN)
#include "osal/darwin/SharedLibrary.h"
#elif defined(LINUX)
#include "osal/linux/SharedLibrary.h"
#endif
