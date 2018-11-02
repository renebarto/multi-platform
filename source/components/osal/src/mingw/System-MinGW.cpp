#include "osal/PlatformDefines.h"

#if defined(WIN_MINGW)

#include <cxxabi.h>
#include <ext/stdio_filebuf.h>
#include <osal/mingw/utsname.h>

#include "osal/System.h"

using namespace std;
using namespace OSAL;

static utsname NameInfo;
static bool NameInfoInitialized = false;

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

    char releaseText[10];
    sprintf_s(releaseText, sizeof(releaseText), "%d.%d", info.dwMajorVersion, info.dwMinorVersion);
    u->release = releaseText;
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

Endianness System::PlatformEndianness()
{
    uint16_t value = 0x0001;
    uint8_t bytes[sizeof(value)];
    memcpy(bytes, &value, sizeof(value));
    return (bytes[0] == 0x01) ? Endianness::LittleEndian : Endianness::BigEndian;
}

string System::Name()
{
    if (!NameInfoInitialized)
    {
        uname(&NameInfo);
        NameInfoInitialized = true;
    }
    return NameInfo.sysname;
}
string System::Variant()
{
    if (!NameInfoInitialized)
    {
        uname(&NameInfo);
        NameInfoInitialized = true;
    }
    return NameInfo.nodename;
}
string System::Release()
{
    if (!NameInfoInitialized)
    {
        uname(&NameInfo);
        NameInfoInitialized = true;
    }
    return NameInfo.release;
}
string System::Version()
{
    if (!NameInfoInitialized)
    {
        uname(&NameInfo);
        NameInfoInitialized = true;
    }
    return NameInfo.version;
}
string System::Platform()
{
    if (!NameInfoInitialized)
    {
        uname(&NameInfo);
        NameInfoInitialized = true;
    }
    return NameInfo.machine;
}

string System::DemangleName(const string & mangledName)
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

#endif // defined(WIN_MINGW)
