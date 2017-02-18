#include "osal/osal.h"

#if defined(WIN_MSVC)

#pragma warning(push)
#pragma warning(disable:4091)
#include <Dbghelp.h>
#pragma warning(pop)

#include <sys/utsname.h>

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

String OS::ConvertTypeName(String mangledName)
{
    String result;
    SymSetOptions(SYMOPT_UNDNAME);
    char demangledName[_MAX_PATH];
    size_t size = UnDecorateSymbolName(mangledName.c_str() + 1, demangledName, _MAX_PATH, UNDNAME_32_BIT_DECODE | UNDNAME_NAME_ONLY);
    if (size > 0)
    {
        result = demangledName;
    }
    return result;
}

#endif // defined(WIN_MSVC)
