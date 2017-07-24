#include "xml/XMLBase.h"

#include "xml/XMLDocument.h"
#include "xml/XMLParsingData.h"
#include <cstring>
#include <ctype.h>

using namespace XMLParser;

namespace XMLParser
{

struct Entity
{
    const char * str;
    unsigned int length;
    char chr;
};
}

static const int NumEntities                   = 5;
static XMLParser::Entity entities[NumEntities] = {
    {"&amp;", 5, '&'}, {"&lt;", 4, '<'}, {"&gt;", 4, '>'}, {"&quot;", 6, '\"'}, {"&apos;", 6, '\''}};

bool XMLBase::condenseWhiteSpace = false;
int XMLBase::tabSize             = 4;

// Table that returns, for a given lead byte, the total number of bytes
// in the UTF-8 sequence.
const int XMLBase::UTF8ByteTable[256]
    = {
        //	0	1	2	3	4	5	6	7	8	9	a	b
        //c	d	e	f
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, // 0x00
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, // 0x10
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, // 0x20
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, // 0x30
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, // 0x40
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, // 0x50
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, // 0x60
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, // 0x70	End of ASCII range
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, // 0x80 0x80 to 0xc1 invalid
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, // 0x90
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, // 0xa0
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, // 0xb0
        1, 1, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, // 0xc0 0xc2 to 0xdf 2 byte
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, // 0xd0
        3, 3, 3, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 3, // 0xe0 0xe0 to 0xef 3 byte
        4, 4, 4, 4, 4, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1 // 0xf0 0xf0 to 0xf4 4 byte, 0xf5 and higher invalid
};

static void ConvertUTF32ToUTF8(unsigned long input, char * output, int & length)
{
    const unsigned long BYTE_MASK          = 0xBF;
    const unsigned long BYTE_MARK          = 0x80;
    const unsigned long FIRST_BYTE_MARK[7] = {0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC};

    if (input < 0x80)
        length = 1;
    else if (input < 0x800)
        length = 2;
    else if (input < 0x10000)
        length = 3;
    else if (input < 0x200000)
        length = 4;
    else
    {
        length = 0;
        return;
    }

    output += length;

    // Scary scary fall throughs.
    switch (length)
    {
    case 4:
        --output;
        *output = (char)((input | BYTE_MARK) & BYTE_MASK);
        input >>= 6;
    case 3:
        --output;
        *output = (char)((input | BYTE_MARK) & BYTE_MASK);
        input >>= 6;
    case 2:
        --output;
        *output = (char)((input | BYTE_MARK) & BYTE_MASK);
        input >>= 6;
    case 1:
        --output;
        *output = (char)(input | FIRST_BYTE_MARK[length]);
    }
}

static const char * GetEntity(const char * xml, char * value, int & length, XMLEncoding encoding)
{
    length = 0;

    if ((*(xml + 1) == '#') && (*(xml + 2) != '\0'))
    {
        unsigned long ucs    = 0;
        std::ptrdiff_t delta = 0;
        unsigned mult        = 1;

        if (*(xml + 2) == 'x')
        {
            // Hexadecimal.
            if ((*(xml + 3) == '\0'))
                return nullptr;

            const char * valueStr = xml + 3;
            valueStr              = strchr(valueStr, ';');

            if ((valueStr == nullptr) || (*valueStr == '\0'))
                return nullptr;

            delta = valueStr - xml;
            --valueStr;

            while (*valueStr != 'x')
            {
                if (*valueStr >= '0' && *valueStr <= '9')
                    ucs += mult * (*valueStr - '0');
                else if (*valueStr >= 'a' && *valueStr <= 'f')
                    ucs += mult * (*valueStr - 'a' + 10);
                else if (*valueStr >= 'A' && *valueStr <= 'F')
                    ucs += mult * (*valueStr - 'A' + 10);
                else
                    return nullptr;
                mult *= 16;
                --valueStr;
            }
        }
        else
        {
            // Decimal.
            const char * valueStr = xml + 2;
            valueStr              = strchr(valueStr, ';');

            if ((valueStr == nullptr) || (*valueStr == '\0'))
                return nullptr;

            delta = valueStr - xml;
            --valueStr;

            while (*valueStr != '#')
            {
                if (*valueStr >= '0' && *valueStr <= '9')
                    ucs += mult * (*valueStr - '0');
                else
                    return nullptr;
                mult *= 10;
                --valueStr;
            }
        }
        if (encoding == XMLEncoding::UTF8)
        {
            // convert the UCS to UTF-8
            ConvertUTF32ToUTF8(ucs, value, length);
        }
        else
        {
            *value = (char)ucs;
            length = 1;
        }
        return xml + delta + 1;
    }

    // Now try to match it.
    for (int i = 0; i < NumEntities; ++i)
    {
        if (strncmp(entities[i].str, xml, entities[i].length) == 0)
        {
            assert(strlen(entities[i].str) == entities[i].length);
            *value = entities[i].chr;
            length = 1;
            return (xml + entities[i].length);
        }
    }

    // So it wasn't an entity, its unrecognized, or something like that.
    *value = *xml; // Don't put back the last one, since we return it!
    return xml + 1;
}

