#pragma once

#include "osal/exports.h"

#include "osal/Unused.h"

#if defined(WIN_MSVC)
#include "osal/windows/NetworkAdapter.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/NetworkAdapter.h"
#elif defined(DARWIN)
#include "osal/darwin/NetworkAdapter.h"
#elif defined(LINUX)
#include "osal/linux/NetworkAdapter.h"
#endif
