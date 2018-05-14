#include <core/files/FileSystemInfo.h>

#include <memory>
#include <osal/Path.h>
#include <osal/Files.h>
#include <core/files/DirectoryInfo.h>

using namespace Core;
using namespace Files;

FileSystemInfo::FileSystemInfo(const std::string & path)
    : _fullPath(path)
    , _name()
    , _creationTime()
    , _lastAccessTime()
    , _lastWriteTime()
    , _attributes(FileAttributes::None)
    , _parent()
    , _userID()
    , _groupID()
{
    struct stat fileInfo;
    memset(&fileInfo, 0, sizeof(fileInfo));
    if (OSAL::Files::Stat(path.c_str(), &fileInfo) != 0)
        memset(&fileInfo, 0, sizeof(fileInfo));
    _name = OSAL::Path::LastPartOfPath(_fullPath);
    _creationTime = Time::DateTime(fileInfo.st_ctim);
    _lastAccessTime = Time::DateTime(fileInfo.st_atim);
    _lastWriteTime = Time::DateTime(fileInfo.st_mtim);
    _attributes = FileAttributes(fileInfo.st_mode);
    _userID = fileInfo.st_uid;
    _groupID = fileInfo.st_gid;
    _fileSize = fileInfo.st_size;
}

FileSystemInfo::FileSystemInfo(const FileSystemInfo & other)
    : _fullPath(other._fullPath)
    , _name(other._name)
    , _creationTime(other._creationTime)
    , _lastAccessTime(other._lastAccessTime)
    , _lastWriteTime(other._lastWriteTime)
    , _attributes(other._attributes)
    , _parent(other._parent)
    , _userID(other._userID)
    , _groupID(other._groupID)
{
}

FileSystemInfo::FileSystemInfo(FileSystemInfo && other)
    : _fullPath(std::move(other._fullPath))
    , _name(std::move(other._name))
    , _creationTime(std::move(other._creationTime))
    , _lastAccessTime(std::move(other._lastAccessTime))
    , _lastWriteTime(std::move(other._lastWriteTime))
    , _attributes(std::move(other._attributes))
    , _parent(std::move(other._parent))
    , _userID(std::move(other._userID))
    , _groupID(std::move(other._groupID))
{
}

FileSystemInfo & FileSystemInfo::operator = (const FileSystemInfo & other)
{
    Copy(other);
    return *this;
}

FileSystemInfo & FileSystemInfo::operator = (FileSystemInfo && other)
{
    Move(std::move(other));
    return *this;
}

void FileSystemInfo::Copy(const FileSystemInfo & other)
{
    if (&other != this)
    {
        _fullPath = other._fullPath;
        _name = other._name;
        _creationTime = other._creationTime;
        _lastAccessTime = other._lastAccessTime;
        _lastWriteTime = other._lastWriteTime;
        _attributes = other._attributes;
        _parent = other._parent;
        _userID = other._userID;
        _groupID = other._groupID;
    }
}

void FileSystemInfo::Move(FileSystemInfo && other)
{
    if (&other != this)
    {
        _fullPath = std::move(other._fullPath);
        _name = std::move(other._name);
        _creationTime = std::move(other._creationTime);
        _lastAccessTime = std::move(other._lastAccessTime);
        _lastWriteTime = std::move(other._lastWriteTime);
        _attributes = std::move(other._attributes);
        _parent = std::move(other._parent);
        _userID = std::move(other._userID);
        _groupID = std::move(other._groupID);
    }
}

bool FileSystemInfo::IsRegularFile() const
{
    return (Attributes() & FileAttributes::Symlink) == FileAttributes::Normal;
}

bool FileSystemInfo::IsSymlink() const
{
    return (Attributes() & FileAttributes::Symlink) == FileAttributes::Symlink;
}

bool FileSystemInfo::IsDirectory() const
{
    return (Attributes() & FileAttributes::Directory) != 0;
}

uint16_t FileSystemInfo::GetRights() const
{
    OSAL::Users::GroupID gid = OSAL::Users::GroupID::GetGroupID();
    OSAL::Users::UserID uid = OSAL::Users::UserID::GetUserID();

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

bool FileSystemInfo::IsHidden() const
{
    return IsHidden(Name());
}

bool FileSystemInfo::IsHidden(const std::string & name)
{
    if (name.empty())
        return false;
    return name[0] == '.';
}

std::string const & FileSystemInfo::GetPath() const
{
    return FullPath();
}

std::string FileSystemInfo::GetDirectoryName() const
{
    return OSAL::Path::FirstPartOfPath(FullPath());
}

std::string FileSystemInfo::GetFilename() const
{
    return OSAL::Path::LastPartOfPath(FullPath());
}

std::string FileSystemInfo::GetFilenameWithoutExtension() const
{
    return OSAL::Path::StripExtension(GetFilename());
}

std::string FileSystemInfo::GetExtension() const
{
    return OSAL::Path::Extension(GetFilename());
}

