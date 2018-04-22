#pragma once

#include <cstdint>
#include "core/serialization/Serialization.h"
#include "json/Array.h"
#include "json/Boolean.h"
#include "json/Number.h"
#include "json/String.h"

namespace JSON {

template<class T>
class StringDeserializer : public Core::IStringDeserializer<T>
{
public:
    bool Deserialize(const std::string & text, T & result) override
    {
        return false;
    }
};

template<>
class StringDeserializer<bool> : public Core::IStringDeserializer<bool>
{
public:
    bool Deserialize(const std::string & text, bool & result) override
    {
        Boolean value;
        std::istringstream stream(text);
        if (!value.Deserialize(stream))
            return false;
        result = value.GetValue();
        return true;
    }
};

template<>
class StringDeserializer<std::string> : public Core::IStringDeserializer<std::string>
{
public:
    bool Deserialize(const std::string & text, std::string & result) override
    {
        String value;
        std::istringstream stream(text);
        if (!value.Deserialize(stream))
            return false;
        result = value.GetValue();
        return true;
    }
};

template<>
class StringDeserializer<Array> : public Core::IStringDeserializer<Array>
{
public:
    bool Deserialize(const std::string & text, Array & result) override
    {
        Array value;
        std::istringstream stream(text);
        if (!value.Deserialize(stream))
            return false;
        result = value;
        return true;
    }
};

template<>
class StringDeserializer<int8_t> : public Core::IStringDeserializer<int8_t>
{
public:
    bool Deserialize(const std::string & text, int8_t & result) override
    {
        Number value;
        std::istringstream stream(text);
        if (!value.Deserialize(stream))
            return false;
        if (!value.GetValue(result))
            return false;
        return true;
    }
};

template<>
class StringDeserializer<uint8_t> : public Core::IStringDeserializer<uint8_t>
{
public:
    bool Deserialize(const std::string & text, uint8_t & result) override
    {
        Number value;
        std::istringstream stream(text);
        if (!value.Deserialize(stream))
            return false;
        if (!value.GetValue(result))
            return false;
        return true;
    }
};

template<>
class StringDeserializer<int16_t> : public Core::IStringDeserializer<int16_t>
{
public:
    bool Deserialize(const std::string & text, int16_t & result) override
    {
        Number value;
        std::istringstream stream(text);
        if (!value.Deserialize(stream))
            return false;
        if (!value.GetValue(result))
            return false;
        return true;
    }
};

template<>
class StringDeserializer<uint16_t> : public Core::IStringDeserializer<uint16_t>
{
public:
    bool Deserialize(const std::string & text, uint16_t & result) override
    {
        Number value;
        std::istringstream stream(text);
        if (!value.Deserialize(stream))
            return false;
        if (!value.GetValue(result))
            return false;
        return true;
    }
};

template<>
class StringDeserializer<int32_t> : public Core::IStringDeserializer<int32_t>
{
public:
    bool Deserialize(const std::string & text, int32_t & result) override
    {
        Number value;
        std::istringstream stream(text);
        if (!value.Deserialize(stream))
            return false;
        if (!value.GetValue(result))
            return false;
        return true;
    }
};

template<>
class StringDeserializer<uint32_t> : public Core::IStringDeserializer<uint32_t>
{
public:
    bool Deserialize(const std::string & text, uint32_t & result) override
    {
        Number value;
        std::istringstream stream(text);
        if (!value.Deserialize(stream))
            return false;
        if (!value.GetValue(result))
            return false;
        return true;
    }
};

template<>
class StringDeserializer<int64_t> : public Core::IStringDeserializer<int64_t>
{
public:
    bool Deserialize(const std::string & text, int64_t & result) override
    {
        Number value;
        std::istringstream stream(text);
        if (!value.Deserialize(stream))
            return false;
        if (!value.GetValue(result))
            return false;
        return true;
    }
};

template<>
class StringDeserializer<uint64_t> : public Core::IStringDeserializer<uint64_t>
{
public:
    bool Deserialize(const std::string & text, uint64_t & result) override
    {
        Number value;
        std::istringstream stream(text);
        if (!value.Deserialize(stream))
            return false;
        if (!value.GetValue(result))
            return false;
        return true;
    }
};

template<>
class StringDeserializer<float> : public Core::IStringDeserializer<float>
{
public:
    bool Deserialize(const std::string & text, float & result) override
    {
        Number value;
        std::istringstream stream(text);
        if (!value.Deserialize(stream))
            return false;
        if (!value.GetValue(result))
            return false;
        return true;
    }
};

template<>
class StringDeserializer<double> : public Core::IStringDeserializer<double>
{
public:
    bool Deserialize(const std::string & text, double & result) override
    {
        Number value;
        std::istringstream stream(text);
        if (!value.Deserialize(stream))
            return false;
        if (!value.GetValue(result))
            return false;
        return true;
    }
};

template<>
class StringDeserializer<long double> : public Core::IStringDeserializer<long double>
{
public:
    bool Deserialize(const std::string & text, long double & result) override
    {
        Number value;
        std::istringstream stream(text);
        if (!value.Deserialize(stream))
            return false;
        if (!value.GetValue(result))
            return false;
        return true;
    }
};

} // namepsace JSON