XMLBase::XMLBase() : userData(nullptr), location(), owningDocument(nullptr)
{
}

XMLBase::~XMLBase()
{
}

bool CheckSpecialCharacters(char c, std::string & str)
{
    for (auto entity : entities)
    {
        if (c == entity.chr)
        {
            str.append(entity.str, entity.length);
            return true;
        }
    }
    return false;
}

void XMLBase::EncodeString(const std::string & in, std::string & out)
{
    size_t i = 0;

    while (i < in.length())
    {
        char c = in[i];

        if ((i < (in.length() - 2)) && (in.substr(i, 3) == "&#x"))
        {
            if ((i < in.length() - 5) && (isxdigit(in[i + 3])) && (isxdigit(in[i + 4]))
                && (in[i + 5] == ';'))
            {
                out += in.substr(i, 6);
                i += 6;
            }
            else
            {
                return;
            }
        }
        else if (c < 32)
        {
            // Easy pass at non-alpha/numeric/symbol
            // Below 32 is symbolic.
            char buf[8];

            sprintf(buf, "&#x%02X;", int(c));

            out.append(buf, strlen(buf));
            ++i;
        }
        else
        {
            if (CheckSpecialCharacters(c, out))
            {
                ++i;
                continue;
            }

            out += c; // somewhat more efficient function call.
            ++i;
        }
    }
}

void XMLBase::SetError(XMLError errorCode, const char * xml)
{
    assert((errorCode > XMLError::NoError) && (errorCode < XMLError::ErrorStringCount));

    throw XMLInternalException(xml, errorCode);
}

void XMLBase::Clear()
{
    userData = nullptr;
    location.Clear();
}

void XMLBase::CopyTo(XMLBase * target) const
{
    target->userData = userData;
    target->location = location;
}

bool XMLBase::IsAlpha(char anyChar, XMLEncoding /*encoding*/)
{
    // This will only work for low-ascii, everything else is assumed to be a valid
    // letter. I'm not sure this is the best approach, but it is quite tricky trying
    // to figure out alhabetical vs. not across encoding. So take a very
    // conservative approach.

    if (anyChar < 127)
        return isalpha(anyChar);
    return true;
}

bool XMLBase::IsAlphaNum(char anyChar, XMLEncoding /*encoding*/)
{
    // This will only work for low-ascii, everything else is assumed to be a valid
    // letter. I'm not sure this is the best approach, but it is quite tricky trying
    // to figure out alhabetical vs. not across encoding. So take a very
    // conservative approach.

    if (anyChar < 127)
        return isalnum(anyChar);
    return true; // What else to do? The unicode set is huge...get the english ones right.
}

const char * XMLBase::SkipWhiteSpace(const char * xml, XMLEncoding encoding)
{
    assert(xml);
    if (*xml == '\0')
    {
        return xml;
    }
    if (encoding == XMLEncoding::UTF8)
    {
        while (*xml != '\0')
        {
            // Skip the UTF-8 Byte order marks
            if ((*(xml + 0) == UTF_BOM0) && (*(xml + 1) == UTF_BOM1) && (*(xml + 2) == UTF_BOM2))
            {
                xml += 3;
                continue;
            }

            if (IsWhiteSpace(*xml)) // Still using old rules for white space.
                ++xml;
            else
                break;
        }
    }
    else
    {
        while ((*xml != '\0') && IsWhiteSpace(*xml))
            ++xml;
    }

    return xml;
}

const char * XMLBase::ReadName(const char * xml, std::string & name, XMLEncoding encoding)
{
    name = "";

    if (*xml == '\0')
        return nullptr;

    // Names start with letters or underscores.
    // After that, they can be letters, underscores, numbers,
    // hyphens, or colons. (Colons are valid ony for namespaces,
    // but we can't tell namespaces from names.)
    if (IsAlpha(*xml, encoding) || *xml == '_')
    {
        const char * start = xml;
        while ((*xml != '\0') && (IsAlphaNum(*xml, encoding) || *xml == '_' || *xml == '-' || *xml == '.'
                                  || *xml == ':'))
        {
            ++xml;
        }
        if (xml > start)
        {
            name.assign(start, xml - start);
        }
        return xml;
    }
    return nullptr;
}

