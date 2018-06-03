#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <osal/FlagOperators.h>
#include "core/ITextStream.h"
#include "core/files/File.h"

namespace Core {
namespace Files {

class BinaryFile : public File, virtual public IBinaryStream
{
public:
    BinaryFile() = delete;

    explicit BinaryFile(const FileInfo & info);

    explicit BinaryFile(const FileInfo & info, DesiredAccess desiredAccess,
                      ShareMode shareMode = ShareMode::ShareReadWrite,
                      CreationFlags creationFlags = CreationFlags::OpenExisting);

    explicit BinaryFile(const std::string & path);

    BinaryFile(const std::string & path, DesiredAccess desiredAccess,
             ShareMode shareMode = ShareMode::ShareReadWrite,
             CreationFlags creationFlags = CreationFlags::OpenExisting);

    BinaryFile(const BinaryFile &);

    BinaryFile(BinaryFile &&);

    BinaryFile & operator =(const BinaryFile &);

    BinaryFile & operator =(BinaryFile &&);

    bool CompareTo(const std::string & pathOther) override;
    bool CompareTo(const BinaryFile & other);

    static bool Compare(const std::string & source, const std::string & destination);

    // TODO: Implement IBinaryStream
//    char ReadChar() override;
//    bool ReadChar(char & ch) override;
//    std::string ReadLine() override;
//    bool ReadLine(std::string & line) override;
//    bool WriteChar(char ch) override;
//    bool WriteLine(const std::string & line) override;
};

} // namespace Files
} // namespace Core
