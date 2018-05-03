#pragma once

#include <memory>
#include <string>
#include <vector>
#include <core/exports.h>
#include <osal/FlagOperators.h>
#include <osal/Users.h>
#include <core/time/DateTime.h>

namespace Core {
namespace File {

enum class FileAttributes : uint16_t
{
    None = 0x0000,
    Directory = 0x4000,
    CharacterDevice = 0x2000,
    BlockDevice = 0x6000,
    Normal = 0x8000,
    Fifo = 0x2000,
    Symlink = 0xA000,
    Socket = 0xC000,
    MaskOwnerRights = 0x01C0,
    MaskGroupRights = 0x0038,
    MaskOtherRights = 0x0007,
    ExecOtherRights = 0x0001,
    WriteOtherRights = 0x0002,
    ReadOtherRights = 0x0004,
};

class DirectoryInfo;

class CORE_EXPORT FileSystemInfo
{
public:
    FileSystemInfo() = delete;

protected:
    explicit FileSystemInfo(const std::string & path);

public:
    const std::string & FullPath() const { return _fullPath; }
    const std::string & Name() const { return _name; }
    const std::shared_ptr<DirectoryInfo> Parent() const { return _parent.lock(); }

    const Time::DateTime & CreationTime() const { return _creationTime; }
    const Time::DateTime & LastAccessTime() const { return _lastAccessTime; }
    const Time::DateTime & LastWriteTime() const { return _lastWriteTime; }
    FileAttributes Attributes() const { return _attributes; }

    bool IsRegularFile() const;
    bool IsDirectory() const;

    uint16_t GetRights() const;
    bool IsReadable() const;
    bool IsWritable() const;
    bool IsExecutable() const;
    bool IsReadOnly() const;
    ssize_t Size() const;

protected:
    std::string _fullPath;
    std::string _name;
    Time::DateTime _creationTime;
    Time::DateTime _lastAccessTime;
    Time::DateTime _lastWriteTime;
    FileAttributes _attributes;
    std::weak_ptr<DirectoryInfo> _parent;
    OSAL::Users::UserID _userID;
    OSAL::Users::GroupID _groupID;
    ssize_t _fileSize;

    void Parent(std::weak_ptr<DirectoryInfo> value) { _parent = value; }
};

} // namespace File
} // namespace Core

template<>
struct is_flag<Core::File::FileAttributes>
{
    static constexpr bool value=true;
};
