#pragma once

#include <memory>
#include "xml/XMLNode.h"
#include "xml/XMLVisitor.h"

namespace XMLParser
{

class XMLUnknown
    : public XMLNode
{
public:
    typedef std::shared_ptr<XMLUnknown> Ptr;

    XMLUnknown();
    XMLUnknown(const std::string & value);
    XMLUnknown(const XMLNode::Ptr element);
    XMLUnknown(const XMLUnknown &);
    virtual ~XMLUnknown();

    XMLUnknown & operator=(const XMLUnknown &);

    virtual XMLNode::Ptr Clone() const;

    void ParseString(const std::string & xml,
                     XMLEncoding encoding = XMLDefaultEncoding);
    virtual std::string GenerateString(int depth = 0) const;

    virtual bool Accept(XMLVisitor * content) const;

protected:
    virtual const char * Parse(const char * xml, XMLEncoding encoding);
};

} // namespace XMLParser

