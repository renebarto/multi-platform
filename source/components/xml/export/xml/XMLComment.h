#pragma once

#include "xml/XMLNode.h"
#include "xml/XMLVisitor.h"
#include <memory>

namespace XMLParser
{

class XMLComment : public XMLNode
{
public:
    typedef std::shared_ptr<XMLComment> Ptr;

    XMLComment();
    XMLComment(const std::string & value);
    XMLComment(const XMLNode::Ptr element);
    XMLComment(const XMLComment & other);
    virtual ~XMLComment();

    XMLComment & operator=(const XMLComment & other);

    virtual XMLNode::Ptr Clone() const;

    void ParseString(const std::string & xml, XMLEncoding encoding = XMLDefaultEncoding);
    virtual std::string GenerateString(int depth = 0) const;

    virtual bool Accept(XMLVisitor * content) const;

protected:
    virtual const char * Parse(const char * xml, XMLEncoding encoding);
};

} // namespace XMLParser
