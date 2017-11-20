#include "osal/OSAL.h"

#include <sys/utsname.h>
#include <cxxabi.h>
#include <ext/stdio_filebuf.h>
#include <cassert>

using namespace OSAL;

OS::TemporaryFile::TemporaryFile()
    : _stream()
    , _path()
{
    const int MAX_PATH = 4096;
    char fileMask[MAX_PATH];
    sprintf(fileMask, "%s/testXXXXXX", P_tmpdir);
    int fd = mkstemp(fileMask);
    _path = fileMask;
    __gnu_cxx::stdio_filebuf<char> filebuf(fd, std::ios::out);
    _stream = std::make_shared<std::iostream>(&filebuf);
}

OS::TemporaryFile::~TemporaryFile()
{
    Close();
    OSAL::Path::MakeSureFileDoesNotExist(_path);
}

void OS::TemporaryFile::Close()
{
    _stream = nullptr;
}

std::iostream & OS::TemporaryFile::GetStream()
{
    assert(_stream != nullptr);
    return *_stream;
}

std::string const & OS::TemporaryFile::GetPath() const
{
    return _path;
}

static utsname NameInfo;
static bool NameInfoInitialized = false;

String OS::Name()
{
    if (!NameInfoInitialized)
    {
        uname(&NameInfo);
        NameInfoInitialized = true;
    }
    return ToString(NameInfo.sysname);
}
String OS::Variant()
{
    if (!NameInfoInitialized)
    {
        uname(&NameInfo);
        NameInfoInitialized = true;
    }
    return ToString(NameInfo.nodename);
}
String OS::Release()
{
    if (!NameInfoInitialized)
    {
        uname(&NameInfo);
        NameInfoInitialized = true;
    }
    return ToString(NameInfo.release);
}
String OS::Version()
{
    if (!NameInfoInitialized)
    {
        uname(&NameInfo);
        NameInfoInitialized = true;
    }
    return ToString(NameInfo.version);
}
String OS::Platform()
{
    if (!NameInfoInitialized)
    {
        uname(&NameInfo);
        NameInfoInitialized = true;
    }
    return ToString(NameInfo.machine);
}

String OS::DemangleName(const String & mangledName)
{
    String result;
    int status;
    char * demangledName = abi::__cxa_demangle(mangledName.c_str(), 0, 0, &status);
    if (status == 0)
    {
        result = ToString(demangledName);
        std::free(demangledName);
    }
    return result;
}
