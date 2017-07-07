#pragma once

#include "osal/Exports.h"

#include "osal/Unused.h"

#if defined(WIN_MSVC)
#include "osal/windows/OSAL.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/OSAL.h"
#elif defined(DARWIN)
#include "osal/darwin/OSAL.h"
#elif defined(LINUX)
#include "osal/linux/OSAL.h"
#endif

#include "osal/OS.h"
#include "osal/Files.h"
#include "osal/Path.h"
#include "osal/Strings.h"
#include "osal/System.h"
#include "osal/Time.h"
