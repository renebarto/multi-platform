#include "osal/OSAL.h"

#if defined(WIN_MSVC)

#pragma warning(push)
#pragma warning(disable:4091)
#include <Dbghelp.h>
#pragma warning(pop)

#include <osal/windows/utsname.h>

using namespace OSAL;

int uname(struct utsname *u)
{
    OSVERSIONINFOEX info;
    info.dwOSVersionInfoSize = sizeof(info);

    if (!GetVersionEx(reinterpret_cast<OSVERSIONINFO *>(&info)))
    {
        return -1;
    }

    u->sysname = "Microsoft Windows";
    u->nodename = "";
    u->release = "";
    u->version = "";
    u->machine = "";

    switch (info.dwMajorVersion) {
    case 4:
        switch (info.dwMinorVersion)
        {
        case 0:
            u->version = "NT 4.0";
            break;
        case 10:
            u->version = "98";
            break;
        case 90:
            u->version = "Me";
            break;
        default:
            return -1;
        }
        break;

    case 5:
        switch (info.dwMinorVersion)
        {
        case 0:
            u->version = "2000";
            break;
        case 1:
            u->version = "XP";
            break;
        case 2:
            u->version = "Server 2003";
            break;
        default:
            return -1;
        }
        break;

    case 6:
        switch (info.dwMinorVersion)
        {
        case 0:
            u->version = (info.wProductType == VER_NT_WORKSTATION)
                ? "Vista"
                : "Server 2008";
            break;
        case 1:
            u->version = (info.wProductType == VER_NT_WORKSTATION)
                ? "7"
                : "Server 2008 R2";
            break;
        case 2:
            u->version = (info.wProductType == VER_NT_WORKSTATION)
                ? "8"
                : "Server 20012";
            break;
        case 3:
            u->version = (info.wProductType == VER_NT_WORKSTATION)
                ? "8.1"
                : "Server 20012 R2";
            break;
        default:
            return -1;
        }
        break;

    case 10:
        switch (info.dwMinorVersion)
        {
        case 0:
            u->version = (info.wProductType == VER_NT_WORKSTATION)
                ? "10"
                : "Server 2016";
            break;
        default:
            return -1;
        }
        break;

    default:
        return -1;
    }

    return 0;
}

String OS::Name()
{
    utsname nameInfo;
    uname(&nameInfo);
    return ToString(nameInfo.sysname);
}
String OS::Variant()
{
    utsname nameInfo;
    uname(&nameInfo);
    return ToString(nameInfo.nodename);
}
String OS::Release()
{
    utsname nameInfo;
    uname(&nameInfo);
    return ToString(nameInfo.release);
}
String OS::Version()
{
    utsname nameInfo;
    uname(&nameInfo);
    return ToString(nameInfo.version);
}
String OS::Platform()
{
    utsname nameInfo;
    uname(&nameInfo);
    return ToString(nameInfo.machine);
}

String OS::DemangleName(const String & mangledName)
{
    String result;
    SymSetOptions(SYMOPT_UNDNAME);
    char demangledName[_MAX_PATH];
    size_t size = UnDecorateSymbolName(ToNarrowString(mangledName).c_str() + 1, demangledName, _MAX_PATH, UNDNAME_32_BIT_DECODE | UNDNAME_NAME_ONLY);
    if (size > 0)
    {
        result = ToString(demangledName);
    }
    return result;
}

#endif // defined(WIN_MSVC)
