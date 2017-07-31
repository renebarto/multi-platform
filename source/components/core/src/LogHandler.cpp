#include "core/LogHandler.h"

using namespace std;
using namespace Core;

basic_ostream<OSAL::Char> & Core::operator << (basic_ostream<OSAL::Char> & stream, Core::LogLevel logLevel)
{
    OSAL::String str;
    if ((logLevel & Core::LogLevel::Fatal) != Core::LogLevel::None)
    {
        str += _("Fatal");
    }
    if ((logLevel & Core::LogLevel::Recursion) != Core::LogLevel::None)
    {
        if (!str.empty())
            str += _("|");
        str += _("Recursion");
    }
    if ((logLevel & Core::LogLevel::Error) != Core::LogLevel::None)
    {
        if (!str.empty())
            str += _("|");
        str += _("Error");
    }
    if ((logLevel & Core::LogLevel::Warning) != Core::LogLevel::None)
    {
        if (!str.empty())
            str += _("|");
        str += _("Warning");
    }
    if ((logLevel & Core::LogLevel::Debug) != Core::LogLevel::None)
    {
        if (!str.empty())
            str += _("|");
        str += _("Debug");
    }
    if ((logLevel & Core::LogLevel::Info) != Core::LogLevel::None)
    {
        if (!str.empty())
            str += _("|");
        str += _("Info");
    }
    if ((logLevel & Core::LogLevel::Message) != Core::LogLevel::None)
    {
        if (!str.empty())
            str += _("|");
        str += _("Message");
    }
    if (logLevel == Core::LogLevel::None)
    {
        str = _("-");
    }
    return stream << str;
}

const OSAL::String LogHandler::DefaultDomain(_(""));

LogHandlerInfo LogHandler::_defaultLogHandlerInfo(Core::LogLevel::All, LogHandler::DefaultLogHandler, nullptr);
map<OSAL::String, LogHandlerInfo> LogHandler::_logHandlerInfo;
recursive_mutex LogHandler::_guard;

LogHandlerInfo LogHandler::Set(const OSAL::String & domainName,
                               Core::LogLevel logLevelFilter,
                               LogHandlerFunction * newHandler,
                               void * userData)
{
    lock_guard<recursive_mutex> lock(_guard);
    LogHandlerInfo * info = FindOrCreateInfo(domainName);
    if (info != nullptr)
    {
        LogHandlerInfo result = *info;
        info->Set(logLevelFilter, newHandler, userData);
        return result;
    }
    return LogHandlerInfo();
}

LogHandlerInfo LogHandler::Set(const OSAL::String & domainName, const LogHandlerInfo & handlerInfo)
{
    lock_guard<recursive_mutex> lock(_guard);
    LogHandlerInfo * info = FindOrCreateInfo(domainName);
    if (info != nullptr)
    {
        LogHandlerInfo result = *info;
        info->Set(handlerInfo);
        return result;
    }
    return LogHandlerInfo();
}

void LogHandler::Reset(const OSAL::String & domainName)
{
    lock_guard<recursive_mutex> lock(_guard);
    if (domainName.empty())
    {
        _defaultLogHandlerInfo.Set(Core::LogLevel::All, DefaultLogHandler, nullptr);
        return;
    }
    LogHandlerInfo * info = FindInfo(domainName);
    if (info != nullptr)
    {
        _logHandlerInfo.erase(domainName);
    }
}

Core::LogLevel LogHandler::GetLogLevelFilter(const OSAL::String & domainName)
{
    lock_guard<recursive_mutex> lock(_guard);
    LogHandlerInfo * info = FindInfo(domainName);
    if (info != nullptr)
    {
        return info->_logLevelFilter;
    }
    return Core::LogLevel::None;
}

void LogHandler::SetLogLevelFilter(const OSAL::String & domainName, Core::LogLevel logLevelFilter)
{
    lock_guard<recursive_mutex> lock(_guard);
    LogHandlerInfo * info = FindInfo(domainName);
    if (info != nullptr)
    {
        info->_logLevelFilter = logLevelFilter;
    }
}

void LogHandler::Log(const OSAL::String & domainName,
                     const OSAL::String & componentName,
                     Core::LogLevel logLevel,
                     const OSAL::String & message)
{
    lock_guard<recursive_mutex> lock(_guard);
    LogHandlerInfo * info = FindInfo(domainName);
    if (info == nullptr)
    {
        info = FindInfo(DefaultDomain);
    }
    if (!info->IsFilteredOut(logLevel))
    {
        if (info->_handler != nullptr)
            info->_handler(domainName, componentName, logLevel, message, info->_userData);
        else
            DefaultLogHandler(domainName, componentName, logLevel, message, info->_userData);
    }
}

LogHandlerInfo * LogHandler::FindInfo(const OSAL::String & domainName)
{
    if (domainName.empty())
    {
        return &_defaultLogHandlerInfo;
    }
    map<OSAL::String, LogHandlerInfo>::iterator foundHandlerInfo = _logHandlerInfo.find(domainName);
    if (foundHandlerInfo == _logHandlerInfo.end())
    {
        return nullptr;
    }
    return &(foundHandlerInfo->second);
}

LogHandlerInfo * LogHandler::FindOrCreateInfo(const OSAL::String & domainName)
{
    LogHandlerInfo * foundInfo = FindInfo(domainName);
    if (foundInfo == nullptr)
    {
        LogHandlerInfo info(Core::LogLevel::All, nullptr, nullptr);
        _logHandlerInfo.insert(pair<OSAL::String, LogHandlerInfo>(domainName, info));
        return FindInfo(domainName);
    }
    return foundInfo;
}

void LogHandler::DefaultLogHandler(const OSAL::String & domainName,
                                   const OSAL::String & componentName,
                                   Core::LogLevel logLevel,
                                   const OSAL::String & message,
                                   void * UNUSED(userData))
{
#if defined(UNICODE) || defined(_UNICODE)
	wcout << _("Domain : ") << (domainName.empty() ? _("-") : domainName) << _(" Component : ") << componentName
          << _(" Level: ") << logLevel << _(": ") << message << endl;
#else
	cout << _("Domain : ") << (domainName.empty() ? "-" : domainName) << _(" Component : ") << componentName
		<< _(" Level: ") << logLevel << _(": ") << message << endl;
#endif
}

