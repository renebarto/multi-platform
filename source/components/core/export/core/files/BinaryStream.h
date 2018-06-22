#pragma once

#include <iostream>
#include <string>
#include <osal/FlagOperators.h>
#include "core/IBinaryStream.h"

namespace Core {
namespace Files {

class BinaryStream : public IBinaryStream
{
public:
    BinaryStream() = delete;

    explicit BinaryStream(std::iostream & info);

    BinaryStream(const BinaryStream &) = delete;
    BinaryStream(BinaryStream &&) = delete;
    BinaryStream & operator =(const BinaryStream &);
    BinaryStream & operator =(BinaryStream &&);

    size_t Read(uint8_t * buffer, size_t bufferSize);
    bool Read(uint8_t * buffer, size_t bufferSize, size_t & samplesRead);

    size_t Write(const uint8_t * buffer, size_t bufferSize);
    bool Write(const uint8_t * buffer, size_t bufferSize, size_t & samplesWritten);

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
    bool Read(OSAL::ByteArray & value, size_t numValues) override;

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
    bool Write(const OSAL::ByteArray & value) override;

private:
    std::iostream * _stream;
};

} // namespace Files
} // namespace Core
