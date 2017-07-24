#include "xml/XMLNode.h"

#include "osal/Strings.h"
#include "xml/XMLComment.h"
#include "xml/XMLDeclaration.h"
#include "xml/XMLElement.h"
#include "xml/XMLParsingData.h"
#include "xml/XMLText.h"
#include "xml/XMLUnknown.h"
#include <iostream>

using namespace std;
using namespace XMLParser;

const XMLElement::Ptr XMLElementList::operator[](size_t index) const
{
    if (index >= size())
        return XMLElement::Ptr();
    return at(index);
}

XMLElement::Ptr XMLElementList::operator[](size_t index)
{
    if (index >= size())
        return XMLElement::Ptr();
    return at(index);
}

XMLElement::Ptr XMLElementList::Find(const std::string name,
                                     XMLIgnoreCase ignoreCase /*= XMLIgnoreCase::MatchCase*/) const
{
    for (auto child : *this)
    {
        if (child->Match(name, ignoreCase))
            return child;
    }
    return XMLElement::Ptr();
}

XMLNode::XMLNode() : value(), children()
{
}

XMLNode::XMLNode(const std::string & value) : XMLNode()
{
    this->value = value;
}

XMLNode::XMLNode(const XMLNode & other) : XMLNode()
{
    other.CopyTo(this);
}

XMLNode::~XMLNode()
{
}

XMLNode & XMLNode::operator=(const XMLNode & other)
{
    if (this != &other)
    {
        Clear();
        other.CopyTo(this);
    }
    return *this;
}

XMLNode::List XMLNode::Children() const
{
    return children;
}

XMLElementList XMLNode::Elements() const
{
    XMLElementList list;
    for (auto child : children)
    {
        XMLElement::Ptr element = dynamic_pointer_cast<XMLElement>(child);
        if (element != nullptr)
        {
            list.push_back(element);
        }
    }
    return list;
}

bool XMLNode::Match(const std::string & expected, XMLIgnoreCase ignoreCase) const
{
    return (ignoreCase == XMLIgnoreCase::MatchCase) ? expected == value
                                                    : OSAL::IsEqualIgnoreCase(expected, value);
}

XMLNode::Ptr XMLNode::FindChild(const std::string name, XMLIgnoreCase ignoreCase) const
{
    return children.Find(name, ignoreCase);
}

XMLElement::Ptr XMLNode::FindElement(const std::string name, XMLIgnoreCase ignoreCase) const
{
    for (auto child : children)
    {
        XMLElement::Ptr element = std::dynamic_pointer_cast<XMLElement>(child);
        if ((element != nullptr) && element->Match(name, ignoreCase))
            return element;
    }

    return XMLElement::Ptr();
}

void XMLNode::AddChild(XMLNode::Ptr childNode)
{
    children.push_back(childNode);
}

std::string XMLNode::GetText() const
{
    for (auto & child : Children())
    {
        XMLText * childText = dynamic_cast<XMLText *>(child.get());
        if (childText != nullptr)
        {
            return childText->Value();
        }
    }
    return "";
}

void XMLNode::Clear()
{
    XMLBase::Clear();
    value.clear();
    children.clear();
}

const XMLElement::Ptr XMLNode::FirstChildElement() const
{
    for (auto & node : children)
    {
        XMLElement::Ptr element = dynamic_pointer_cast<XMLElement>(node);
        if (element != nullptr)
            return element;
    }
    return nullptr;
}

XMLNode::Ptr XMLNode::Identify(const char * xml, XMLEncoding encoding)
{
    XMLNode::Ptr element = 0;

    // What is this thing?
    // - Elements start with a letter or underscore, but xml is reserved.
    // - Comments: <!--
    // - Declaration: <?xml
    // - Everthing else is unknown.
    //

    const char * xmlHeader     = "<?xml";
    const char * commentHeader = "<!--";
    const char * dtdHeader     = "<!";
    const char * cdataHeader   = "<![CDATA[";

    if (StringEqual(xml, xmlHeader, XMLIgnoreCase::IgnoreCase, encoding))
    {
        element = make_shared<XMLDeclaration>();
    }
    else if (StringEqual(xml, commentHeader, XMLIgnoreCase::MatchCase, encoding))
    {
        element = make_shared<XMLComment>();
    }
    else if (StringEqual(xml, cdataHeader, XMLIgnoreCase::MatchCase, encoding))
    {
        XMLText::Ptr text = make_shared<XMLText>();
        text->SetCDATA(true);
        element = text;
    }
    else if (StringEqual(xml, dtdHeader, XMLIgnoreCase::MatchCase, encoding))
    {
        element = make_shared<XMLUnknown>();
    }
    else if (IsAlpha(*(xml + 1), encoding) || (*(xml + 1) == '_'))
    {
        element = make_shared<XMLElement>();
    }
    else
    {
        element = make_shared<XMLUnknown>();
    }

    return element;
}

void XMLNode::CopyTo(XMLNode * target) const
{
    XMLBase::CopyTo(target);
    target->value = value;

    for (auto & child : Children())
    {
        target->AddChild(child->Clone());
    }
}
