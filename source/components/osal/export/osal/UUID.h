#pragma once

#include <iostream>

#include "osal/osal.h"
#include "osal/Assert.h"
#include "osal/bytearray.h"

namespace OSAL {

enum class UUIDVersion
{
    Null = 0,
    DateTimeMac = 1,
    DataTimeMacSecure = 2,
    NameBasedMD5 = 3,
    Random = 4,
    NameBasedSHA1 = 5,
};

// indicated by a bit pattern in octet 8, marked with N in xxxxxxxx-xxxx-xxxx-Nxxx-xxxxxxxxxxxx
enum class UUIDVariant
{
    // NCS backward compatibility (with the obsolete Apollo Network Computing System 1.5 UUID format)
    // N bit pattern: 0xxx
    // > the first 6 octets of the UUID are a 48-bit timestamp (the number of 4 microsecond units of time since 1 Jan 1980 UTC);
    // > the next 2 octets are reserved;
    // > the next octet is the "address family";
    // > the final 7 octets are a 56-bit host ID in the form specified by the address family
    NCS,

    // RFC 4122/DCE 1.1
    // N bit pattern: 10xx
    // > big-endian byte order
    RFC4122,

    // Microsoft Corporation backward compatibility
    // N bit pattern: 110x
    // > little endian byte order
    // > formerly used in the Component Object Model (COM) library
    Microsoft,

    // reserved for possible future definition
    // N bit pattern: 111x
    Reserved
};

class OSAL_EXPORT UUID
{
public:
    static const UUID Null;
    UUID()
        : _uuid(UUIDSize)
    {
    }
    UUID(const UUID & other)
        : _uuid(other._uuid)
    {
    }
    UUID(OSAL::bytearray uuid)
        : _uuid(std::move(uuid))
    {
        ASSERT(_uuid.size() == UUIDSize);
        if (_uuid.size() != UUIDSize)
            _uuid.resize(UUIDSize);
    }
    virtual ~UUID() {}

    static UUID Generate();
    static UUID GenerateRandom();
    static UUID GenerateTime();
    static UUID GenerateTimeSafe();
    timeval GetTime() const;
    UUIDVersion GetVersion() const;
    UUIDVariant GetVariant() const;

    static UUID Parse(const std::string & text);
    static bool TryParse(const std::string & text, UUID & ipAddress);
    UUID & operator = (const UUID & other);
    bool operator == (const UUID & other) const;
    bool operator != (const UUID & other) const;
    uint8_t & operator[] (size_t offset);
    const uint8_t & operator[] (size_t offset) const;

    virtual OSAL::bytearray GetBytes() const;
    virtual std::ostream & PrintTo(std::ostream & stream) const;

private:
    bytearray _uuid;
    static const size_t UUIDSize = 16;

    static bool TryParse(const std::string & text, uint8_t & value);
};

inline void PrintTo(const UUID & value, std::ostream & stream)
{
    value.PrintTo(stream);
}

inline std::ostream & operator << (std::ostream & stream, const UUID & value)
{
    value.PrintTo(stream);
    return stream;
}

} // namespace OSAL

