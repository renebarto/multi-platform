#pragma once

#include <memory>
#include <string>
#include <vector>
#include <core/Core.h>
#include <core/Util.h>
#include <core/serialization/DeserializationImpl.h>
#include <network/IPV4Address.h>
#include <network/IPV6Address.h>
#include "xml/XMLBase.h"

namespace XMLParser
{

class XMLDocument;

class XMLParsingData;

class XMLAttribute : public XMLBase
{
public:
    typedef std::shared_ptr<XMLAttribute> Ptr;

    class List : public std::vector<Ptr>
    {
    public:
        const Ptr operator[](size_t index)
        {
            if (index >= size())
                return Ptr();
            return at(index);
        }

        Ptr Find(const std::string name, XMLIgnoreCase ignoreCase = XMLIgnoreCase::MatchCase) const
        {
            for (auto attribute : *this)
            {
                if (attribute->Match(name, ignoreCase))
                    return attribute;
            }
            return Ptr();
        }

        Ptr FindOrCreate(const std::string name, XMLIgnoreCase ignoreCase = XMLIgnoreCase::MatchCase)
        {
            Ptr attribute = Find(name, ignoreCase);
            if (attribute != nullptr)
            {
                return attribute;
            }
            return Create(name);
        }

        Ptr Create(const std::string name)
        {
            Ptr attribute = std::make_shared<XMLAttribute>(name);
            push_back(attribute);
            return attribute;
        }
    };

    XMLAttribute();

    XMLAttribute(const std::string & name);

    XMLAttribute(const std::string & name, const std::string & value);

    XMLAttribute(const XMLAttribute & other);

    virtual ~XMLAttribute();

    XMLAttribute & operator=(const XMLAttribute & other);

    const std::string Name() const
    {
        return name;
    }

    bool Match(const std::string & expected,
               XMLIgnoreCase ignoreCase = XMLIgnoreCase::MatchCase) const;

    const std::string Value() const
    {
        return value;
    }

    template <class T>
    T Value() const;

    void SetValue(const std::string & value)
    {
        this->value = value;
    }

    void ParseString(const std::string & xml, XMLEncoding encoding = XMLDefaultEncoding);

    virtual std::string GenerateString(int depth = 0) const;

protected:
    std::string name;
    std::string value;

    void CopyTo(XMLAttribute * target) const;

    const char * Parse(const char * xml, XMLEncoding encoding);

    friend class XMLDeclaration;

    friend class XMLElement;
};

template <>
inline std::string XMLAttribute::Value<std::string>() const
{
    return Value();
}

template <>
inline Network::IPV4Address XMLAttribute::Value<Network::IPV4Address>() const
{
    Network::IPV4Address result;

    if (!Network::IPV4Address::TryParse(Value(), result))
    {
        std::ostringstream stream;
        stream << "Error parsing IP address: " << Value();
        throw XMLValueParseException(stream.str());
    }
    return result;
}

template <class T>
inline T XMLAttribute::Value() const
{
    T result;

    if (!Core::Deserialize(Value(), result))
    {
        std::ostringstream stream;
        stream << "Error parsing value: " << Value();
        throw XMLValueParseException(stream.str());
    }
    return result;
}

} // namespace XMLParser
