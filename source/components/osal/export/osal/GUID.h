#pragma once

#include <iostream>

#include "osal/osal.h"
#include "osal/Assert.h"
#include "osal/ByteArray.h"

namespace OSAL {

enum class GUIDVersion
{
    Null = 0,
    DateTimeMac = 1,
    DataTimeMacSecure = 2,
    NameBasedMD5 = 3,
    Random = 4,
    NameBasedSHA1 = 5,
};

class OSAL_EXPORT GUID
{
public:
    static const GUID Null;
    GUID()
        : _guid(GUIDSize)
    {
    }
    GUID(const GUID & other)
        : _guid(other._guid)
    {
    }
    GUID(OSAL::ByteArray guid)
        : _guid(std::move(guid))
    {
        ASSERT(_guid.Size() == GUIDSize);
        if (_guid.Size() != GUIDSize)
            _guid.Size(GUIDSize);
    }
    virtual ~GUID() {}

    static GUID Generate();
    static GUID GenerateRandom();
    static GUID GenerateTime();
    static GUID GenerateTimeSafe();
    timeval GetTime() const;
    GUIDVersion GetVersion() const;
    int GetVariant() const;

    static GUID Parse(const std::string & text);
    static bool TryParse(const std::string & text, GUID & ipAddress);
    GUID & operator = (const GUID & other);
    bool operator == (const GUID & other) const;
    bool operator != (const GUID & other) const;
    uint8_t & operator[] (size_t offset);
    const uint8_t & operator[] (size_t offset) const;

    virtual OSAL::ByteArray GetBytes() const;
    virtual std::ostream & PrintTo(std::ostream & stream) const;

private:
    ByteArray _guid;
    static const size_t GUIDSize = 16;

    static bool TryParse(const std::string & text, uint8_t & value);
};

inline void PrintTo(const GUID & value, std::ostream & stream)
{
    value.PrintTo(stream);
}

inline std::ostream & operator << (std::ostream & stream, const GUID & value)
{
    value.PrintTo(stream);
    return stream;
}

} // namespace OSAL

