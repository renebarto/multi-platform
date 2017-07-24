#pragma once

#include <vector>
#include <osal/Strings.h>

namespace Core {

template<class T>
class IStringDeserializer
{
public:
    IStringDeserializer()
    {}

    virtual ~IStringDeserializer()
    {}

    virtual bool Deserialize(const OSAL::String &text, T &result) = 0;
};

template<class T>
class IStringSerializer
{
public:
    IStringSerializer()
    {}

    virtual ~IStringSerializer()
    {}

    virtual void Serialize(OSAL::String &text, const T &result) = 0;
};

template<typename EnumType>
struct EnumConversion
{
    EnumType value;
    const char * name;
};

template<typename EnumType>
struct EnumSerializationInfo
{
public:
    EnumSerializationInfo() {}
    EnumSerializationInfo(const EnumSerializationInfo &) = delete;
    EnumSerializationInfo & operator = (const EnumSerializationInfo &) = delete;

    static OSAL::String Serialize(EnumType value)
    {
        for (const auto & item : Info)
        {
            if (item.value == value)
                return item.name;
        }
        return OSAL::String{};
    }
private:
    static std::vector<EnumConversion<EnumType>> Info;
};

} // namespace Core

