#pragma once

#include <iterator>
#include <memory>
#include <string>
#include <vector>
#include "xml/XMLAttribute.h"
#include "xml/XMLNode.h"
#include "xml/XMLVisitor.h"

namespace XMLParser
{

class XMLElement
    : public XMLNode
{
public:
    typedef std::shared_ptr<XMLElement> Ptr;
    XMLElement();
    XMLElement(const std::string & value);
    XMLElement(const XMLElement & other);
    virtual ~XMLElement();

    XMLElement & operator=(const XMLElement & other);

    XMLAttribute::List Attributes() const;

    void AddAttribute(const std::string & attributeName,
                      const std::string & attributeValue,
                      XMLIgnoreCase ignoreCase = XMLIgnoreCase::MatchCase);
    void AddOrModifyAttribute(const std::string & attributeName,
                              const std::string & attributeValue,
                              XMLIgnoreCase ignoreCase = XMLIgnoreCase::MatchCase);
    void ModifyAttribute(const std::string & attributeName,
                         const std::string & attributeValue,
                         XMLIgnoreCase ignoreCase = XMLIgnoreCase::MatchCase);

    template<class T>
    bool
    CheckAttribute(const std::string & attributeName, T & value, XMLIgnoreCase ignoreCase = XMLIgnoreCase::MatchCase);
    template<class T>
    T ExpectAttribute(const std::string & attributeName, XMLIgnoreCase ignoreCase = XMLIgnoreCase::MatchCase);

    bool HaveAttribute(const std::string name, XMLIgnoreCase ignoreCase = XMLIgnoreCase::MatchCase) const;
    XMLAttribute::Ptr FindAttribute(const std::string name, XMLIgnoreCase ignoreCase = XMLIgnoreCase::MatchCase) const;

    virtual XMLNode::Ptr Clone() const;

    void ParseString(const std::string & xml,
                     XMLEncoding encoding = XMLDefaultEncoding);
    virtual std::string GenerateString(int depth = 0) const;

    virtual bool Accept(XMLVisitor * content) const;

protected:
    void Clear();
    void CopyTo(XMLElement * target) const;
    const char * Parse(const char * xml, XMLEncoding encoding);
    const char * ReadValue(const char * xml, XMLEncoding encoding);

    XMLAttribute::List attributes;

    friend class XMLDocument;
};

template<class T>
bool XMLElement::CheckAttribute(const std::string & attributeName, T & value, XMLIgnoreCase ignoreCase)
{
    XMLAttribute::Ptr attribute = FindAttribute(attributeName, ignoreCase);
    if (!attribute)
        return false;
    try
    {
        value = attribute->Value<T>();
    }
    catch (const XMLValueParseException &)
    {
        return false;
    }
    return true;
}

template<class T>
T XMLElement::ExpectAttribute(const std::string & attributeName, XMLIgnoreCase ignoreCase)
{
    XMLAttribute::Ptr attribute = FindAttribute(attributeName, ignoreCase);
    if (!attribute)
    {
        T value;
        std::ostringstream stream;
        stream << "Expected value of type " << typeid(value).name() << " for attribute " << attributeName;
        throw XMLException(stream.str());
    }
    return attribute->Value<T>();
}

} // namespace XMLParser

