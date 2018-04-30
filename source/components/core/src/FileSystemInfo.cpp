#include <core/FileSystemInfo.h>

#include <sys/stat.h>
#include <cstring>
#include <osal/Path.h>

using namespace Core;

FileSystemInfo::FileSystemInfo(const std::string & path) :
    _fullPath(path),
    _name(),
//    _creationTime(),
//    _lastAccessTime(),
//    _lastWriteTime(),
    _attributes(FileAttributes::None),
    _parent(),
    _userID(),
    _groupID()
{
    struct stat fileInfo;
    if (stat(path.c_str(), &fileInfo) != 0)
        memset(&fileInfo, 0, sizeof(fileInfo));
    _name = OSAL::Path::LastPartOfPath(_fullPath);
//    _creationTime = DateTime(fileInfo.st_ctim);
//    _lastAccessTime = DateTime(fileInfo.st_atim);
//    _lastWriteTime = DateTime(fileInfo.st_mtim);
    _attributes = FileAttributes(fileInfo.st_mode);
    _userID = fileInfo.st_uid;
    _groupID = fileInfo.st_gid;
    _length = fileInfo.st_size;
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

    if (int16_t(_attributes) == 0)
        return 0;
    if (_userID == uid)
    {
        return uint16_t(_attributes & FileAttributes::MaskOwnerRights) >> 6;
    }
    if (_groupID == gid)
    {
        return uint16_t(_attributes & FileAttributes::MaskGroupRights) >> 3;
    }
    return uint16_t(_attributes & FileAttributes::MaskOtherRights);
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
