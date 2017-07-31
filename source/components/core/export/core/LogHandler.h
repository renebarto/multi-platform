#pragma once

#include <string>
#include <iostream>
#include <map>
#include <osal/OSAL.h>
WARNING_PUSH
WARNING_DISABLE(4265)
#include <mutex>
WARNING_POP
#include "osal/FlagOperators.h"
#include "core/Util.h"

namespace Core
{

enum class LogLevel : uint8_t
{
    Fatal = 1 << 1,
    Recursion = 1 << 2,
    Error = 1 << 3,
    Warning = 1 << 4,
    Message = 1 << 5,
    Info = 1 << 6,
    Debug = 1 << 7,
    All = Error | Warning | Message | Info | Debug,
	Mask = Error | Warning | Message | Info | Debug,
    None = 0x00,
};

DEFINE_FLAG_OPERATORS(LogLevel, uint8_t);

std::basic_ostream<OSAL::Char> & operator << (std::basic_ostream<OSAL::Char> & stream, LogLevel logLevel);

typedef void LogHandlerFunction(const OSAL::String & domainName,
                                const OSAL::String & componentName,
                                LogLevel logLevel,
                                const OSAL::String & message,
                                void * userData);

struct LogHandlerInfo
{
    LogHandlerInfo() :
        _logLevelFilter(LogLevel::All),
        _handler(nullptr),
        _userData(nullptr)
    {
    }
    LogHandlerInfo(LogLevel logLevelFilter, LogHandlerFunction * handler, void * userData) :
        _logLevelFilter(logLevelFilter),
        _handler(handler),
        _userData(userData)
    {
    }
    bool IsFilteredOut(LogLevel logLevel)
    {
        return (logLevel & this->_logLevelFilter) == LogLevel::None;
    }
    void Set(LogLevel logLevelFilter, LogHandlerFunction * handler, void * userData)
    {
        this->_logLevelFilter = logLevelFilter;
        this->_handler = handler;
        this->_userData = userData;
    }
    void Set(const LogHandlerInfo & other)
    {
        this->_logLevelFilter = other._logLevelFilter;
        this->_handler = other._handler;
        this->_userData = other._userData;
    }

    LogLevel _logLevelFilter;
    LogHandlerFunction * _handler;
    void * _userData;
};

class LogHandler
{
public:
    static const OSAL::String DefaultDomain;
    static LogHandlerInfo Set(const OSAL::String & domainName,
                              LogLevel logLevelFilter,
                              LogHandlerFunction * newHandler,
                              void * userData);
    static LogHandlerInfo Set(const OSAL::String & domainName, const LogHandlerInfo & handlerInfo);
    static void Reset(const OSAL::String & domainName);
    static LogLevel GetLogLevelFilter(const OSAL::String & domainName);
    static void SetLogLevelFilter(const OSAL::String & domainName, LogLevel logLevelFilter);

    static void Log(const OSAL::String & domainName,
                    const OSAL::String & componentName,
                    LogLevel logLevel,
                    const OSAL::String & message);
    static bool IsFilteredOut(LogLevel logLevel);

private:
    typedef std::recursive_mutex Mutex;
    typedef std::lock_guard<Mutex> Lock;

    static LogHandlerInfo _defaultLogHandlerInfo;
    static std::map<OSAL::String, LogHandlerInfo> _logHandlerInfo;
    static Mutex _guard;

    static LogHandlerInfo * FindInfo(const OSAL::String & domainName);
    static LogHandlerInfo * FindOrCreateInfo(const OSAL::String & domainName);

    static void DefaultLogHandler(const OSAL::String & domainName,
                                  const OSAL::String & componentName,
                                  LogLevel logLevel,
                                  const OSAL::String & message,
                                  void * userData);
};

} // namespace Core
