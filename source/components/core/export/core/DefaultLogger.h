#pragma once

#include "core/Exports.h"
#include <list>
#include <osal/OSAL.h>
WARNING_PUSH
WARNING_DISABLE(4265)
#include <mutex>
WARNING_POP
#include "core/Logger.h"

namespace Core
{

class ILogListener;

class CORE_EXPORT DefaultLogger : public Logger
{
public:
    DefaultLogger() = delete;
    DefaultLogger(LogLevel logLevelFilter)
        : Logger(LogHandler::DefaultDomain, logLevelFilter)
    {
    }
    DefaultLogger(const DefaultLogger &) = delete;

    DefaultLogger & operator = (const DefaultLogger &) = delete;
};

extern CORE_EXPORT DefaultLogger & TheLogger();

} // namespace Core
