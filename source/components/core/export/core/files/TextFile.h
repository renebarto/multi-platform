#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <osal/FlagOperators.h>
#include "core/files/TextStream.h"
#include "core/files/File.h"

namespace Core {
namespace Files {

class TextFile : virtual public TextStream, public File
{
public:
    TextFile() = delete;

    explicit TextFile(const FileInfo & info);

    explicit TextFile(const FileInfo & info, DesiredAccess desiredAccess,
                      ShareMode shareMode = ShareMode::ShareReadWrite,
                      CreationFlags creationFlags = CreationFlags::OpenExisting);

    explicit TextFile(const std::string & path);

    TextFile(const std::string & path, DesiredAccess desiredAccess,
             ShareMode shareMode = ShareMode::ShareReadWrite,
             CreationFlags creationFlags = CreationFlags::OpenExisting);

    TextFile(const TextFile &);

    TextFile(TextFile &&);

    TextFile & operator =(const TextFile &);

    TextFile & operator =(TextFile &&);

    bool CompareTo(const std::string & pathOther) override;
    bool CompareTo(const TextFile & other);

    static bool Compare(const std::string & source, const std::string & destination);
};

} // namespace Files
} // namespace Core
