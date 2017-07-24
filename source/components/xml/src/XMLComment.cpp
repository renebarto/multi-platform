#include "xml/XMLComment.h"

#include "core/Util.h"
#include "xml/XMLParsingData.h"
#include <iostream>

using namespace std;

namespace XMLParser
{

XMLComment::XMLComment() : XMLNode()
{
}

XMLComment::XMLComment(const std::string & value) : XMLNode(value)
{
}

XMLComment::XMLComment(const XMLNode::Ptr element) : XMLNode(element->Value())
{
}

XMLComment::XMLComment(const XMLComment & other) : XMLComment()
{
    other.CopyTo(this);
}

XMLComment::~XMLComment()
{
}

XMLComment & XMLComment::operator=(const XMLComment & other)
{
    if (this != &other)
    {
        Clear();
        other.CopyTo(this);
    }
    return *this;
}

XMLNode::Ptr XMLComment::Clone() const
{
    XMLComment * clone = new XMLComment();
    if (clone == nullptr)
        return nullptr;

    try
    {
        CopyTo(clone);
    }
    catch (...)
    {
        delete clone;
        throw;
    }
    return XMLNode::Ptr(clone);
}

void XMLComment::ParseString(const std::string & xml, XMLEncoding encoding)
{
    try
    {
        Parse(xml.c_str(), encoding);
    }
    catch (const XMLInternalException & e)
    {
        XMLParsingData parseData(xml.c_str());
        parseData.Update(e.ErrorPosition(), encoding);
        XMLErrorInfo errorInfo(e.ErrorCode(), parseData.Cursor());
        throw XMLParseException(e.ErrorPosition(), errorInfo);
    }
}

std::string XMLComment::GenerateString(int depth /*= 0*/) const
{
    std::ostringstream stream;

    for (int i = 0; i < depth; i++)
    {
        stream << std::string(TabSize(), ' ');
    }
    stream << "<!--" << value << "-->";

    return stream.str();
}

const char * XMLComment::Parse(const char * xml, XMLEncoding encoding)
{
    assert(xml != nullptr);

    xml = SkipWhiteSpace(xml, encoding);

    if ((xml == nullptr) || (*xml == '\0'))
    {
        SetError(XMLError::ErrorParsingEmpty, xml);
    }

    value = "";

    const char * startTag = "<!--";
    const char * endTag   = "-->";

    if (!StringEqual(xml, startTag, XMLIgnoreCase::MatchCase, encoding))
    {
        SetError(XMLError::ErrorParsingComment, xml);
    }
    xml += strlen(startTag);

    // Keep all the white space.
    while ((*xml != '\0') && !StringEqual(xml, endTag, XMLIgnoreCase::MatchCase, encoding))
    {
        value += *xml;
        ++xml;
    }
    if (!StringEqual(xml, endTag, XMLIgnoreCase::MatchCase, encoding))
    {
        value = "";
        SetError(XMLError::ErrorParsingComment, xml);
    }
    xml += strlen(endTag);

    return xml;
}

bool XMLComment::Accept(XMLVisitor * visitor) const
{
    return visitor->Visit(*this);
}

} // namespace XMLParser
