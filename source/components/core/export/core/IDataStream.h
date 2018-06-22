#pragma once

#include <string>
#include <osal/ByteArray.h>
#include <osal/System.h>
#include "core/Util.h"

namespace Core {

class IDataStream
{
public:
    virtual ~IDataStream() {}

    virtual bool Read(char & ch) = 0;
    virtual bool Read(int8_t & value) = 0;
    virtual bool Read(uint8_t & value) = 0;
    virtual bool Read(int16_t & value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) = 0;
    virtual bool Read(uint16_t & value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) = 0;
    virtual bool Read(int32_t & value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) = 0;
    virtual bool Read(uint32_t & value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) = 0;
    virtual bool Read(int64_t & value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) = 0;
    virtual bool Read(uint64_t & value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) = 0;
    virtual bool Read(float & value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) = 0;
    virtual bool Read(double & value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) = 0;
    virtual bool Read(long double & value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) = 0;
    virtual bool Read(std::string & value, size_t numChars) = 0;
    virtual bool Read(OSAL::ByteArray & value, size_t numValues) = 0;

    virtual bool Write(char ch) = 0;
    virtual bool Write(int8_t value) = 0;
    virtual bool Write(uint8_t value) = 0;
    virtual bool Write(int16_t value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) = 0;
    virtual bool Write(uint16_t value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) = 0;
    virtual bool Write(int32_t value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) = 0;
    virtual bool Write(uint32_t value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) = 0;
    virtual bool Write(int64_t value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) = 0;
    virtual bool Write(uint64_t value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) = 0;
    virtual bool Write(float value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) = 0;
    virtual bool Write(double value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) = 0;
    virtual bool Write(long double value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian) = 0;
    virtual bool Write(const std::string & value) = 0;
    virtual bool Write(const OSAL::ByteArray & value) = 0;
};

} // namespace Core
