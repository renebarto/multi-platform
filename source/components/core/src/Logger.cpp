#include <sstream>
#include <iostream>
#include "core/Logger.h"
#include "core/DateTime.h"
#include "core/ILogListener.h"

using namespace std;
using namespace Core;

static const OSAL::String DefaultComponent = _("");

Logger::Logger(const OSAL::String & domainName, LogLevel logLevelFilter)
    : _domainName(domainName)
{
    _originalLogHandler = LogHandler::Set(domainName,
                                         logLevelFilter | LogLevel::Fatal | LogLevel::Recursion,
                                         Logger::Handler, this);
}

Logger::~Logger()
{
    LogHandler::Set(_domainName, _originalLogHandler);
}

void Logger::Error(const OSAL::String & componentName, const OSAL::String & message)
{
    Log(componentName, LogLevel::Error, message);
}

void Logger::Error(const OSAL::String & componentName, const OSAL::String & message, const std::exception & exception)
{
    Log(componentName, LogLevel::Error, message, &exception);
}

void Logger::Warn(const OSAL::String & componentName, const OSAL::String & message)
{
    Log(componentName, LogLevel::Warning, message);
}

void Logger::Warn(const OSAL::String & componentName, const OSAL::String & message, const std::exception & exception)
{
    Log(componentName, LogLevel::Warning, message, &exception);
}

void Logger::Message(const OSAL::String & componentName, const OSAL::String & message)
{
    Log(componentName, LogLevel::Message, message);
}

void Logger::Info(const OSAL::String & componentName, const OSAL::String & message)
{
    Log(componentName, LogLevel::Info, message);
}

void Logger::Debug(const OSAL::String & componentName, const OSAL::String & message)
{
    Log(componentName, LogLevel::Debug, message);
}

LogLevel Logger::GetLogLevelFilter()
{
    return LogHandler::GetLogLevelFilter(_domainName);
}

void Logger::SetLogLevelFilter(LogLevel logLevelFilter)
{
    LogHandler::SetLogLevelFilter(_domainName, logLevelFilter);
}

void Logger::Log(const OSAL::String & componentName, LogLevel logLevel, const OSAL::String & message, const exception * exception /*= nullptr */)
{
    OSAL::String fullMessage = message;
    if (exception != nullptr)
    {
        fullMessage += _(" (");
        fullMessage += OSAL::ToString(exception->what());
        fullMessage += _(")");
    }
    LogHandler::Log(_domainName, componentName, logLevel, fullMessage);
}

void Logger::OnLog(const OSAL::String & domainName,
                   const OSAL::String & componentName,
                   LogLevel logLevel,
                   const OSAL::String & message)
{
    lock_guard<recursive_mutex> lock(_listenersGuard);
    const DateTime & timestamp = DateTime::NowLocal();
    lock_guard<recursive_mutex> listenersLock(GetMutex());
    list<ILogListener *>::iterator it = begin();
    while (it != end())
    {
        (*it)->Log(timestamp, domainName, componentName, logLevel, message);
        ++it;
    }
}

void Logger::Handler(const OSAL::String & domainName,
                     const OSAL::String & componentName,
                     LogLevel logLevel,
                     const OSAL::String & message,
                     void * userData)
{
    Logger * pThis = reinterpret_cast<Logger *>(userData);
    pThis->OnLog(domainName, componentName, logLevel, message);
}

