#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <osal/FlagOperators.h>
#include "core/ITextStream.h"
#include "core/files/File.h"

namespace Core {
namespace Files {

class TextFile : public File, public ITextStream
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
    std::string ReadLine() override;
    bool ReadLine(std::string & line) override;
    bool WriteLine(const std::string & line) override;
    bool ReadString(std::string & value, const std::string & delimiters) override;
    bool ReadAll(std::string & value) override;

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
