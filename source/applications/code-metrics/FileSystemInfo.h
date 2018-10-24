#pragma once

#include <string>
#include <memory>
#include <sys/stat.h>
#include "Core/DateTime.h"
#include "Core/FlagOperators.h"

namespace Core
{

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

DEFINE_FLAG_OPERATORS(FileAttributes, uint16_t);

class DirectoryInfo;

class FileSystemInfo
{
public:
    FileSystemInfo() = delete;

protected:
    explicit FileSystemInfo(const std::string & path);

public:
    const std::string & FullPath() const { return fullPath; }
    const std::string & Name() const { return name; }
    const std::shared_ptr<DirectoryInfo> Parent() const { return parent.lock(); }

    const Core::DateTime & CreationTime() const { return creationTime; }
    const Core::DateTime & LastAccessTime() const { return lastAccessTime; }
    const Core::DateTime & LastWriteTime() const { return lastWriteTime; }
    FileAttributes Attributes() const { return attributes; }

    bool IsRegularFile() const;
    bool IsDirectory() const;

    uint16_t GetRights() const;
    bool IsReadable() const;
    bool IsWritable() const;
    bool IsExecutable() const;
    bool IsReadOnly() const;

protected:
    std::string fullPath;
    std::string name;
    Core::DateTime creationTime;
    Core::DateTime lastAccessTime;
    Core::DateTime lastWriteTime;
    FileAttributes attributes;
    std::weak_ptr<DirectoryInfo> parent;
    uid_t userID;
    gid_t groupID;
    off_t length;

    void Parent(std::weak_ptr<DirectoryInfo> value) { parent = value; }
};

} // namespace Core
