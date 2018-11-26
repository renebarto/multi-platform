#pragma once

#include <osal/exports.h>
#if defined(LINUX) || defined(APPLE)
#include <osal/linux/DomainSocketAddress.h>
#endif
#include <osal/IPV4Address.h>
#include <osal/IPV6Address.h>
#include <osal/MACAddress.h>
#include "core/core.h"
#include "core/serialization/Serialization.h"
#include "core/nullable.h"

namespace Core {

CORE_EXPORT bool Deserialize(const std::string & text, bool & value);
CORE_EXPORT bool Deserialize(const std::string & text, int8_t & value, int base = 10);
CORE_EXPORT bool Deserialize(const std::string & text, uint8_t & value, int base = 10);
CORE_EXPORT bool Deserialize(const std::string & text, int16_t & value, int base = 10);
CORE_EXPORT bool Deserialize(const std::string & text, uint16_t & value, int base = 10);
CORE_EXPORT bool Deserialize(const std::string & text, int32_t & value, int base = 10);
CORE_EXPORT bool Deserialize(const std::string & text, uint32_t & value, int base = 10);
CORE_EXPORT bool Deserialize(const std::string & text, int64_t & value, int base = 10);
CORE_EXPORT bool Deserialize(const std::string & text, uint64_t & value, int base = 10);
CORE_EXPORT bool Deserialize(const std::string & text, float & value);
CORE_EXPORT bool Deserialize(const std::string & text, double & value);
CORE_EXPORT bool Deserialize(const std::string & text, long double & value);
CORE_EXPORT bool Deserialize(const OSAL::bytearray & text, bool & value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
CORE_EXPORT bool Deserialize(const OSAL::bytearray & text, int8_t & value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
CORE_EXPORT bool Deserialize(const OSAL::bytearray & text, uint8_t & value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
CORE_EXPORT bool Deserialize(const OSAL::bytearray & text, int16_t & value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
CORE_EXPORT bool Deserialize(const OSAL::bytearray & text, uint16_t & value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
CORE_EXPORT bool Deserialize(const OSAL::bytearray & text, int32_t & value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
CORE_EXPORT bool Deserialize(const OSAL::bytearray & text, uint32_t & value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
CORE_EXPORT bool Deserialize(const OSAL::bytearray & text, int64_t & value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
CORE_EXPORT bool Deserialize(const OSAL::bytearray & text, uint64_t & value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
CORE_EXPORT bool Deserialize(const OSAL::bytearray & text, float & value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
CORE_EXPORT bool Deserialize(const OSAL::bytearray & text, double & value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
CORE_EXPORT bool Deserialize(const OSAL::bytearray & text, long double & value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
CORE_EXPORT bool Deserialize(const OSAL::bytearray & text, std::string & value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
#if defined(LINUX) || defined(APPLE)
CORE_EXPORT bool Deserialize(const std::string & text, OSAL::Network::DomainSocketAddress & value);
CORE_EXPORT bool Deserialize(const OSAL::bytearray & data, OSAL::Network::DomainSocketAddress & value);
#endif
CORE_EXPORT bool Deserialize(const std::string & text, OSAL::Network::IPV4Address & value);
CORE_EXPORT bool Deserialize(const std::string & text, OSAL::Network::IPV6Address & value);
CORE_EXPORT bool Deserialize(const std::string & text, OSAL::Network::MACAddress & value);
CORE_EXPORT bool Deserialize(const OSAL::bytearray & data, OSAL::Network::IPV4Address & value);
CORE_EXPORT bool Deserialize(const OSAL::bytearray & data, OSAL::Network::IPV6Address & value);
CORE_EXPORT bool Deserialize(const OSAL::bytearray & data, OSAL::Network::MACAddress & value);

inline bool Deserialize(const std::string & text, std::string & value)
{
    value = text;
    return true;
}
inline bool Deserialize(const OSAL::bytearray & text, OSAL::bytearray & value)
{
    value = text;
    return true;
}
template<class T>
bool Deserialize(const std::string &text, nullable<T> &value)
{
    T parsedValue;
    if (Core::Deserialize(text, parsedValue)) {
        value = parsedValue;
        return true;
    }
    value.SetNull();
    return false;
}

template<class T>
class StringDeserializer : public IStringDeserializer<T>
{
public:
    virtual bool Deserialize(const std::string &text, T &value) override
    {
        T parsedValue;
        if (Core::Deserialize(text, parsedValue)) {
            value = parsedValue;
            return true;
        }
        value = T{};
        return false;
    }
};

template<class T>
class BinaryDeserializer : public IBinaryDeserializer<T>
{
public:
    virtual bool Deserialize(const OSAL::bytearray &text, T &value) override
    {
        T parsedValue;
        if (Core::Deserialize(text, parsedValue)) {
            value = parsedValue;
            return true;
        }
        value = T{};
        return false;
    }
};

} // namespace Core
