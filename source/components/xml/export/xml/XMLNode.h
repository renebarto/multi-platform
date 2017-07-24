#pragma once

#include <iterator>
#include <memory>
#include <string>
#include <vector>
#include "xml/XMLBase.h"
#include "xml/XMLVisitor.h"

namespace XMLParser
{

class XMLElement;

typedef std::shared_ptr<XMLElement> XMLElementPtr;

class XMLElementList
    : public std::vector<XMLElementPtr>
{
public:
    const XMLElementPtr operator[](size_t index) const;
    XMLElementPtr operator[](size_t index);
    XMLElementPtr Find(const std::string name, XMLIgnoreCase ignoreCase = XMLIgnoreCase::MatchCase) const;
};

class XMLNode
    : public XMLBase
{
public:
    typedef std::shared_ptr<XMLNode> Ptr;

    class List
        : public std::vector<Ptr>
    {
    public:
        const Ptr operator[](size_t index) const
        {
            if (index >= size())
                return Ptr();
            return at(index);
        }

        Ptr operator[](size_t index)
        {
            if (index >= size())
                return Ptr();
            return at(index);
        }

        Ptr Find(const std::string name, XMLIgnoreCase ignoreCase = XMLIgnoreCase::MatchCase) const
        {
            for (auto child : *this)
            {
                if (child->Match(name, ignoreCase))
                    return child;
            }
            return Ptr();
        }
    };

    XMLNode();
    XMLNode(const std::string & value);
    XMLNode(const XMLNode & other);
    virtual ~XMLNode();

    XMLNode & operator=(const XMLNode & other);

    const std::string & Value() const
    {
        return value;
    }

    void SetValue(const std::string & value)
    {
        this->value = value;
    }

    XMLNode::List Children() const;
    XMLElementList Elements() const;

    bool Match(const std::string & expected, XMLIgnoreCase ignoreCase = XMLIgnoreCase::MatchCase) const;

    bool HasChildren() const
    {
        return Children().size() > 0;
    }

    XMLNode::Ptr FindChild(const std::string name, XMLIgnoreCase ignoreCase = XMLIgnoreCase::MatchCase) const;
    XMLElementPtr FindElement(const std::string name, XMLIgnoreCase ignoreCase = XMLIgnoreCase::MatchCase) const;
    void AddChild(XMLNode::Ptr childNode);

    std::string GetText() const;

    virtual XMLNode::Ptr Clone() const = 0;

    virtual bool Accept(XMLVisitor * content) const = 0;

protected:
    void Clear();
    void CopyTo(XMLNode * target) const;
    virtual const char * Parse(const char * xml, XMLEncoding encoding) = 0;
    Ptr Identify(const char * xml, XMLEncoding encoding);
    const XMLElementPtr FirstChildElement() const;

    std::string value;
    XMLNode::List children;

    friend class XMLElement;

    friend class XMLDocument;
};

} // namespace XMLParser

