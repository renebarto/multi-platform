#include "json/Object.h"
#include "Parse.h"
#include <core/serialization/DeserializationImpl.h>
#include <core/serialization/SerializationImpl.h>

namespace JSON
{

KVPair::KVPair()
    : _key()
    , _value()
{
}

KVPair::KVPair(const OSAL::String & key, const ValuePtr & value)
    : _key(key)
    , _value(value)
{
}

bool KVPair::Deserialize(std::basic_istream<OSAL::Char> & stream)
{
    return false;
}

void KVPair::Serialize(std::basic_ostream<OSAL::Char> & stream)
{
    stream << _key << _(" : ") << _value;
}

} // namespace JSON
