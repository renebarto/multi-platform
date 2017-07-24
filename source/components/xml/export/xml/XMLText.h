#pragma once

#include <memory>
#include "xml/XMLNode.h"
#include "xml/XMLVisitor.h"

namespace XMLParser
{

class XMLText
    : public XMLNode
{
public:
    typedef std::shared_ptr<XMLText> Ptr;

    XMLText();
    XMLText(const std::string & value, bool cdata = false);
    XMLText(const XMLNode::Ptr element);
    XMLText(const XMLText &);
    virtual ~XMLText();

    XMLText & operator=(const XMLText &);

    virtual XMLNode::Ptr Clone() const;

    bool CDATA() const
    {
        return cdata;
    }

    void SetCDATA(bool cdata)
    {
        this->cdata = cdata;
    }

    void ParseString(const std::string & xml,
                     XMLEncoding encoding = XMLDefaultEncoding);
    virtual std::string GenerateString(int depth = 0) const;

    virtual bool Accept(XMLVisitor * content) const;

protected:
    void Clear();
    void CopyTo(XMLText * target) const;
    virtual const char * Parse(const char * xml, XMLEncoding encoding);
    bool IsBlank() const; // Returns true if empty or all white space and new lines

    bool cdata;    // True if this should be input and output as a CDATA style text element

    friend class XMLElement;
};

} // namespace XMLParser

