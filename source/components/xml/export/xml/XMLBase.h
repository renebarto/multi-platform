#pragma once

#include <cassert>
#include <memory>
#include "xml/XMLDefs.h"
#include "xml/XMLErrorInfo.h"

namespace XMLParser
{

class XMLDocument;

class XMLParsingData;

class XMLBase
{
public:
    typedef std::shared_ptr<XMLBase> Ptr;

    XMLBase();
    virtual ~XMLBase();

    int Row() const
    {
        return location.Row() + 1;
    }

    int Column() const
    {
        return location.Col() + 1;
    }

    void SetLocation(const XMLCursor & location)
    {
        this->location = location;
    }

    void SetUserData(void * value)
    {
        this->userData = value;
    }

    void * GetUserData()
    {
        return userData;
    }

    const void * GetUserData() const
    {
        return userData;
    }

    static void EncodeString(const std::string & in, std::string & out);

    virtual void ParseString(const std::string & xml, XMLEncoding encoding = XMLDefaultEncoding) = 0;
    virtual std::string GenerateString(int depth = 0) const = 0;

    static bool IsWhiteSpaceCondensed()
    {
        return condenseWhiteSpace;
    }

    static void SetCondenseWhiteSpace(bool value)
    {
        condenseWhiteSpace = value;
    }

    static int TabSize()
    {
        return tabSize;
    }

    static void SetTabSize(int value)
    {
        tabSize = value;
    }

    const XMLDocument * OwningDocument()
    {
        return owningDocument;
    }

    void SetOwningDocument(XMLDocument * document)
    {
        owningDocument = document;
    }

    void SetError(XMLError err, const char * xml);

protected:
    void Clear();
    void CopyTo(XMLBase * target) const;

    virtual const char * Parse(const char * xml, XMLEncoding encoding) = 0;

    static const char * SkipWhiteSpace(const char * xml, XMLEncoding encoding);

    inline static bool IsWhiteSpace(char c)
    {
        return (isspace((unsigned char)c) || c == '\n' || c == '\r');
    }

    static bool IsAlpha(char anyChar, XMLEncoding encoding);
    static bool IsAlphaNum(char anyChar, XMLEncoding encoding);
    static const char * ReadName(const char * xml, std::string & name, XMLEncoding encoding);
    static const char * ReadText(const char * xml,         // where to start
                                 std::string & text,       // the string read
                                 bool ignoreWhiteSpace,    // whether to keep the white space
                                 const char * endTag,      // what ends this text
                                 XMLIgnoreCase ignoreCase, // whether to ignore case in the end tag
                                 XMLEncoding encoding);    // the current encoding
    static bool
    IsEndTag(const char * xml, const char * endTag, XMLIgnoreCase ignoreCase, XMLEncoding encoding);

    inline static int ToLower(int v, XMLEncoding encoding)
    {
        if (encoding == XMLEncoding::UTF8)
        {
            if (v < 128)
                return tolower(v);
            return v;
        }
        else
        {
            return tolower(v);
        }
    }

    // Get a character, while interpreting entities.
    // The length can be from 0 to 4 bytes.
    static const char * GetChar(const char * xml, char * value, int & length, XMLEncoding encoding);
    static bool
    StringEqual(const char * xml, const char * tag, XMLIgnoreCase ignoreCase, XMLEncoding encoding);

    void * userData;
    XMLCursor location;
    XMLDocument * owningDocument;
    static bool condenseWhiteSpace;
    static int tabSize;

    friend class XMLParsingData;

    static const int UTF8ByteTable[256];
};

} // namespace XMLParser
