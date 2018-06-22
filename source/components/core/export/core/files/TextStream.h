#pragma once

#include <iostream>
#include <string>
#include <osal/FlagOperators.h>
#include "core/ITextStream.h"

namespace Core {
namespace Files {

class TextStream : public ITextStream
{
public:
    TextStream() = delete;

    explicit TextStream(std::iostream & info);

    TextStream(const TextStream &) = delete;
    TextStream(TextStream &&) = delete;
    TextStream & operator =(const TextStream &);
    TextStream & operator =(TextStream &&);

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