const char * XMLBase::ReadText(const char * xml,
                               std::string & text,
                               bool trimWhiteSpace,
                               const char * endTag,
                               XMLIgnoreCase caseInsensitive,
                               XMLEncoding encoding)
{
    text = "";
    if (!trimWhiteSpace ||   // certain tags always keep isWhiteSpace
        !condenseWhiteSpace) // if true, isWhiteSpace is always kept
    {
        // Keep all the white space.
        while ((*xml != '\0') && !IsEndTag(xml, endTag, caseInsensitive, encoding))
        {
            int len;
            char characterBytes[4] = {0, 0, 0, 0};
            xml                    = GetChar(xml, characterBytes, len, encoding);
            text.append(characterBytes, len);
        }
    }
    else
    {
        bool isWhiteSpace = false;

        // Remove leading white space:
        xml = SkipWhiteSpace(xml, encoding);
        while ((*xml != '\0') && !IsEndTag(xml, endTag, caseInsensitive, encoding))
        {
            if ((*xml == '\r') || (*xml == '\n'))
            {
                isWhiteSpace = true;
                ++xml;
            }
            else if (IsWhiteSpace(*xml))
            {
                isWhiteSpace = true;
                ++xml;
            }
            else
            {
                // If we've found isWhiteSpace, add it before the
                // new character. Any isWhiteSpace just becomes a space.
                if (isWhiteSpace)
                {
                    text += ' ';
                    isWhiteSpace = false;
                }
                int len;
                char characterBytes[4] = {0, 0, 0, 0};
                xml                    = GetChar(xml, characterBytes, len, encoding);
                if (len == 1)
                    text += characterBytes[0];
                else
                    text.append(characterBytes, len);
            }
        }
    }
    if ((*xml != '\0'))
        xml += strlen(endTag);
    return xml;
}

bool XMLBase::IsEndTag(const char * xml,
                       const char * endTag,
                       XMLIgnoreCase ignoreCase,
                       XMLEncoding encoding)
{
    assert(endTag);
    if (*xml == '\0')
    {
        assert(false);
        return false;
    }

    if (ignoreCase == XMLIgnoreCase::IgnoreCase)
    {
        while ((*xml != '\0') && *endTag && (ToLower(*xml, encoding) == ToLower(*endTag, encoding)))
        {
            ++xml;
            ++endTag;
        }
    }
    else
    {
        while ((*xml != '\0') && *endTag && *xml == *endTag)
        {
            ++xml;
            ++endTag;
        }
    }
    if (*endTag == '\0')
        return true;
    return false;
}

// Get a character, while interpreting entities.
// The length can be from 0 to 4 bytes.
const char * XMLBase::GetChar(const char * xml, char * value, int & length, XMLEncoding encoding)
{
    if (encoding == XMLEncoding::UTF8)
    {
        length = UTF8ByteTable[(unsigned char)*xml];
        assert((length >= 0) && (length < 5));
    }
    else
    {
        length = 1;
    }

    if (length == 1)
    {
        if (*xml == '&')
            return GetEntity(xml, value, length, encoding);
        *value = *xml;
        return xml + 1;
    }
    else if (length)
    {
        for (int i = 0; (i < length) && (*xml != '\0'); ++i)
        {
            value[i] = *xml++;
        }
        return xml;
    }
    else
    {
        // Not valid text.
        return nullptr;
    }
}

bool XMLBase::StringEqual(const char * xml,
                          const char * tag,
                          XMLIgnoreCase ignoreCase,
                          XMLEncoding encoding)
{
    assert(xml);
    assert(tag);
    if ((xml == nullptr) || (*xml == '\0'))
    {
        return false;
    }

    if (ignoreCase == XMLIgnoreCase::IgnoreCase)
    {
        while ((*xml != '\0') && (*tag != '\0')
               && (ToLower(*xml, encoding) == ToLower(*tag, encoding)))
        {
            ++xml;
            ++tag;
        }
    }
    else
    {
        while ((*xml != '\0') && (*tag != '\0') && (*xml == *tag))
        {
            ++xml;
            ++tag;
        }
    }
    if (*tag == '\0')
        return true;
    return false;
}
