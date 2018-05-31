#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <osal/FlagOperators.h>
#include "core/files/FileInfo.h"

namespace Core {
namespace Files {

enum class DesiredAccess
{
    Closed = 0,
    ReadOnly = 1 << 0,
    WriteOnly = 1 << 1,
    ReadWrite = (1 << 0) | (1 << 1),
    Binary = 1 << 2,
    Text = 0 << 2,
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

class File : public FileInfo
{
public:
    File() = delete;

    explicit File(const FileInfo & info);

    explicit File(const FileInfo & info, DesiredAccess desiredAccess,
                  ShareMode shareMode = ShareMode::ShareReadWrite,
                  CreationFlags creationFlags = CreationFlags::OpenExisting);

    explicit File(const std::string & path);

    File(const std::string & path, DesiredAccess desiredAccess,
         ShareMode shareMode = ShareMode::ShareReadWrite,
         CreationFlags creationFlags = CreationFlags::OpenExisting);

    File(const File &);

    File(File &&);

    File & operator =(const File &);

    File & operator =(File &&);

    bool Good() const;
    bool Eof() const;

    std::iostream & GetStream();

    bool Open(DesiredAccess desiredAccess = DesiredAccess::ReadOnly,
              ShareMode shareMode = ShareMode::ShareReadWrite,
              CreationFlags creationFlags = CreationFlags::OpenExisting);

    void Close();

    bool IsOpen() const;
    std::ios_base::openmode GetOpenMode() const;

    bool Delete();
    bool CopyTo(const std::string & destination);
    bool MoveTo(const std::string & destination);
    bool CopyTo(const File & fileDestination);
    bool MoveTo(const File & fileDestination);

    static bool Delete(const std::string & path);
    static bool Copy(const std::string & source, const std::string & destination);
    static bool Move(const std::string & source, const std::string & destination);

    virtual bool CompareTo(const std::string & pathOther);
    virtual bool CompareTo(const File & other);

    static bool Compare(const std::string & source, const std::string & destination);

protected:
    std::fstream _stream;
    std::ios_base::openmode _openMode;

    bool OpenInternal(std::ios_base::openmode openMode);
};

} // namespace Files
} // namespace Core

template<>
struct is_flag<Core::Files::DesiredAccess>
{
    static constexpr bool value=true;
};
