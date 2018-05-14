#include "core/files/FileInfo.h"

#include <algorithm>
#include <climits>
#include <fstream>
#include <unistd.h>
#include "osal/Exception.h"
#include "osal/Path.h"

using namespace std;
using namespace Core;
using namespace Files;

FileInfo::FileInfo(const string & path) :
    FileSystemInfo(path)
{
}

FileInfo::FileInfo(const FileInfo & other)
    : FileSystemInfo(other._fullPath)
{
}

FileInfo::FileInfo(FileInfo && other)
    : FileSystemInfo(std::move(other._fullPath))
{
}

FileInfo & FileInfo::operator = (const FileInfo & other)
{
    if (&other != this)
    {
        FileSystemInfo::Copy(other);
    }
    return *this;
}

FileInfo & FileInfo::operator = (FileInfo && other)
{
    if (&other != this)
    {
        FileSystemInfo::Move(std::move(other));
    }
    return *this;
}

bool FileInfo::Exists() const
{
    return Exists(FullPath());
}

bool FileInfo::Exists(const std::string & path)
{
    return OSAL::Path::FileExists(path);
}

