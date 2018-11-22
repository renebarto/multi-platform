#include "osal/PlatformDefines.h"

#if defined(WIN_MINGW)

#include <cassert>
#include <ext/stdio_filebuf.h>

#include "osal/Files.h"
#include "osal/Path.h"

using namespace std;
using namespace OSAL;
using namespace Files;

TemporaryFile::TemporaryFile()
    : _stream()
    , _path()
{
    char directory[MAX_PATH];
    if (!GetTempPathA(sizeof(directory), directory))
        return;
    char fileMask[MAX_PATH];
    sprintf(fileMask, "%s/testXXXXXX", directory);
    int fd = mkstemp(fileMask);
    _path = fileMask;
    __gnu_cxx::stdio_filebuf<char> filebuf(fd, std::ios::out);
    _stream = std::make_shared<std::iostream>(&filebuf);
}

TemporaryFile::~TemporaryFile()
{
    Close();
    OSAL::Path::MakeSureFileDoesNotExist(_path);
}

void TemporaryFile::Close()
{
    _stream = nullptr;
}

std::iostream * TemporaryFile::GetStream()
{
    return _stream.get();
}

std::string const & TemporaryFile::GetPath() const
{
    return _path;
}

#endif // defined(WIN_MINGW)
