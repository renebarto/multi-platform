#include "osal/PlatformDefines.h"

#if defined(LINUX)

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
            __assert_fail(expressionText, file, line, func);
    }
}

AssertHandler * SetAssertionHandler(AssertHandler * handler)
{
    AssertHandler * previousHandler = currentAssertHandler;
    currentAssertHandler = handler;
    return previousHandler;
}

} // namespace OSAL

#endif // defined(LINUX)
