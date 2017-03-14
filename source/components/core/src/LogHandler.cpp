#include "core/LogHandler.h"

using namespace std;
using namespace Core;

ostream & Core::operator << (ostream & stream, Core::LogLevel logLevel)
{
    string str;
    if ((logLevel & Core::LogLevel::Fatal) != Core::LogLevel::None)
    {
        str += "Fatal";
    }
    if ((logLevel & Core::LogLevel::Recursion) != Core::LogLevel::None)
    {
        if (!str.empty())
            str += "|";
        str += "Recursion";
    }
    if ((logLevel & Core::LogLevel::Error) != Core::LogLevel::None)
    {
        if (!str.empty())
            str += "|";
        str += "Error";
    }
    if ((logLevel & Core::LogLevel::Warning) != Core::LogLevel::None)
    {
        if (!str.empty())
            str += "|";
        str += "Warning";
    }
    if ((logLevel & Core::LogLevel::Debug) != Core::LogLevel::None)
    {
        if (!str.empty())
            str += "|";
        str += "Debug";
    }
    if ((logLevel & Core::LogLevel::Info) != Core::LogLevel::None)
    {
        if (!str.empty())
            str += "|";
        str += "Info";
    }
    if ((logLevel & Core::LogLevel::Message) != Core::LogLevel::None)
    {
        if (!str.empty())
            str += "|";
        str += "Message";
    }
    if (logLevel == Core::LogLevel::None)
    {
        str = "-";
    }
    return stream << str;
}

const string LogHandler::DefaultDomain("");

LogHandlerInfo LogHandler::defaultLogHandlerInfo(Core::LogLevel::All, LogHandler::DefaultLogHandler, nullptr);
map<string, LogHandlerInfo> LogHandler::logHandlerInfo;
recursive_mutex LogHandler::guard;

LogHandlerInfo LogHandler::Set(const string & domainName,
                               Core::LogLevel logLevelFilter,
                               LogHandlerFunction * newHandler,
                               void * userData)
{
    lock_guard<recursive_mutex> lock(guard);
    LogHandlerInfo * info = FindOrCreateInfo(domainName);
    if (info != nullptr)
    {
        LogHandlerInfo result = *info;
        info->Set(logLevelFilter, newHandler, userData);
        return result;
    }
    return LogHandlerInfo();
}

LogHandlerInfo LogHandler::Set(const string & domainName, const LogHandlerInfo & handlerInfo)
{
    lock_guard<recursive_mutex> lock(guard);
    LogHandlerInfo * info = FindOrCreateInfo(domainName);
    if (info != nullptr)
    {
        LogHandlerInfo result = *info;
        info->Set(handlerInfo);
        return result;
    }
    return LogHandlerInfo();
}

void LogHandler::Reset(const string & domainName)
{
    lock_guard<recursive_mutex> lock(guard);
    if (domainName.empty())
    {
        defaultLogHandlerInfo.Set(Core::LogLevel::All, DefaultLogHandler, nullptr);
        return;
    }
    LogHandlerInfo * info = FindInfo(domainName);
    if (info != nullptr)
    {
        logHandlerInfo.erase(domainName);
    }
}

Core::LogLevel LogHandler::GetLogLevelFilter(const string & domainName)
{
    lock_guard<recursive_mutex> lock(guard);
    LogHandlerInfo * info = FindInfo(domainName);
    if (info != nullptr)
    {
        return info->logLevelFilter;
    }
    return Core::LogLevel::None;
}

void LogHandler::SetLogLevelFilter(const string & domainName, Core::LogLevel logLevelFilter)
{
    lock_guard<recursive_mutex> lock(guard);
    LogHandlerInfo * info = FindInfo(domainName);
    if (info != nullptr)
    {
        info->logLevelFilter = logLevelFilter;
    }
}

void LogHandler::Log(const string & domainName,
                     const string & componentName,
                     Core::LogLevel logLevel,
                     const string & message)
{
    lock_guard<recursive_mutex> lock(guard);
    LogHandlerInfo * info = FindInfo(domainName);
    if (info == nullptr)
    {
        info = FindInfo(DefaultDomain);
    }
    if (!info->IsFilteredOut(logLevel))
    {
        if (info->handler != nullptr)
            info->handler(domainName, componentName, logLevel, message, info->userData);
        else
            DefaultLogHandler(domainName, componentName, logLevel, message, info->userData);
    }
}

LogHandlerInfo * LogHandler::FindInfo(const string & domainName)
{
    if (domainName.empty())
    {
        return &defaultLogHandlerInfo;
    }
    map<string, LogHandlerInfo>::iterator foundHandlerInfo = logHandlerInfo.find(domainName);
    if (foundHandlerInfo == logHandlerInfo.end())
    {
        return nullptr;
    }
    return &(foundHandlerInfo->second);
}

LogHandlerInfo * LogHandler::FindOrCreateInfo(const string & domainName)
{
    LogHandlerInfo * info = FindInfo(domainName);
    if (info == nullptr)
    {
        LogHandlerInfo info(Core::LogLevel::All, nullptr, nullptr);
        logHandlerInfo.insert(pair<string, LogHandlerInfo>(domainName, info));
        return FindInfo(domainName);
    }
    return info;
}

void LogHandler::DefaultLogHandler(const string & domainName,
                                   const string & componentName,
                                   Core::LogLevel logLevel,
                                   const string & message,
                                   void * userData __attribute__((unused)))
{
    cout << "Domain : " << (domainName.empty() ? "-" : domainName) << " Component : " << componentName
         << " Level: " << logLevel << ": " << message << endl;
}

