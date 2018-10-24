#include "Core/FileSystemInfo.h"

#include <algorithm>
#include <climits>
#include <unistd.h>
#include "Core/Exception.h"
#include "Core/Path.h"

using namespace std;
using namespace Core;

FileSystemInfo::FileSystemInfo(const std::string & path) :
    fullPath(path),
    name(),
    creationTime(),
    lastAccessTime(),
    lastWriteTime(),
    attributes(FileAttributes::None),
    parent(),
    userID(),
    groupID()
{
    struct stat fileInfo;
    if (stat(path.c_str(), &fileInfo) != 0)
        bzero(&fileInfo, sizeof(fileInfo));
    name = Path::LastPartOfPath(fullPath);
    creationTime = DateTime(fileInfo.st_ctim);
    lastAccessTime = DateTime(fileInfo.st_atim);
    lastWriteTime = DateTime(fileInfo.st_mtim);
    attributes = FileAttributes(fileInfo.st_mode);
    userID = fileInfo.st_uid;
    groupID = fileInfo.st_gid;
    length = fileInfo.st_size;
}

bool FileSystemInfo::IsRegularFile() const
{
    return (Attributes() & FileAttributes::Normal) != 0;
}

bool FileSystemInfo::IsDirectory() const
{
    return (Attributes() & FileAttributes::Directory) != 0;
}

uint16_t FileSystemInfo::GetRights() const
{
    __gid_t gid = getgid();
    __uid_t uid = getuid();

    if (int16_t(attributes) == 0)
        return 0;
    if (userID == uid)
    {
        return uint16_t(attributes & FileAttributes::MaskOwnerRights) >> 6;
    }
    if (groupID == gid)
    {
        return uint16_t(attributes & FileAttributes::MaskGroupRights) >> 3;
    }
    return uint16_t(attributes & FileAttributes::MaskOtherRights);
}

bool FileSystemInfo::IsReadable() const
{
    return (GetRights() & uint16_t(FileAttributes::ReadOtherRights)) != 0;
}

bool FileSystemInfo::IsWritable() const
{
    return (GetRights() & uint16_t(FileAttributes::WriteOtherRights)) != 0;
}

bool FileSystemInfo::IsExecutable() const
{
    return (GetRights() & uint16_t(FileAttributes::ExecOtherRights)) != 0;
}

bool FileSystemInfo::IsReadOnly() const
{
    return (GetRights() & uint16_t(FileAttributes::ReadOtherRights | FileAttributes::WriteOtherRights)) == FileAttributes::ReadOtherRights;
}

