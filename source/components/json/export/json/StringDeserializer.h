#pragma once

#include "core/serialization/Serialization.h"
#include "json/Boolean.h"

namespace JSON {

template<class T>
class StringDeserializer : public Core::IStringDeserializer<T>
{
public:
    bool Deserialize(const std::string & text, T & result) override
    {

    }
};

class StringDeserializer<bool> : public Core::IStringDeserializer<bool>
{
public:
    bool Deserialize(const std::string & text, T & result) override
    {
        Boolean value;
        std::istringstream stream(text);
        if (!value.Deserialize(stream))
            return false;
        result = value.GetValue();
        return true;
    }
};

} // namepsace JSON
