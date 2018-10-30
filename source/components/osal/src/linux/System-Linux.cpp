#include "osal/PlatformDefines.h"

#if defined(LINUX)

#include <cstring>
#include <cxxabi.h>
#include <ext/stdio_filebuf.h>
//#include <sys/utsname.h>

#include "osal/System.h"

using namespace std;

namespace OSAL {
namespace System {

//static utsname NameInfo;
//static bool NameInfoInitialized = false;

//Endianness PlatformEndianness()
//{
//    uint16_t value = 0x0001;
//    uint8_t bytes[sizeof(value)];
//    memcpy(bytes, &value, sizeof(value));
//    return (bytes[0] == 0x01) ? Endianness::LittleEndian : Endianness::BigEndian;
//}
//
//string Name()
//{
//    if (!NameInfoInitialized)
//    {
//        uname(&NameInfo);
//        NameInfoInitialized = true;
//    }
//    return NameInfo.sysname;
//}
//
//string Variant()
//{
//    if (!NameInfoInitialized)
//    {
//        uname(&NameInfo);
//        NameInfoInitialized = true;
//    }
//    return NameInfo.nodename;
//}
//
//string Release()
//{
//    if (!NameInfoInitialized)
//    {
//        uname(&NameInfo);
//        NameInfoInitialized = true;
//    }
//    return NameInfo.release;
//}
//
//string Version()
//{
//    if (!NameInfoInitialized)
//    {
//        uname(&NameInfo);
//        NameInfoInitialized = true;
//    }
//    return NameInfo.version;
//}
//
//string Platform()
//{
//    if (!NameInfoInitialized)
//    {
//        uname(&NameInfo);
//        NameInfoInitialized = true;
//    }
//    return NameInfo.machine;
//}

string DemangleName(const string & mangledName)
{
    string result;
    int status;
    char * demangledName = abi::__cxa_demangle(mangledName.c_str(), 0, 0, &status);
    if (status == 0)
    {
        result = demangledName;
        std::free(demangledName);
    }
    return result;
}

//SystemLog::SystemLog(const string & identifier)
//    : SystemLog(identifier, SystemLogOption::LogConsoleOnError | SystemLogOption::LogNoDelayOpen |
//                            SystemLogOption::LogPError | SystemLogOption::LogPID)
//{
//}
//
//SystemLog::SystemLog(const string & identifier, SystemLogOption options)
//    : _identifier(identifier)
//{
//    openlog(_identifier.c_str(), static_cast<int>(options), LOG_USER);
//}
//
//SystemLog::~SystemLog()
//{
//    closelog();
//}
//
//void SystemLog::Write(SystemLogPriority priority, std::string const & message)
//{
//    syslog(static_cast<int>(priority), "%s", message.c_str());
//}

} // namespace System
} // namespace OSAL

#endif // defined(LINUX)
