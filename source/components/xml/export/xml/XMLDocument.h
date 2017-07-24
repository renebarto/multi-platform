#pragma once

#include "core/FixedArray.h"
#include "xml/XMLElement.h"
#include "xml/XMLErrorInfo.h"
#include <string>

namespace XMLParser
{

class XMLDocument : public XMLNode
{
public:
    XMLDocument();
    XMLDocument(const XMLDocument & other);

    virtual ~XMLDocument();

    XMLDocument & operator=(const XMLDocument & other);

    void LoadFile(const std::string path, XMLEncoding encoding = XMLDefaultEncoding);
    void SaveFile(const std::string path) const;
    void LoadFile(std::istream & stream, XMLEncoding encoding = XMLDefaultEncoding);
    void SaveFile(std::ostream & stream) const;

    void ParseString(const std::string & xml, XMLEncoding encoding = XMLDefaultEncoding);
    std::string GenerateString(int depth = 0) const;

    void PrintTo(std::ostream & stream, int depth = 0) const;

    const XMLElement::Ptr Root()
    {
        return FirstChildElement();
    }

    bool UseBOM()
    {
        return useMicrosoftBOM;
    }

    void UseBOM(bool value)
    {
        useMicrosoftBOM = value;
    }

    virtual bool Accept(XMLVisitor * content) const;

protected:
    bool useMicrosoftBOM;

    void CopyTo(XMLDocument * target) const;
    virtual XMLNode::Ptr Clone() const;
    virtual const char * Parse(const char * xml, XMLEncoding encoding);
    Core::FixedArray<char> LoadFileContents(std::istream & stream);

    void NormalizeLines(Core::FixedArray<char> & buffer);
};

} // namespace XMLParser
