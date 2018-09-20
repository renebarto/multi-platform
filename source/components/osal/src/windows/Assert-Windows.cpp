#include "osal/PlatformDefines.h"

#if defined(WIN_MSVC)

#include "osal/Assert.h"

#include <string>
#include <cassert>
#define _WIN32_WINNT_WINTHRESHOLD           0x0601
#define NTDDI_WIN7SP1                       0x0601010
#include <windows.h>

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
        {
            char lineNumberText[10];
            _itoa(line, lineNumberText, 10);
            std::string output = std::string(file) + ": " + lineNumberText + ": " + func + ": " + expressionText;
            OutputDebugStringA(output.c_str());
        }
    }
}

AssertHandler * SetAssertionHandler(AssertHandler * handler)
{
    AssertHandler * previousHandler = currentAssertHandler;
    currentAssertHandler = handler;
    return previousHandler;
}

} // namespace OSAL

#endif // defined(WIN_MSVC)
