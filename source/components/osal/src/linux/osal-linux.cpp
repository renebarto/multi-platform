#include "osal/osal.h"

#include <sys/utsname.h>
#include <cxxabi.h>

using namespace OSAL;

std::string OS::Name()
{
    utsname nameInfo;
    uname(&nameInfo);
    return nameInfo.sysname;
}
std::string OS::Variant()
{
    utsname nameInfo;
    uname(&nameInfo);
    return nameInfo.nodename;
}
std::string OS::Release()
{
    utsname nameInfo;
    uname(&nameInfo);
    return nameInfo.release;
}
std::string OS::Version()
{
    utsname nameInfo;
    uname(&nameInfo);
    return nameInfo.version;
}
std::string OS::Platform()
{
    utsname nameInfo;
    uname(&nameInfo);
    return nameInfo.machine;
}

std::string OS::DemangleName(const std::string & mangledName)
{
    std::string result;
    int status;
    char * demangledName = abi::__cxa_demangle(mangledName.c_str(), 0, 0, &status);
    if (status == 0)
    {
        result = demangledName;
        std::free(demangledName);
    }
    return result;
}
