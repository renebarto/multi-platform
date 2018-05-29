#include "core/files/DirectoryInfo.h"

#include <algorithm>
#include <glob.h>
#include <unistd.h>
#include "osal/Exception.h"
#include "osal/Path.h"

using namespace std;
using namespace Core;
using namespace Files;

DirectoryInfo::DirectoryInfo(const string & path)
    : FileSystemInfo(path)
{
}

DirectoryInfo::DirectoryInfo(const DirectoryInfo & other)
    : FileSystemInfo(other._fullPath)
{
}

DirectoryInfo::DirectoryInfo(DirectoryInfo && other)
    : FileSystemInfo(std::move(other._fullPath))
{
}

DirectoryInfo & DirectoryInfo::operator = (const DirectoryInfo & other)
{
    if (&other != this)
    {
        FileSystemInfo::Copy(other);
    }
    return *this;
}

DirectoryInfo & DirectoryInfo::operator = (DirectoryInfo && other)
{
    if (&other != this)
    {
        FileSystemInfo::Move(std::move(other));
    }
    return *this;
}

bool DirectoryInfo::Exists() const
{
    return Exists(FullPath());
}

bool DirectoryInfo::Exists(const std::string & path)
{
    return OSAL::Path::DirectoryExists(path);
}

