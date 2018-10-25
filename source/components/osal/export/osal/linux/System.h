#pragma once

#include <syslog.h>
#include <cstdlib>
#include <string>
#include <typeinfo>
#include "osal/FlagOperators.h"

namespace OSAL {
namespace System {

OSAL_EXPORT Endianness PlatformEndianness();
OSAL_EXPORT std::string Name();
OSAL_EXPORT std::string Variant();
OSAL_EXPORT std::string Release();
OSAL_EXPORT std::string Version();
OSAL_EXPORT std::string Platform();

OSAL_EXPORT std::string DemangleName(const std::string & mangledName);

template <class T>
inline std::string TypeName(const T & x)
{
    return DemangleName(typeid(x).name());
}

inline const char * GetEnvironmentVariable(const char * name)
{
    return ::getenv(name);
}
inline int SetEnvironmentVariable(const char * name, const char * value, bool overwrite = true)
{
    return ::setenv(name, value, overwrite ? 1 : 0);
}
inline int UnSetEnvironmentVariable(const char * name)
{
    return ::unsetenv(name);
}

//enum class SystemLogOption : uint8_t
//{
//    LogConsoleOnError = LOG_CONS,
//    LogNoDelayOpen = LOG_NDELAY,
//    LogNoWait = LOG_NOWAIT,
//    LogDelayOpen = LOG_ODELAY,
//    LogPError = LOG_PERROR,
//    LogPID = LOG_PID,
//};
//
//enum class SystemLogPriority : uint8_t
//{
//    Error = LOG_ERR,
//    Warning = LOG_WARNING,
//    Notice = LOG_NOTICE,
//    Info = LOG_INFO,
//    Debug = LOG_DEBUG,
//};
//
//class SystemLog
//{
//public:
//    SystemLog() = delete;
//    explicit SystemLog(std::string const & identifier);
//    SystemLog(std::string const & identifier, SystemLogOption options);
//    SystemLog(const SystemLog &) = delete;
//    virtual ~SystemLog();
//
//    SystemLog & operator = (const SystemLog &) = delete;
//
//    void Write(SystemLogPriority priority, const std::string & message);
//
//protected:
//    std::string _identifier;
//};

} // namespace System
} // namespace OSAL

//template<>
//struct is_flag<OSAL::System::SystemLogOption>
//{
//    static constexpr bool value=true;
//};
