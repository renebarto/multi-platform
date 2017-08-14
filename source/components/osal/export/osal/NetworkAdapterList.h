#pragma once

#include "osal/exports.h"

#include "osal/Unused.h"

#if defined(WIN_MSVC)
#include "osal/windows/NetworkAdapterList.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/NetworkAdapterList.h"
#elif defined(DARWIN)
#include "osal/darwin/NetworkAdapterList.h"
#elif defined(LINUX)
#include "osal/linux/NetworkAdapterList.h"
#endif
