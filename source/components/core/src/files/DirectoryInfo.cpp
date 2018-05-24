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
    , _subDirectories()
    , _files()
{
}

DirectoryInfo::DirectoryInfo(const DirectoryInfo & other)
    : FileSystemInfo(other._fullPath)
    , _subDirectories(other._subDirectories)
    , _files(other._files)
{
}

DirectoryInfo::DirectoryInfo(DirectoryInfo && other)
    : FileSystemInfo(std::move(other._fullPath))
    , _subDirectories(std::move(other._subDirectories))
    , _files(std::move(other._files))
{
}

DirectoryInfo & DirectoryInfo::operator = (const DirectoryInfo & other)
{
    if (&other != this)
    {
        FileSystemInfo::Copy(other);
        _subDirectories = other._subDirectories;
        _files = other._files;
    }
    return *this;
}

DirectoryInfo & DirectoryInfo::operator = (DirectoryInfo && other)
{
    if (&other != this)
    {
        FileSystemInfo::Move(std::move(other));
        _subDirectories = std::move(other._subDirectories);
        _files = std::move(other._files);
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

void DirectoryInfo::AddSubDirectory(const DirectoryInfo & directory) const
{
    _subDirectories.push_back(directory);
}

bool DirectoryInfo::HaveSubDirectory(const std::string & name) const
{
    for (auto const & directory : _subDirectories)
    {
        if (directory.Name() == name)
            return true;
    }
    return false;
}

void DirectoryInfo::AddFile(const FileInfo & file) const
{
    _files.push_back(file);
}

bool DirectoryInfo::HaveFile(const std::string & name) const
{
    for (auto const & file : _files)
    {
        if (file.Name() == name)
            return true;
    }
    return false;
}

