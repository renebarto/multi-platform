#include "osal/PlatformDefines.h"

#if defined(DARWIN)

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
    const int MAX_PATH = 4096;
    char fileMask[MAX_PATH];
    sprintf(fileMask, "%s/testXXXXXX", P_tmpdir);
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

std::iostream & TemporaryFile::GetStream()
{
    assert(_stream != nullptr);
    return *_stream;
}

std::string const & TemporaryFile::GetPath() const
{
    return _path;
}

#endif // defined(DARWIN)
