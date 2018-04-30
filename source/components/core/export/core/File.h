#pragma once

#include <iostream>
#include <memory>
#include <string>

namespace Core {

enum class DesiredAccess
{
    Closed = 0,
    ReadOnly = 1,
    WriteOnly = 2,
    ReadWrite = 3,
};

enum class ShareMode
{
    ShareNone = 0,
    SharedRead = 1,
    ShareWrite = 2,
    ShareReadWrite = 3,
    ShareDelete = 4,
    ShareAll = 7,
};

enum class CreationFlags
{
    CreateNew = 1,
    CreateOrOverwrite = 2,
    OpenExisting = 3,
    OpenOrCreate = 4,
    TruncateExisting = 5,
};

class FileInfo;

class File
{
public:
    File() = delete;
    File(const File &);
    File(File &&);
    explicit File(const std::string & path);
    File(const std::string & path, DesiredAccess desiredAccess = DesiredAccess::ReadOnly,
         ShareMode shareMode = ShareMode::ShareReadWrite,
         CreationFlags creationFlags = CreationFlags::OpenExisting);
    explicit File(const FileInfo & info);
    explicit File(const FileInfo & info, DesiredAccess desiredAccess = DesiredAccess::ReadOnly,
        ShareMode shareMode = ShareMode::ShareReadWrite,
        CreationFlags creationFlags = CreationFlags::OpenExisting);

    File & operator = (const File &);
    File & operator = (File &&);

    std::iostream * GetStream();

    bool Open(DesiredAccess desiredAccess = DesiredAccess::ReadOnly,
              ShareMode shareMode = ShareMode::ShareReadWrite,
              CreationFlags creationFlags = CreationFlags::OpenExisting);
    void Close();
    bool IsOpen() const;
    bool Exists() const;
    static bool Exists(const std::string & path);
    void Delete();
    static void Delete(const std::string & path);
    bool CopyTo(const std::string & destination);
    bool Copy(File & fileDestination);
    static bool Copy(const std::string & source, const std::string & destination);
    bool MoveTo(const std::string & destination);
    bool Move(File & fileDestination);
    bool Move(const std::string & source, const std::string & destination);
    bool IsHiddenFile() const;
    static bool IsHiddenFile(const std::string & name);
    std::string const & GetPath() const;
    std::string GetDirectory() const;
    std::string GetFilename() const;
    std::string GetFilenameWithoutExtension() const;
    std::string GetExtension() const;

protected:
    std::shared_ptr<std::iostream> _stream;
    std::string _path;
};

} // namespace Core