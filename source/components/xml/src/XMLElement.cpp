#include "xml/XMLElement.h"

#include "xml/XMLComment.h"
#include "xml/XMLDeclaration.h"
#include "xml/XMLParsingData.h"
#include "xml/XMLText.h"
#include "xml/XMLUnknown.h"
#include <iostream>

using namespace std;
using namespace XMLParser;

XMLElement::XMLElement() : attributes()
{
}

XMLElement::XMLElement(const std::string & value) : XMLElement()
{
    this->value = value;
}

XMLElement::XMLElement(const XMLElement & other) : XMLElement()
{
    other.CopyTo(this);
}

XMLElement::~XMLElement()
{
}

XMLElement & XMLElement::operator=(const XMLElement & other)
{
    if (this != &other)
    {
        Clear();
        other.CopyTo(this);
    }
    return *this;
}

XMLAttribute::List XMLElement::Attributes() const
{
    return attributes;
}

void XMLElement::AddAttribute(const std::string & attributeName,
                              const std::string & attributeValue,
                              XMLIgnoreCase ignoreCase /*= XMLIgnoreCase::MatchCase*/)
{
    XMLAttribute::Ptr attribute = attributes.Find(attributeName, ignoreCase);
    if (attribute != nullptr)
    {
        throw XMLException("AddAttribute: Already existing attribute: " + attributeName);
    }
    attribute = make_shared<XMLAttribute>(attributeName, attributeValue);
    attributes.push_back(attribute);
}

void XMLElement::AddOrModifyAttribute(const std::string & attributeName,
                                      const std::string & attributeValue,
                                      XMLIgnoreCase ignoreCase /*= XMLIgnoreCase::MatchCase*/)
{
    XMLAttribute::Ptr attribute = attributes.FindOrCreate(attributeName, ignoreCase);
    if (attribute == nullptr)
        throw OSAL::Exception(__func__, __FILE__, __LINE__, "Unexpected NULL pointer.");
    attribute->SetValue(attributeValue);
}

void XMLElement::ModifyAttribute(const std::string & attributeName,
                                 const std::string & attributeValue,
                                 XMLIgnoreCase ignoreCase /*= XMLIgnoreCase::MatchCase*/)
{
    XMLAttribute::Ptr attribute = attributes.Find(attributeName, ignoreCase);
    if (attribute == nullptr)
        throw XMLException("ModifyAttribute: Non-existing attribute: " + attributeName);
    attribute->SetValue(attributeValue);
}

bool XMLElement::HaveAttribute(const std::string name, XMLIgnoreCase ignoreCase) const
{
    return attributes.Find(name, ignoreCase) != nullptr;
}

XMLAttribute::Ptr XMLElement::FindAttribute(const std::string name, XMLIgnoreCase ignoreCase) const
{
    return attributes.Find(name, ignoreCase);
}

void XMLElement::Clear()
{
    XMLNode::Clear();
    attributes.clear();
}

void XMLElement::ParseString(const std::string & xml, XMLEncoding encoding)
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

std::string XMLElement::GenerateString(int depth /*= 0*/) const
{
    std::ostringstream stream;

    for (int i = 0; i < depth; i++)
    {
        stream << std::string(TabSize(), ' ');
    }

    stream << "<" << value;

    for (auto & attribute : Attributes())
    {
        stream << " " << attribute->GenerateString(depth);
    }

    // There are 3 different formatting approaches:
    // 1) An element without children is printed as a <foo /> node
    // 2) An element with only a text child is printed as <foo> text </foo>
    // 3) An element with children is printed on multiple lines.
    if (children.size() == 0)
    {
        stream << " />";
    }
    else if ((children.size() == 1) && (std::dynamic_pointer_cast<XMLText>(children[0]) != nullptr))
    {
        stream << ">";
        stream << children[0]->GenerateString(depth + 1);
        stream << "</" << value << ">";
    }
    else
    {
        stream << ">";
        for (auto & child : children)
        {
            if (std::dynamic_pointer_cast<XMLText>(child) == nullptr)
            {
                stream << std::endl;
            }
            stream << child->GenerateString(depth + 1);
        }
        stream << std::endl;
        for (int i = 0; i < depth; i++)
        {
            stream << std::string(TabSize(), ' ');
        }
        stream << "</" << value << ">";
    }
    return stream.str();
}

