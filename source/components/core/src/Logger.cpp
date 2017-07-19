#include <sstream>
#include <iostream>
#include "core/Logger.h"
#include "core/DateTime.h"
#include "core/ILogListener.h"

using namespace std;
using namespace Core;

static const string DefaultComponent = "";

Logger::Logger(const string & domainName, LogLevel logLevelFilter)
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

void Logger::Error(const std::string & componentName, const std::string & message)
{
    Log(componentName, LogLevel::Error, message);
}

void Logger::Error(const std::string & componentName, const std::string & message, const std::exception & exception)
{
    Log(componentName, LogLevel::Error, message, &exception);
}

void Logger::Warn(const std::string & componentName, const std::string & message)
{
    Log(componentName, LogLevel::Warning, message);
}

void Logger::Warn(const std::string & componentName, const std::string & message, const std::exception & exception)
{
    Log(componentName, LogLevel::Warning, message, &exception);
}

void Logger::Message(const std::string & componentName, const std::string & message)
{
    Log(componentName, LogLevel::Message, message);
}

void Logger::Info(const std::string & componentName, const std::string & message)
{
    Log(componentName, LogLevel::Info, message);
}

void Logger::Debug(const std::string & componentName, const std::string & message)
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

void Logger::Log(const string & componentName, LogLevel logLevel, const string & message, const exception * exception /*= nullptr */)
{
    string fullMessage = message;
    if (exception != nullptr)
    {
        fullMessage += " (";
        fullMessage += exception->what();
        fullMessage += ")";
    }
    LogHandler::Log(_domainName, componentName, logLevel, fullMessage);
}

void Logger::OnLog(const string & domainName,
                   const string & componentName,
                   LogLevel logLevel,
                   const string & message)
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

void Logger::Handler(const string & domainName,
                     const string & componentName,
                     LogLevel logLevel,
                     const string & message,
                     void * userData)
{
    Logger * pThis = reinterpret_cast<Logger *>(userData);
    pThis->OnLog(domainName, componentName, logLevel, message);
}

