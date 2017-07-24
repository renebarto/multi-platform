#include "xml/XMLText.h"

#include "core/Util.h"
#include "xml/XMLParsingData.h"
#include <iostream>

using namespace std;

namespace XMLParser
{

XMLText::XMLText() : XMLNode(), cdata(false)
{
}

XMLText::XMLText(const std::string & value, bool cdata) : XMLNode(value), cdata(cdata)
{
}

XMLText::XMLText(const XMLNode::Ptr element) : XMLNode(element->Value()), cdata(false)
{
}

XMLText::XMLText(const XMLText & other) : XMLText()
{
    other.CopyTo(this);
}

XMLText::~XMLText()
{
}

XMLText & XMLText::operator=(const XMLText & other)
{
    if (this != &other)
    {
        Clear();
        other.CopyTo(this);
    }
    return *this;
}

void XMLText::Clear()
{
    XMLNode::Clear();
    cdata = false;
}

void XMLText::CopyTo(XMLText * target) const
{
    XMLNode::CopyTo(target);
    target->cdata = cdata;
}

XMLNode::Ptr XMLText::Clone() const
{
    XMLText * clone = new XMLText();
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

void XMLText::ParseString(const std::string & xml, XMLEncoding encoding)
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

std::string XMLText::GenerateString(int depth /*= 0*/) const
{
    std::ostringstream stream;

    if (cdata)
    {
        stream << std::endl;
        for (int i = 0; i < depth; i++)
        {
            stream << std::string(TabSize(), ' ');
        }
        stream << "<![CDATA[" << value << "]]>" << std::endl;
    }
    else
    {
        std::string buffer;
        EncodeString(value, buffer);
        stream << buffer;
    }

    return stream.str();
}

const char * XMLText::Parse(const char * xml, XMLEncoding encoding)
{
    assert(xml != nullptr);

    if ((xml == nullptr) || (*xml == '\0'))
    {
        SetError(XMLError::ErrorParsingEmpty, xml);
    }

    value = "";

    const char * const startTag = "<![CDATA[";

    if (cdata || StringEqual(xml, startTag, XMLIgnoreCase::MatchCase, encoding))
    {
        cdata = true;

        if (!StringEqual(xml, startTag, XMLIgnoreCase::MatchCase, encoding))
        {
            cdata = false;
            SetError(XMLError::ErrorParsingCDATA, xml);
        }
        xml += strlen(startTag);

        const char * const endTag = "]]>";

        // Keep all the white space, ignore the encoding, etc.
        while ((*xml != '\0') && !StringEqual(xml, endTag, XMLIgnoreCase::MatchCase, encoding))
        {
            value += *xml;
            ++xml;
        }
        if (!StringEqual(xml, endTag, XMLIgnoreCase::MatchCase, encoding))
        {
            value = "";
            cdata = false;
            SetError(XMLError::ErrorParsingCDATA, xml);
        }
        xml += strlen(endTag);

        std::string dummy;
        xml = ReadText(xml, dummy, false, endTag, XMLIgnoreCase::MatchCase, encoding);
        return xml;
    }
    const char * end = "<";
    xml              = ReadText(xml, value, false, end, XMLIgnoreCase::MatchCase, encoding);
    if (xml != nullptr)
    {
        if (*xml != '\0')
            return xml - 1; // don't truncate the '<'
        else
            return xml;
    }
    return nullptr;
}

bool XMLText::IsBlank() const
{
    for (size_t i = 0; i < value.length(); i++)
        if (!IsWhiteSpace(value[i]))
            return false;
    return true;
}

bool XMLText::Accept(XMLVisitor * visitor) const
{
    return visitor->Visit(*this);
}

} // namespace XMLParser
