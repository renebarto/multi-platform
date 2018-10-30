#include "osal/PlatformDefines.h"

#if defined(WIN_MINGW)

#include "osal/Assert.h"

#include <cassert>

namespace OSAL
{

static AssertHandler * currentAssertHandler = nullptr;

void InternalAssertionHandler(bool expression, const char * expressionText, const char * file, int line, const char * func)
{
    if (!expression)
    {
        if (currentAssertHandler)
            currentAssertHandler(expression, expressionText, file, line, func);
        else
            _assert(expressionText, file, line);
    }
}

AssertHandler * SetAssertionHandler(AssertHandler * handler)
{
    AssertHandler * previousHandler = currentAssertHandler;
    currentAssertHandler = handler;
    return previousHandler;
}

} // namespace OSAL

#endif // defined(WIN_MINGW)
