#pragma once

#include "core/Core.h"
#include "core/serialization/Serialization.h"
#include "core/Nullable.h"

namespace Core {

CORE_EXPORT bool Deserialize(const OSAL::String & text, bool & value);
CORE_EXPORT bool Deserialize(const OSAL::String & text, int8_t & value, int base = 10);
CORE_EXPORT bool Deserialize(const OSAL::String & text, uint8_t & value, int base = 10);
CORE_EXPORT bool Deserialize(const OSAL::String & text, int16_t & value, int base = 10);
CORE_EXPORT bool Deserialize(const OSAL::String & text, uint16_t & value, int base = 10);
CORE_EXPORT bool Deserialize(const OSAL::String & text, int32_t & value, int base = 10);
CORE_EXPORT bool Deserialize(const OSAL::String & text, uint32_t & value, int base = 10);
CORE_EXPORT bool Deserialize(const OSAL::String & text, int64_t & value, int base = 10);
CORE_EXPORT bool Deserialize(const OSAL::String & text, uint64_t & value, int base = 10);
CORE_EXPORT bool Deserialize(const OSAL::String & text, float & value);
CORE_EXPORT bool Deserialize(const OSAL::String & text, double & value);
CORE_EXPORT bool Deserialize(const OSAL::String & text, long double & value);
inline bool Deserialize(const OSAL::String & text, OSAL::String & value)
{
    value = text;
    return true;
}
template<class T>
bool Deserialize(const OSAL::String &text, Nullable<T> &value)
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
    virtual bool Deserialize(const OSAL::String &text, T &value) override
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
