#include "xml/XMLDocument.h"

#include "core/Util.h"
#include "osal/Strings.h"
#include "xml/XMLDeclaration.h"
#include "xml/XMLParsingData.h"
#include <cassert>
#include <fstream>

using namespace std;
using namespace XMLParser;

const XMLEncoding XMLParser::XMLDefaultEncoding = XMLEncoding::Unknown;

XMLDocument::XMLDocument() : useMicrosoftBOM(false)
{
}

XMLDocument::XMLDocument(const XMLDocument & other) : XMLDocument()
{
    other.CopyTo(this);
}

XMLDocument::~XMLDocument()
{
}

XMLDocument & XMLDocument::operator=(const XMLDocument & other)
{
    if (&other != this)
    {
        Clear();
        other.CopyTo(this);
    }
    return *this;
}

void XMLDocument::LoadFile(const std::string path, XMLEncoding encoding /* = XMLDefaultEncoding*/)
{
    ifstream stream(path, ios_base::in | ios_base::binary);
    return LoadFile(stream, encoding);
}

void XMLDocument::SaveFile(const std::string path) const
{
    ofstream stream(path, ios_base::out | ios_base::trunc);
    return SaveFile(stream);
}

void XMLDocument::LoadFile(std::istream & stream, XMLEncoding encoding /*= XMLDefaultEncoding*/)
{
    const char * xml = nullptr;
    try
    {
        Core::FixedArray<char> buffer = std::move(LoadFileContents(stream));
        xml                           = buffer.Data();
        Parse(xml, encoding);
    }
    catch (const XMLInternalException & e)
    {
        XMLParsingData parseData(xml);
        parseData.Update(e.ErrorPosition(), encoding);
        XMLErrorInfo errorInfo(e.ErrorCode(), parseData.Cursor());
        //        cout << "Error: " << errorInfo.ErrorDescription()
        //             << " Error Code: " << int(errorInfo.ErrorCode())
        //             << " Location: (" << errorInfo.ErrorRow() << "," << errorInfo.ErrorCol() <<
        //             ")" << std::endl
        //             << ((e.ErrorPosition() != nullptr) ? e.ErrorPosition() : "(null)") <<
        //             std::endl;
        throw XMLParseException(e.ErrorPosition(), errorInfo);
    }
}

Core::FixedArray<char> XMLDocument::LoadFileContents(std::istream & stream)
{
    if (!stream)
        SetError(XMLError::ErrorOpeningFile, nullptr);

    Clear();

    streampos length = 0;
    stream.seekg(0, ios_base::seekdir::_S_end);
    length = stream.tellg();
    stream.seekg(0, ios_base::seekdir::_S_beg);
    if (!stream || (length <= 0))
        SetError(XMLError::ErrorDocumentEmpty, nullptr);

    Core::FixedArray<char> buf(size_t(length) + 1);
    stream.read(buf, length);
    if (!stream)
        SetError(XMLError::ErrorOpeningFile, nullptr);

    NormalizeLines(buf);

    return buf;
}

void XMLDocument::SaveFile(std::ostream & stream) const
{
    if (useMicrosoftBOM)
    {
        stream.write(const_cast<char *>(reinterpret_cast<const char *>(&UTF_BOM0)), 1);
        stream.write(const_cast<char *>(reinterpret_cast<const char *>(&UTF_BOM1)), 1);
        stream.write(const_cast<char *>(reinterpret_cast<const char *>(&UTF_BOM2)), 1);
    }
    PrintTo(stream, 0);
}

void XMLDocument::ParseString(const std::string & xml, XMLEncoding encoding /*= XMLDefaultEncoding*/)
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

std::string XMLDocument::GenerateString(int depth /*= 0*/) const
{
    std::ostringstream stream;

    for (auto & element : Children())
    {
        stream << element->GenerateString(depth) << '\n';
    }
    return stream.str();
}

