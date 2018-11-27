#pragma once

#include <string>
#include <vector>
#include "osal/bytearray.h"

namespace Core {

template<class T>
class IStringDeserializer
{
public:
    IStringDeserializer()
    {}

    virtual ~IStringDeserializer()
    {}

    virtual bool Deserialize(const std::string & text, T & result) = 0;
};

template<class T>
class IStringSerializer
{
public:
    IStringSerializer()
    {}

    virtual ~IStringSerializer()
    {}

    virtual void Serialize(std::string & text, const T & result) = 0;
};

template<class T>
class IBinaryDeserializer
{
public:
    IBinaryDeserializer()
    {}

    virtual ~IBinaryDeserializer()
    {}

    virtual bool Deserialize(const OSAL::bytearray & text, T & result) = 0;
};

template<class T>
class IBinarySerializer
{
public:
    IBinarySerializer()
    {}

    virtual ~IBinarySerializer()
    {}

    virtual void Serialize(OSAL::bytearray & text, const T & result) = 0;
};

} // namespace Core

