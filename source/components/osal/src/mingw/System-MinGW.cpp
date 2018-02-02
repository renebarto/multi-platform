#include "osal/PlatformDefines.h"

#if defined(WIN_MINGW)

#include <cxxabi.h>
#include <ext/stdio_filebuf.h>
#include <sys/utsname.h>

#include "osal/System.h"

using namespace std;
using namespace OSAL;

static utsname NameInfo;
static bool NameInfoInitialized = false;

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
