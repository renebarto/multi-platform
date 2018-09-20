#pragma once

#include "osal/exports.h"
#if defined(WIN_MSVC)
#include "osal/windows/Exception.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/Exception.h"
#elif defined(DARWIN)
#include "osal/darwin/Exception.h"
#elif defined(LINUX)
#include "osal/linux/Exception.h"
#endif
