#include "osal/PlatformDefines.h"

#if defined(WIN_MSVC)

#include <cassert>
#include <fstream>
#include "osal/osal.h"

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
    char filePath[MAX_PATH];
    if (!GetTempFileNameA(directory, "test", 0, filePath))
        return;
    _path = filePath;
    _stream = std::make_shared<std::fstream>(_path);
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

#endif // defined(WIN_MSVC)
