#include "FileInfo.h"

#include <algorithm>
#include <climits>
#include <fstream>
#include <unistd.h>
//#include "Core/Exception.h"
//#include "Core/Path.h"

using namespace std;
using namespace Core;

FileInfo::FileInfo(const string & path) :
    FileSystemInfo(path)
{
}

string FileInfo::DirectoryName() const
{
    DirectoryInfo::Ptr directory = Directory();
    if (directory == nullptr)
        return string();
    return directory->Name();
}

DirectoryInfo::Ptr FileInfo::Directory() const
{
    return parent.lock();
}

bool FileInfo::Exists() const
{
    return Exists(FullPath());
}

bool FileInfo::Exists(const std::string & path)
{
    return Path::FileExists(path);
}

void FileInfo::Delete()
{
    Delete(FullPath());
}

void FileInfo::Delete(const std::string & path)
{
    unlink(path.c_str());
}

void FileInfo::CopyTo(const std::string & destination)
{
    Copy(FullPath(), destination);
}

void FileInfo::Copy(const std::string & source, const std::string & destination)
{
    std::ifstream streamIn(source, std::ios::binary);
    std::ofstream streamOut(destination, std::ios::binary);

    uint8_t buffer[1024];
    while (streamIn && streamOut)
    {
        std::streamsize bytesRead = streamIn.readsome((char *)buffer, sizeof(buffer));
        streamOut.write((char *)buffer, bytesRead);
    }
}

void FileInfo::MoveTo(const std::string & destination)
{
    Move(FullPath(), destination);
}

void FileInfo::Move(const std::string & source, const std::string & destination)
{
    if (rename(source.c_str(), destination.c_str()) != 0)
        Object::ThrowOnError(__func__, __FILE__, __LINE__, errno);
}

bool FileInfo::IsHiddenFile() const
{
    return IsHiddenFile(Name());
}

bool FileInfo::IsHiddenFile(const string & name) const
{
    if (name.empty())
        return false;
    return name[0] == '.';
}
