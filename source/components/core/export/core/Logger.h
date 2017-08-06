#pragma once

#include <list>
#include <osal/OSAL.h>
WARNING_PUSH
WARNING_DISABLE(4265)
#include <mutex>
WARNING_POP
#include "core/ILogger.h"

namespace Core
{

class ILogListener;

class CORE_EXPORT Logger : public ILogger
{
public:
    Logger() = delete;
    Logger(const OSAL::String & domainName, LogLevel logLevelFilter);
    Logger(const Logger &) = delete;
    virtual ~Logger();

    Logger & operator = (const Logger &) = delete;

    void Error(const OSAL::String & componentName, const OSAL::String & message);
    void Error(const OSAL::String & componentName, const OSAL::String & message, const std::exception & exception);
    void Warn(const OSAL::String & componentName, const OSAL::String & message);
    void Warn(const OSAL::String & componentName, const OSAL::String & message, const std::exception & exception);
    void Message(const OSAL::String & componentName, const OSAL::String & message);
    void Info(const OSAL::String & componentName, const OSAL::String & message);
    void Debug(const OSAL::String & componentName, const OSAL::String & message);

    LogLevel GetLogLevelFilter();
    void SetLogLevelFilter(LogLevel logLevelFilter);

protected:
    typedef std::recursive_mutex Mutex;
    typedef std::lock_guard<Mutex> Lock;

    OSAL::String _domainName;
    LogHandlerInfo _originalLogHandler;
    Mutex _listenersGuard;

    void Log(const OSAL::String & componentName, LogLevel logLevel, const OSAL::String & message, const std::exception * exception = nullptr);
    void OnLog(const OSAL::String & domainName,
               const OSAL::String & componentName,
               LogLevel logLevel,
               const OSAL::String & message);

    static void Handler(const OSAL::String & domainName,
                        const OSAL::String & componentName,
                        LogLevel logLevel,
                        const OSAL::String & message,
                        void * userData);
};

} // namespace Core
