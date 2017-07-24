#include "xml/XMLAttribute.h"

#include "core/Util.h"
#include "osal/Strings.h"
#include "xml/XMLDocument.h"
#include "xml/XMLParsingData.h"
#include <iostream>

using namespace std;

namespace XMLParser
{

XMLAttribute::XMLAttribute() : name(), value()
{
}

XMLAttribute::XMLAttribute(const std::string & name) : name(name), value()
{
}

XMLAttribute::XMLAttribute(const std::string & name, const std::string & value)
    : name(name), value(value)
{
}

XMLAttribute::XMLAttribute(const XMLAttribute & other) : XMLAttribute()
{
    other.CopyTo(this);
}

XMLAttribute::~XMLAttribute()
{
}

XMLAttribute & XMLAttribute::operator=(const XMLAttribute & other)
{
    other.CopyTo(this);
    return *this;
}

bool XMLAttribute::Match(const std::string & expected, XMLIgnoreCase ignoreCase) const
{
    return (ignoreCase == XMLIgnoreCase::IgnoreCase) ? OSAL::IsEqualIgnoreCase(expected, Name())
                                                     : expected == Name();
}

void XMLAttribute::ParseString(const std::string & xml, XMLEncoding encoding /*= XMLDefaultEncoding*/)
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

const char * XMLAttribute::Parse(const char * xml, XMLEncoding encoding = XMLDefaultEncoding)
{
    assert(xml != nullptr);

    xml = SkipWhiteSpace(xml, encoding);
    if ((xml == nullptr) || (*xml == '\0'))
        SetError(XMLError::ErrorParsingEmpty, nullptr);

    // Read the name, the '=' and the value.
    xml = ReadName(xml, name, encoding);
    if ((xml == nullptr) || (*xml == '\0'))
        SetError(XMLError::ErrorReadingAttributes, xml);
    xml = SkipWhiteSpace(xml, encoding);
    if (*xml != '=')
        SetError(XMLError::ErrorReadingAttributes, xml);

    ++xml; // skip '='
    xml = SkipWhiteSpace(xml, encoding);
    if (*xml == '\0')
        SetError(XMLError::ErrorReadingAttributes, xml);

    const char * SINGLE_QUOTE = "\'";
    const char * DOUBLE_QUOTE = "\"";

    if (*xml == *SINGLE_QUOTE)
    {
        ++xml;
        xml = ReadText(xml, value, false, SINGLE_QUOTE, XMLIgnoreCase::MatchCase, encoding);
    }
    else if (*xml == *DOUBLE_QUOTE)
    {
        ++xml;
        xml = ReadText(xml, value, false, DOUBLE_QUOTE, XMLIgnoreCase::MatchCase, encoding);
    }
    else
    {
        const char * END_CLOSED_TAG = "/>";
        const char * END_OPEN_TAG   = ">";
        // All attribute values should be in single or double quotes.
        // But this is such a common error that the parser will try
        // its best, even without them.
        value = "";
        while ((*xml != '\0') && !IsWhiteSpace(*xml) && (*xml != *END_CLOSED_TAG)
               && (*xml != *END_OPEN_TAG))
        {
            if ((*xml == *SINGLE_QUOTE) || (*xml == *DOUBLE_QUOTE))
            {
                value.clear();
                SetError(XMLError::ErrorReadingAttributes, xml);
            }
            value += *xml;
            ++xml;
        }
    }
    return xml;
}

std::string XMLAttribute::GenerateString(int /*depth = 0*/) const
{
    std::string nameEncoded;
    std::string valueEncoded;

    EncodeString(name, nameEncoded);
    EncodeString(value, valueEncoded);

    return nameEncoded + "=\"" + valueEncoded + "\"";
}

void XMLAttribute::CopyTo(XMLAttribute * target) const
{
    target->name  = name;
    target->value = value;
}

} // namespace XMLParser
