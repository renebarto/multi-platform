#pragma once

#include "osal/exports.h"

namespace OSAL
{
    using AssertHandler = void (bool expression, const char * expressionText, const char * file, int line, const char * func);

    extern OSAL_EXPORT void InternalAssertionHandler(bool expression, const char * expressionText, const char * file, int line, const char * func);
    extern OSAL_EXPORT AssertHandler * SetAssertionHandler(AssertHandler * handler);

} // namespace OSAL

#define ASSERT(expression) \
OSAL::InternalAssertionHandler(expression, #expression, __FILE__, __LINE__, __func__)