const char * XMLElement::Parse(const char * xml, XMLEncoding encoding)
{
    assert(xml != nullptr);
    xml = SkipWhiteSpace(xml, encoding);

    if ((xml == nullptr) || (*xml == '\0'))
    {
        SetError(XMLError::ErrorParsingEmpty, xml);
    }
    if (*xml != '<')
        SetError(XMLError::ErrorParsingElement, xml);
    ++xml;

    // Read the name.
    const char * str = xml;

    xml = ReadName(xml, value, encoding);
    if ((xml == nullptr) || (*xml == '\0'))
        SetError(XMLError::ErrorReadingElementValue, str);

    const char * ClosedTagEnd = "/>";
    const char * OpenTagEnd   = ">";
    std::string endTag("</");
    endTag += value;

    // Check for and read attributes. Also look for an empty
    // tag or an end tag.
    while ((xml != nullptr) && (*xml != '\0'))
    {
        str = xml;
        xml = SkipWhiteSpace(xml, encoding);
        if (*xml == '\0')
            SetError(XMLError::ErrorReadingAttributes, str);
        if (*xml == *ClosedTagEnd)
        {
            if (!StringEqual(xml, ClosedTagEnd, XMLIgnoreCase::IgnoreCase, encoding))
            {
                SetError(XMLError::ErrorParsingElement, str);
            }
            return xml + strlen(ClosedTagEnd);
        }
        else if (*xml == *OpenTagEnd)
        {
            // Done with attributes (if there were any.)
            // Read the value -- which can include other
            // elements -- read the end tag, and return.
            ++xml;
            str = xml;
            xml = ReadValue(xml, encoding);
            if ((xml == nullptr) || (*xml == '\0'))
                SetError(XMLError::ErrorReadingEndTag, str);

            // We should find the end tag now
            // note that:
            // </foo > and
            // </foo>
            // are both valid end tags.
            if (StringEqual(xml, endTag.c_str(), XMLIgnoreCase::MatchCase, encoding))
            {
                xml += endTag.length();
                xml = SkipWhiteSpace(xml, encoding);
                if ((xml != nullptr) && *xml == '>')
                {
                    ++xml;
                    return xml;
                }
            }
            SetError(XMLError::ErrorReadingEndTag, xml);
        }
        else
        {
            // Try to read an attribute:
            XMLAttribute::Ptr attribute = make_shared<XMLAttribute>();
            assert(attribute != nullptr);

            str = xml;
            xml = attribute->Parse(xml, encoding);

            if ((xml == nullptr) || (*xml == '\0'))
                SetError(XMLError::ErrorReadingAttributes, str);

            // Handle the strange case of double attributes:
            XMLAttribute::Ptr existingAttribute = attributes.Find(attribute->Name());
            if (existingAttribute != nullptr)
                SetError(XMLError::ErrorDuplicateAttribute, str);

            attributes.push_back(attribute);
        }
    }
    return xml;
}

const char * XMLElement::ReadValue(const char * xml, XMLEncoding encoding)
{
    // Read in text and elements in any order.
    const char * xmlWithWhiteSpace = xml;
    xml                            = SkipWhiteSpace(xml, encoding);

    const char * TagStart    = "<";
    const char * EndTagStart = "</";

    while ((xml != nullptr) && (*xml != '\0'))
    {
        if (*xml != *TagStart)
        {
            // Take what we have, make a text element.
            XMLText::Ptr text = make_shared<XMLText>();

            assert(text != nullptr);

            if (XMLBase::IsWhiteSpaceCondensed())
            {
                xml = text->Parse(xml, encoding);
            }
            else
            {
                // Special case: we want to keep the white space
                // so that leading spaces aren't removed.
                xml = text->Parse(xmlWithWhiteSpace, encoding);
            }

            if (!text->IsBlank())
                AddChild(text);
        }
        else
        {
            // We hit a '<'
            // Have we hit a new element or an end tag? This could also be
            // a XMLText in the "CDATA" style.
            if (StringEqual(xml, EndTagStart, XMLIgnoreCase::MatchCase, encoding))
            {
                return xml;
            }
            else
            {
                XMLNode::Ptr node = Identify(xml, encoding);
                assert(node != nullptr);

                xml = node->Parse(xml, encoding);
                AddChild(node);
            }
        }
        xmlWithWhiteSpace = xml;
        xml               = SkipWhiteSpace(xml, encoding);
    }

    if (xml == nullptr)
    {
        SetError(XMLError::ErrorReadingElementValue, xmlWithWhiteSpace);
    }
    return xml;
}

void XMLElement::CopyTo(XMLElement * target) const
{
    XMLBase::CopyTo(target);
    target->value = value;

    for (auto & child : Children())
    {
        target->AddChild(child->Clone());
    }
    for (auto & attribute : Attributes())
    {
        target->AddAttribute(attribute->Name(), attribute->Value());
    }
}

XMLNode::Ptr XMLElement::Clone() const
{
    XMLElement * clone = new XMLElement();
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

bool XMLElement::Accept(XMLVisitor * visitor) const
{
    if (visitor->VisitEnter(*this, Attributes()))
    {
        for (auto & child : Children())
        {
            if (!child->Accept(visitor))
                break;
        }
    }
    return visitor->VisitExit(*this);
}
