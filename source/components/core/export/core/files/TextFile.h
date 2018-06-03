#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <osal/FlagOperators.h>
#include "core/ITextStream.h"
#include "core/files/File.h"

namespace Core {
namespace Files {

class TextFile : public File, virtual public ITextStream
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

    char ReadChar() override;
    bool ReadChar(char & ch) override;
    std::string ReadLine() override;
    bool ReadLine(std::string & line) override;
    bool WriteChar(char ch) override;
    bool WriteLine(const std::string & line) override;
};

} // namespace Files
} // namespace Core
