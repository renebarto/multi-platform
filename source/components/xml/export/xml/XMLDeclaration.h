#pragma once

#include "xml/XMLNode.h"
#include "xml/XMLVisitor.h"
#include <memory>

namespace XMLParser
{

class XMLDeclaration : public XMLNode
{
public:
    typedef std::shared_ptr<XMLDeclaration> Ptr;

    XMLDeclaration();
    XMLDeclaration(const std::string & version,
                   const std::string & encoding,
                   const std::string & standalone);
    XMLDeclaration(const XMLDeclaration & other);
    virtual ~XMLDeclaration();

    XMLDeclaration & operator=(const XMLDeclaration & other);

    virtual XMLNode::Ptr Clone() const;

    const std::string & Version() const
    {
        return version;
    }

    const std::string & Encoding() const
    {
        return encoding;
    }

    const std::string & Standalone() const
    {
        return standalone;
    }

    void ParseString(const std::string & xml, XMLEncoding encoding = XMLDefaultEncoding);
    virtual std::string GenerateString(int depth = 0) const;

    virtual bool Accept(XMLVisitor * content) const;

protected:
    void CopyTo(XMLDeclaration * target) const;
    virtual const char * Parse(const char * xml, XMLEncoding encoding);

    std::string version;
    std::string encoding;
    std::string standalone;
};

} // namespace XMLParser
