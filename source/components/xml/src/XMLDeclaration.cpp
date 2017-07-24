#include "xml/XMLDeclaration.h"

#include "core/Util.h"
#include "osal/Strings.h"
#include "xml/XMLParsingData.h"
#include <iostream>

using namespace std;

namespace XMLParser
{

XMLDeclaration::XMLDeclaration() : XMLNode()
{
}

XMLDeclaration::XMLDeclaration(const std::string & version,
                               const std::string & encoding,
                               const std::string & standalone)
    : XMLNode(value), version(version), encoding(encoding), standalone(standalone)
{
}

XMLDeclaration::XMLDeclaration(const XMLDeclaration & other) : XMLDeclaration()
{
    other.CopyTo(this);
}

XMLDeclaration::~XMLDeclaration()
{
}

XMLDeclaration & XMLDeclaration::operator=(const XMLDeclaration & other)
{
    if (this != &other)
    {
        Clear();
        other.CopyTo(this);
    }
    return *this;
}

XMLNode::Ptr XMLDeclaration::Clone() const
{
    XMLDeclaration * clone = new XMLDeclaration();
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

void XMLDeclaration::CopyTo(XMLDeclaration * target) const
{
    XMLNode::CopyTo(target);

    target->version    = version;
    target->encoding   = encoding;
    target->standalone = standalone;
}

void XMLDeclaration::ParseString(const std::string & xml, XMLEncoding encoding)
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

std::string XMLDeclaration::GenerateString(int /*depth = 0*/) const
{
    std::ostringstream stream;

    stream << "<?xml ";
    if (!version.empty())
    {
        stream << "version=\"" << version << "\" ";
    }
    if (!encoding.empty())
    {
        stream << "encoding=\"" << encoding << "\" ";
    }
    if (!standalone.empty())
    {
        stream << "standalone=\"" << standalone << "\" ";
    }
    stream << "?>";

    return stream.str();
}

const char * XMLDeclaration::Parse(const char * xml, XMLEncoding encoding)
{
    assert(xml != nullptr);

    xml = SkipWhiteSpace(xml, encoding);
    if ((xml == nullptr) || (*xml == '\0'))
    {
        SetError(XMLError::ErrorParsingEmpty, xml);
    }

    this->value      = "";
    this->version    = "";
    this->encoding   = "";
    this->standalone = "";

    const char * startTag = "<?xml";
    const char * endTag   = "?>";

    if (!StringEqual(xml, startTag, XMLIgnoreCase::MatchCase, encoding))
    {
        SetError(XMLError::ErrorParsingDeclaration, xml);
    }
    xml += strlen(startTag);
    xml = SkipWhiteSpace(xml, encoding);

    // Keep all the white space.
    while ((*xml != '\0') && !StringEqual(xml, endTag, XMLIgnoreCase::MatchCase, encoding))
    {
        XMLAttribute attribute;
        xml = attribute.Parse(xml, encoding);
        if (xml == nullptr)
        {
            this->version    = "";
            this->encoding   = "";
            this->standalone = "";
            SetError(XMLError::ErrorParsingDeclaration, xml);
        }
        if (OSAL::IsEqualIgnoreCase(attribute.Name(), "version"))
        {
            this->version = attribute.Value();
        }
        else if (OSAL::IsEqualIgnoreCase(attribute.Name(), "encoding"))
        {
            this->encoding = attribute.Value();
        }
        else if (OSAL::IsEqualIgnoreCase(attribute.Name(), "standalone"))
        {
            this->standalone = attribute.Value();
        }
        xml = SkipWhiteSpace(xml, encoding);
    }
    if (!StringEqual(xml, endTag, XMLIgnoreCase::MatchCase, encoding))
    {
        this->version    = "";
        this->encoding   = "";
        this->standalone = "";
        SetError(XMLError::ErrorParsingDeclaration, xml);
    }
    xml += strlen(endTag);
    return xml;
}

bool XMLDeclaration::Accept(XMLVisitor * visitor) const
{
    return visitor->Visit(*this);
}

} // namespace XMLParser