const char * XMLDocument::Parse(const char * xml, XMLEncoding encoding /*= XMLDefaultEncoding*/)
{
    // Parse away, at the document level. Since a document
    // contains nothing but other tags, most of what happens
    // here is skipping white space.
    if ((xml == nullptr) || (*xml == '\0'))
        SetError(XMLError::ErrorParsingEmpty, nullptr);

    // Note that, for a document, this needs to come
    // before the while space skip, so that parsing
    // starts from the pointer we are given.
    if (encoding == XMLEncoding::Unknown)
    {
        // Check for the Microsoft UTF-8 lead bytes.
        if ((*(xml + 0) != '\0') && (*(xml + 0) == UTF_BOM0) && (*(xml + 1) != '\0')
            && (*(xml + 1) == UTF_BOM1)
            && (*(xml + 2) != '\0')
            && (*(xml + 2) == UTF_BOM2))
        {
            encoding        = XMLEncoding::UTF8;
            useMicrosoftBOM = true;
        }
    }

    xml = SkipWhiteSpace(xml, encoding);
    if ((xml == nullptr) || (*xml == '\0'))
        SetError(XMLError::ErrorParsingEmpty, nullptr);

    while ((xml != nullptr) && (*xml != '\0'))
    {
        XMLNode::Ptr element = Identify(xml, encoding);
        if (element != nullptr)
        {
            xml = element->Parse(xml, encoding);
            AddChild(element);
        }
        else
        {
            break;
        }

        // Did we get encoding info?
        if ((encoding == XMLEncoding::Unknown)
            && (dynamic_pointer_cast<XMLDeclaration>(element) != nullptr))
        {
            XMLDeclaration::Ptr declaration = dynamic_pointer_cast<XMLDeclaration>(element);
            std::string encodingType        = declaration->Encoding();

            if (encodingType.empty())
                encoding = XMLEncoding::UTF8;
            else if (OSAL::IsEqualIgnoreCase(encodingType, "UTF-8"))
                encoding = XMLEncoding::UTF8;
            else if (OSAL::IsEqualIgnoreCase(encodingType, "UTF8"))
                encoding = XMLEncoding::UTF8; // incorrect, but be nice
            else
                encoding = XMLEncoding::Legacy;
        }

        xml = SkipWhiteSpace(xml, encoding);
    }
    // Was this empty?
    if (!HasChildren())
        SetError(XMLError::ErrorDocumentEmpty, nullptr);

    // All is well.
    return xml;
}

void XMLDocument::PrintTo(std::ostream & stream, int depth /*= 0*/) const
{
    stream << GenerateString(depth);
}

void XMLDocument::CopyTo(XMLDocument * target) const
{
    XMLNode::CopyTo(target);

    target->location        = location;
    target->useMicrosoftBOM = useMicrosoftBOM;

    for (auto & element : Children())
    {
        target->AddChild(element->Clone());
    }
}

XMLNode::Ptr XMLDocument::Clone() const
{
    XMLDocument * clone = new XMLDocument();
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

void XMLDocument::NormalizeLines(Core::FixedArray<char> & buffer)
{
    // Process the buffer in place to normalize new lines. (See comment above.)
    //
    // Wikipedia:
    // Systems based on ASCII or a compatible character set use either LF  (Line feed, '\n', 0x0A,
    // 10 in decimal) or
    // CR (Carriage return, '\r', 0x0D, 13 in decimal) individually, or CR followed by LF (CR+LF,
    // 0x0D 0x0A)...
    //		* LF:    Multics, Unix and Unix-like systems (GNU/Linux, AIX, Xenix, Mac OS X, FreeBSD,
    //etc.), BeOS, Amiga, RISC OS, and others
    //		* CR+LF: DEC RT-11 and most other early non-Unix, non-IBM OSes, CP/M, MP/M, DOS, OS/2,
    //Microsoft Windows, Symbian OS
    //		* CR:    Commodore 8-bit machines, Apple II family, Mac OS up to version 9 and OS-9

    char * buf           = buffer.Data();
    const char * readPos = buf;
    char * writePos      = buf;
    const char CR        = 0x0d;
    const char LF        = 0x0a;
    size_t length        = buffer.Size();

    buffer[length - 1] = '\0';
    while (*readPos)
    {
        assert(readPos < (buf + length));
        assert(writePos <= (buf + length));
        assert(writePos <= readPos);

        if (*readPos == CR)
        {
            *writePos++ = LF;
            readPos++;
            if (*readPos == LF)
            {
                readPos++;
            }
        }
        else
        {
            *writePos++ = *readPos++;
        }
    }
    assert(writePos <= (buf + length));
    *writePos = '\0';
}

bool XMLDocument::Accept(XMLVisitor * visitor) const
{
    if (visitor->VisitEnter(*this))
    {
        for (auto & child : Children())
        {
            if (!child->Accept(visitor))
                break;
        }
    }
    return visitor->VisitExit(*this);
}
