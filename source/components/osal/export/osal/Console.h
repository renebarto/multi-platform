#pragma once

#include "osal/ConsoleBase.h"
#if defined(WIN_MSVC)
#include "osal/windows/Console.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/Console.h"
#elif defined(DARWIN)
#include "osal/darwin/Console.h"
#elif defined(LINUX)
#include "osal/linux/Console.h"
#endif

namespace OSAL {

typedef ConsoleBase<Char> Console;

} // namespace OSAL