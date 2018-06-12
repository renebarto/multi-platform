#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <osal/FlagOperators.h>
#include "core/IBinaryStream.h"
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

    bool Read(char & ch) override;
    bool Read(int8_t & value) override;
    bool Read(uint8_t & value) override;
    bool Read(int16_t & value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) override;
    bool Read(uint16_t & value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) override;
    bool Read(int32_t & value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) override;
    bool Read(uint32_t & value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) override;
    bool Read(int64_t & value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) override;
    bool Read(uint64_t & value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) override;
    bool Read(float & value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) override;
    bool Read(double & value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) override;
    bool Read(long double & value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) override;
    bool Read(std::string & value, size_t numChars) override;

    bool Write(char ch) override;
    bool Write(int8_t value) override;
    bool Write(uint8_t value) override;
    bool Write(int16_t value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) override;
    bool Write(uint16_t value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) override;
    bool Write(int32_t value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) override;
    bool Write(uint32_t value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) override;
    bool Write(int64_t value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) override;
    bool Write(uint64_t value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) override;
    bool Write(float value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) override;
    bool Write(double value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) override;
    bool Write(long double value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) override;
    bool Write(const std::string & value) override;
};

} // namespace Files
} // namespace Core
