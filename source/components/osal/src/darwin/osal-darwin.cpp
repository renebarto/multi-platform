#include "osal/osal.h"

#if defined(DARWIN)

#include <sys/utsname.h>
#include <cxxabi.h>

using namespace OSAL;

String OS::Name()
{
    utsname nameInfo;
    uname(&nameInfo);
    return nameInfo.sysname;
}
String OS::Variant()
{
    utsname nameInfo;
    uname(&nameInfo);
    return nameInfo.nodename;
}
String OS::Release()
{
    utsname nameInfo;
    uname(&nameInfo);
    return nameInfo.release;
}
String OS::Version()
{
    utsname nameInfo;
    uname(&nameInfo);
    return nameInfo.version;
}
String OS::Platform()
{
    utsname nameInfo;
    uname(&nameInfo);
    return nameInfo.machine;
}

String OS::DemangleName(const String & mangledName)
{
    String result;
    int status;
    char * demangledName = abi::__cxa_demangle(mangledName.c_str(), 0, 0, &status);
    if (status == 0)
    {
        result = demangledName;
        std::free(demangledName);
    }
    return result;
}

#endif // defined(DARWIN)
