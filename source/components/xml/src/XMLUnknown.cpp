#include "xml/XMLUnknown.h"

#include "core/Util.h"
#include "xml/XMLParsingData.h"
#include <iostream>

using namespace std;

namespace XMLParser
{

XMLUnknown::XMLUnknown() : XMLNode()
{
}

XMLUnknown::XMLUnknown(const std::string & value) : XMLNode(value)
{
}

XMLUnknown::XMLUnknown(const XMLNode::Ptr element) : XMLNode(element->Value())
{
}

XMLUnknown::XMLUnknown(const XMLUnknown & other) : XMLUnknown()
{
    other.CopyTo(this);
}

XMLUnknown::~XMLUnknown()
{
}

XMLUnknown & XMLUnknown::operator=(const XMLUnknown & other)
{
    if (this != &other)
    {
        Clear();
        other.CopyTo(this);
    }
    return *this;
}

XMLNode::Ptr XMLUnknown::Clone() const
{
    XMLUnknown * clone = new XMLUnknown();
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

void XMLUnknown::ParseString(const std::string & xml, XMLEncoding encoding)
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

std::string XMLUnknown::GenerateString(int depth /*= 0*/) const
{
    std::ostringstream stream;

    for (int i = 0; i < depth; i++)
    {
        stream << std::string(TabSize(), ' ');
    }
    stream << "<" << Value() << ">";

    return stream.str();
}

const char * XMLUnknown::Parse(const char * xml, XMLEncoding encoding)
{
    assert(xml != nullptr);

    xml = SkipWhiteSpace(xml, encoding);

    if ((xml == nullptr) || (*xml == '\0'))
        SetError(XMLError::ErrorParsingEmpty, xml);

    if (*xml != '<')
        SetError(XMLError::ErrorParsingUnknown, xml);
    ++xml;

    value = "";

    while ((*xml != '\0') && (*xml != '>'))
    {
        value += *xml;
        ++xml;
    }

    if (*xml == '>')
        return xml + 1;
    return xml;
}

bool XMLUnknown::Accept(XMLVisitor * visitor) const
{
    return visitor->Visit(*this);
}

} // namespace XMLParser
