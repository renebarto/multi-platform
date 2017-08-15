#pragma once

#include "osal/exports.h"

#include "osal/Unused.h"

#if defined(WIN_MSVC)
#include "osal/windows/NetworkAddress.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/NetworkAddress.h"
#elif defined(DARWIN)
#include "osal/darwin/NetworkAddress.h"
#elif defined(LINUX)
#include "osal/linux/NetworkAddress.h"
#endif
