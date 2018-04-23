#pragma once

#include <sstream>
#include "core/serialization/Serialization.h"
#include "json/Boolean.h"
#include "json/Number.h"
#include "json/Array.h"
#include "json/String.h"

namespace JSON {

template<class T>
class StringSerializer
{
public:
    void Serialize(std::string & text, const T & result) override
    {

    }
};

template<>
class StringSerializer<std::string> : public Core::IStringSerializer<std::string>
{
public:
    void Serialize(std::string & text, const std::string & value) override
    {
        String variable(value);
        std::ostringstream stream;
        variable.Serialize(stream);
        text = stream.str();
    }
};

template<>
class StringSerializer<Array> : public Core::IStringSerializer<Array>
{
public:
    void Serialize(std::string & text, const Array & value) override
    {
        Array variable(value);
        std::ostringstream stream;
        variable.Serialize(stream);
        text = stream.str();
    }
};

template <>
class StringSerializer<bool> : public Core::IStringSerializer<bool>
{
public:
    void Serialize(std::string & text, const bool & value) override
    {
        Boolean variable(value);
        std::ostringstream stream;
        variable.Serialize(stream);
        text = stream.str();
    }
};

template<>
class StringSerializer<int8_t> : public Core::IStringSerializer<int8_t>
{
public:
    void Serialize(std::string & text, const int8_t & value) override
    {
        Number variable(value);
        std::ostringstream stream;
        variable.Serialize(stream);
        text = stream.str();
    }
};

template<>
class StringSerializer<uint8_t> : public Core::IStringSerializer<uint8_t>
{
public:
    void Serialize(std::string & text, const uint8_t & value) override
    {
        Number variable(value);
        std::ostringstream stream;
        variable.Serialize(stream);
        text = stream.str();
    }
};

template<>
class StringSerializer<int16_t> : public Core::IStringSerializer<int16_t>
{
public:
    void Serialize(std::string & text, const int16_t & value) override
    {
        Number variable(value);
        std::ostringstream stream;
        variable.Serialize(stream);
        text = stream.str();
    }
};

template<>
class StringSerializer<uint16_t> : public Core::IStringSerializer<uint16_t>
{
public:
    void Serialize(std::string & text, const uint16_t & value) override
    {
        Number variable(value);
        std::ostringstream stream;
        variable.Serialize(stream);
        text = stream.str();
    }
};

template<>
class StringSerializer<int32_t> : public Core::IStringSerializer<int32_t>
{
public:
    void Serialize(std::string & text, const int32_t & value) override
    {
        Number variable(value);
        std::ostringstream stream;
        variable.Serialize(stream);
        text = stream.str();
    }
};

template<>
class StringSerializer<uint32_t> : public Core::IStringSerializer<uint32_t>
{
public:
    void Serialize(std::string & text, const uint32_t & value) override
    {
        Number variable(value);
        std::ostringstream stream;
        variable.Serialize(stream);
        text = stream.str();
    }
};

template<>
class StringSerializer<int64_t> : public Core::IStringSerializer<int64_t>
{
public:
    void Serialize(std::string & text, const int64_t & value) override
    {
        Number variable(value);
        std::ostringstream stream;
        variable.Serialize(stream);
        text = stream.str();
    }
};

template<>
class StringSerializer<uint64_t> : public Core::IStringSerializer<uint64_t>
{
public:
    void Serialize(std::string & text, const uint64_t & value) override
    {
        Number variable(value);
        std::ostringstream stream;
        variable.Serialize(stream);
        text = stream.str();
    }
};

template<>
class StringSerializer<float> : public Core::IStringSerializer<float>
{
public:
    void Serialize(std::string & text, const float & value) override
    {
        Number variable(value);
        std::ostringstream stream;
        variable.Serialize(stream);
        text = stream.str();
    }
};

template<>
class StringSerializer<double> : public Core::IStringSerializer<double>
{
public:
    void Serialize(std::string & text, const double & value) override
    {
        Number variable(value);
        std::ostringstream stream;
        variable.Serialize(stream);
        text = stream.str();
    }
};

template<>
class StringSerializer<long double> : public Core::IStringSerializer<long double>
{
public:
    void Serialize(std::string & text, const long double & value) override
    {
        Number variable(value);
        std::ostringstream stream;
        variable.Serialize(stream);
        text = stream.str();
    }
};

} // namepsace JSON
