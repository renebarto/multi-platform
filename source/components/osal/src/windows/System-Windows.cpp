#include "osal/PlatformDefines.h"

#if defined(WIN_MSVC)

#include "osal/osal.h"
WARNING_PUSH
WARNING_DISABLE(4668)
WARNING_DISABLE(4091)
#include <Dbghelp.h>
WARNING_POP

//#include <osal/windows/utsname.h>

#include "osal/System.h"

using namespace std;
using namespace OSAL;

//static utsname NameInfo;
//static bool NameInfoInitialized = false;

//int uname(struct utsname *u)
//{
//    OSVERSIONINFOEX info;
//    info.dwOSVersionInfoSize = sizeof(info);
//
//#pragma warning(disable : 4996)
//    if (!GetVersionEx(reinterpret_cast<OSVERSIONINFO *>(&info)))
//    {
//        return -1;
//    }
//#pragma warning(default : 4996)
//
//    u->sysname = "Microsoft Windows";
//    u->nodename = "";
//    u->release = "";
//    u->version = "";
//    u->machine = "";
//
//    switch (info.dwMajorVersion) {
//    case 4:
//        switch (info.dwMinorVersion)
//        {
//        case 0:
//            u->version = "NT 4.0";
//            break;
//        case 10:
//            u->version = "98";
//            break;
//        case 90:
//            u->version = "Me";
//            break;
//        default:
//            return -1;
//        }
//        break;
//
//    case 5:
//        switch (info.dwMinorVersion)
//        {
//        case 0:
//            u->version = "2000";
//            break;
//        case 1:
//            u->version = "XP";
//            break;
//        case 2:
//            u->version = "Server 2003";
//            break;
//        default:
//            return -1;
//        }
//        break;
//
//    case 6:
//        switch (info.dwMinorVersion)
//        {
//        case 0:
//            u->version = (info.wProductType == VER_NT_WORKSTATION)
//                ? "Vista"
//                : "Server 2008";
//            break;
//        case 1:
//            u->version = (info.wProductType == VER_NT_WORKSTATION)
//                ? "7"
//                : "Server 2008 R2";
//            break;
//        case 2:
//            u->version = (info.wProductType == VER_NT_WORKSTATION)
//                ? "8"
//                : "Server 20012";
//            break;
//        case 3:
//            u->version = (info.wProductType == VER_NT_WORKSTATION)
//                ? "8.1"
//                : "Server 20012 R2";
//            break;
//        default:
//            return -1;
//        }
//        break;
//
//    case 10:
//        switch (info.dwMinorVersion)
//        {
//        case 0:
//            u->version = (info.wProductType == VER_NT_WORKSTATION)
//                ? "10"
//                : "Server 2016";
//            break;
//        default:
//            return -1;
//        }
//        break;
//
//    default:
//        return -1;
//    }
//
//    return 0;
//}
//
//Endianness System::PlatformEndianness()
//{
//    uint16_t value = 0x0001;
//    uint8_t bytes[sizeof(value)];
//    memcpy(bytes, &value, sizeof(value));
//    return (bytes[0] == 0x01) ? Endianness::LittleEndian : Endianness::BigEndian;
//}
//
//string System::Name()
//{
//    if (!NameInfoInitialized)
//    {
//        uname(&NameInfo);
//        NameInfoInitialized = true;
//    }
//    return NameInfo.sysname;
//}
//string System::Variant()
//{
//    if (!NameInfoInitialized)
//    {
//        uname(&NameInfo);
//        NameInfoInitialized = true;
//    }
//    return NameInfo.nodename;
//}
//string System::Release()
//{
//    if (!NameInfoInitialized)
//    {
//        uname(&NameInfo);
//        NameInfoInitialized = true;
//    }
//    return NameInfo.release;
//}
//string System::Version()
//{
//    if (!NameInfoInitialized)
//    {
//        uname(&NameInfo);
//        NameInfoInitialized = true;
//    }
//    return NameInfo.version;
//}
//string System::Platform()
//{
//    if (!NameInfoInitialized)
//    {
//        uname(&NameInfo);
//        NameInfoInitialized = true;
//    }
//    return NameInfo.machine;
//}

string System::DemangleName(const string & mangledName)
{
    string result;
    SymSetOptions(SYMOPT_UNDNAME);
    char demangledName[_MAX_PATH];
    const string ClassLiteral = "class ";
    size_t size = UnDecorateSymbolName(mangledName.c_str(), demangledName, _MAX_PATH, UNDNAME_32_BIT_DECODE | UNDNAME_NAME_ONLY);
    if (size > 0)
    {
        result = demangledName;
    }
    if (result.find(ClassLiteral, 0) == 0)
    {
        result = result.substr(ClassLiteral.length());
    }
    return result;
}

#endif // defined(WIN_MSVC)
