#pragma once

#include "osal/Console.h"
#include "core/ILogListener.h"

namespace Core
{

class CORE_EXPORT ConsoleLogger : public ILogListener
{
public:
    ConsoleLogger() = delete;
    ConsoleLogger(ILogger & logger, OSAL::Console & console, bool autoRegister = true);
    ConsoleLogger(const ConsoleLogger &) = delete;
    virtual ~ConsoleLogger();

    ConsoleLogger & operator = (const ConsoleLogger &) = delete;

    void Register();
    void UnRegister();

    virtual void Log(const DateTime & timestamp,
                     const OSAL::String & domainName,
                     const OSAL::String & componentName,
                     LogLevel logLevel,
                     const OSAL::String & message);

protected:
    ILogger & logger;
    OSAL::Console & console;
};

} // namespace Core

