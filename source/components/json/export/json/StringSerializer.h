#pragma once

#include <sstream>
#include "core/serialization/Serialization.h"
#include "json/Boolean.h"

namespace JSON {

template<class T>
class StringSerializer
{
public:
    void Serialize(std::string & text, const T & result) override
    {

    }
};

template <>
class StringSerializer<bool> : public Core::IStringSerializer<bool>
{
public:
    void Serialize(std::string & text, const bool & result) override
    {
        Boolean value(result);
        std::ostringstream stream;
        value.Serialize(stream);
        text = stream.str();
    }
};

} // namepsace JSON
