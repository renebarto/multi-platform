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

bool FileInfo::IsHiddenFile() const
{
    return IsHiddenFile(FullPath());
}

bool FileInfo::Exists(const std::string & path)
{
    return OSAL::Path::FileExists(path);
}

bool FileInfo::IsHiddenFile(const string & name)
{
    if (name.empty())
        return false;
    return name[0] == '.';
}

std::string const & FileInfo::GetPath() const
{
    return FullPath();
}

string FileInfo::GetDirectoryName() const
{
    return OSAL::Path::FirstPartOfPath(FullPath());
}

std::string FileInfo::GetFilename() const
{
    return OSAL::Path::LastPartOfPath(FullPath());
}

std::string FileInfo::GetFilenameWithoutExtension() const
{
    return OSAL::Path::StripExtension(GetFilename());
}

std::string FileInfo::GetExtension() const
{
    return OSAL::Path::Extension(GetFilename());
}

