#include <unittest-c++/UnitTestC++.h>

#include <string>
#include <core/DefaultLogger.h>
#include "xml/TestData.h"
#include "xml/XMLDeclaration.h"
#include "xml/XMLDocument.h"

using namespace std;

namespace XMLParser {
namespace Test {

class XMLDocumentTest: public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void XMLDocumentTest::SetUp()
{
}

void XMLDocumentTest::TearDown()
{
}

class FileWrapper
{
public:
    FileWrapper() :
        _handle(nullptr)
    {
    }
    FileWrapper(const FileWrapper &) = delete;
    ~FileWrapper()
    {
        Close();
    }

    FileWrapper & operator = (const FileWrapper &) = delete;

    operator FILE * ()
    {
        return _handle;
    }
    bool Open(const char * path, const char * mode)
    {
        Close();
        _handle = fopen(path, mode);
        return (_handle != nullptr);
    }
    void Close()
    {
        if (_handle != nullptr)
            fclose(_handle);
        _handle = nullptr;
    }
    size_t Size()
    {
        fseek(_handle, 0, SEEK_END);
        size_t result = static_cast<size_t>(ftell(_handle));
        fseek(_handle, 0, SEEK_SET);
        return result;
    }
    size_t Read(uint8_t * buffer, size_t numBytes)
    {
        return fread(buffer, sizeof(uint8_t), numBytes, _handle);
    }
    size_t Write(const uint8_t * buffer, size_t numBytes)
    {
        return fwrite(buffer, sizeof(uint8_t), numBytes, _handle);
    }
    bool Eof() { return feof(_handle) != 0; }

protected:
    FILE * _handle;
};

class Buffer
{
public:
    Buffer() = delete;
    Buffer(const Buffer &) = delete;
    explicit Buffer(size_t size) :
        _size(size),
        _contents(nullptr)
    {
        _contents = new char [_size];
    }
    ~Buffer()
    {
        delete [] _contents;
        _size = 0;
        _contents = nullptr;
    }

    Buffer & operator = (const Buffer &) = delete;

    size_t _size;
    char * _contents;
};

ssize_t ReadLine(Buffer & buffer, FILE * file)
{
    ssize_t count = getline(&buffer._contents, &buffer._size, file);
    return count;
}

bool CompareLines(Buffer & bufferA, FileWrapper & fileA, Buffer & bufferB, FileWrapper & fileB)
{
    int line = 1;
    while (!feof(fileA) && !feof(fileB))
    {
        ssize_t countA = ReadLine(bufferA, fileA);
        ssize_t countB = ReadLine(bufferB, fileB);
        if ((countA == -1) && (countB == -1))
            return true;
        if ((countA == -1) || (countB == -1))
        {
            ostringstream stream;
            stream << "Difference between text files in line " << line;
            Core::TheLogger().Warn("Util::CompareTextFiles", stream.str());
            return false;
        }
        if (countA != countB)
        {
            ostringstream stream;
            stream << "Difference between text files in line " << line;
            Core::TheLogger().Warn("Util::CompareTextFiles", stream.str());
            return false;
        }
        if (strcmp(bufferA._contents, bufferB._contents) != 0)
        {
            ostringstream stream;
            stream << "Difference between text files in line " << line;
            Core::TheLogger().Warn("Util::CompareTextFiles", stream.str());
            return false;
        }
        line++;
    }
    return (feof(fileA) && feof(fileB));
}

bool CompareTextFiles(const char * pathA, const char * pathB)
{
    bool result = false;
    FileWrapper fileA;
    FileWrapper fileB;

    if (fileA.Open(pathA, "r"))
    {
        if (fileB.Open(pathB, "r"))
        {
            const size_t BufferSize = 1024;
            Buffer bufferA(BufferSize);
            Buffer bufferB(BufferSize);
            result = CompareLines(bufferA, fileA, bufferB, fileB);
        }
    }
    return result;
}

bool CompareTextFiles(const string & pathA, const string & pathB)
{
    return CompareTextFiles(pathA.c_str(), pathB.c_str());
}

TEST_SUITE(xml)
{

TEST_FIXTURE(XMLDocumentTest, Construct)
{
    XMLDocument document;
    EXPECT_TRUE(document.Root() == nullptr);
}

TEST_FIXTURE(XMLDocumentTest, ParseANonExistingFile)
{
    XMLDocument document;
    bool hadException = false;
    try
    {
        document.LoadFile("non_existing_file.xml");
    }
    catch (const XMLParseException & e)
    {
        hadException = true;
        EXPECT_TRUE(document.Root() == nullptr);
        EXPECT_EQ(XMLError::ErrorOpeningFile, e.ErrorInfo().ErrorCode());
        EXPECT_EQ(0, e.ErrorInfo().ErrorRow());
        EXPECT_EQ(0, e.ErrorInfo().ErrorCol());
    }
    EXPECT_TRUE(hadException);
}

TEST_FIXTURE(XMLDocumentTest, ParseEmpty)
{
    XMLDocument document;
    bool hadException = false;
    try
    {
        document.LoadFile(XMLParserTestData::EmptyXML());
    }
    catch (const XMLParseException & e)
    {
        hadException = true;
        EXPECT_TRUE(document.Root() == nullptr);
        EXPECT_EQ(XMLError::ErrorDocumentEmpty, e.ErrorInfo().ErrorCode());
        EXPECT_EQ(0, e.ErrorInfo().ErrorRow());
        EXPECT_EQ(0, e.ErrorInfo().ErrorCol());
    }
    EXPECT_TRUE(hadException);
}

TEST_FIXTURE(XMLDocumentTest, ParseDeclarationOnly)
{
    XMLDocument document;
    document.LoadFile(XMLParserTestData::DeclarationOnlyXML());
    EXPECT_TRUE(document.Root() == nullptr);
}

TEST_FIXTURE(XMLDocumentTest, ParseRootOnly)
{
    XMLDocument document;
    document.LoadFile(XMLParserTestData::RootOnlyXML());

    ASSERT_FALSE(document.Root() == nullptr);
    EXPECT_TRUE(document.Root()->Match("root"));
    EXPECT_EQ(size_t(0), document.Root()->Attributes().size());
    EXPECT_EQ(size_t(0), document.Root()->Children().size());
    EXPECT_EQ(size_t(0), document.Root()->Elements().size());
}

TEST_FIXTURE(XMLDocumentTest, ParseVerySimpleFile)
{
    XMLDocument document;
    document.LoadFile(XMLParserTestData::VerySimpleXML());

    ASSERT_FALSE(document.Root() == nullptr);
    EXPECT_TRUE(document.Root()->Match("root"));
    EXPECT_EQ(size_t(2), document.Root()->Children().size());
    EXPECT_EQ(size_t(2), document.Root()->Elements().size());

    EXPECT_TRUE(document.Root()->Elements()[0]->Match("element"));
    EXPECT_EQ(size_t(1), document.Root()->Elements()[0]->Attributes().size());

    EXPECT_TRUE(document.Root()->Elements()[0]->Attributes()[0]->Match("name"));
    EXPECT_EQ("element1", document.Root()->Elements()[0]->Attributes()[0]->Value());
    EXPECT_EQ(size_t(0), document.Root()->Elements()[0]->Elements().size());

    EXPECT_TRUE(document.Root()->Elements()[1]->Match("element"));
    EXPECT_EQ(size_t(1), document.Root()->Elements()[1]->Attributes().size());

    EXPECT_TRUE(document.Root()->Elements()[1]->Attributes()[0]->Match("name"));
    EXPECT_EQ("element2", document.Root()->Elements()[1]->Attributes()[0]->Value());
    EXPECT_EQ(size_t(0), document.Root()->Elements()[1]->Elements().size());
}

TEST_FIXTURE(XMLDocumentTest, ParseSimpleFile)
{
    XMLDocument document;
    document.LoadFile(XMLParserTestData::SimpleXML());

    ASSERT_FALSE(document.Root() == nullptr);
    EXPECT_TRUE(document.Root()->Match("root"));
    EXPECT_EQ(size_t(2), document.Root()->Children().size());
    EXPECT_EQ(size_t(2), document.Root()->Elements().size());

    EXPECT_TRUE(document.Root()->Elements()[0]->Match("element"));
    EXPECT_EQ(size_t(1), document.Root()->Elements()[0]->Attributes().size());

    EXPECT_TRUE(document.Root()->Elements()[0]->Attributes()[0]->Match("name"));
    EXPECT_EQ("element1", document.Root()->Elements()[0]->Attributes()[0]->Value());

    EXPECT_EQ(size_t(2), document.Root()->Elements()[0]->Elements().size());

    EXPECT_TRUE(document.Root()->Elements()[0]->Elements()[0]->Match("subelement"));
    EXPECT_EQ(size_t(1), document.Root()->Elements()[0]->Elements()[0]->Attributes().size());

    EXPECT_TRUE(document.Root()->Elements()[0]->Elements()[0]->Attributes()[0]->Match("name"));
    EXPECT_EQ("sub1", document.Root()->Elements()[0]->Elements()[0]->Attributes()[0]->Value());

    EXPECT_TRUE(document.Root()->Elements()[0]->Elements()[1]->Match("subelement"));
    EXPECT_EQ(size_t(1), document.Root()->Elements()[0]->Elements()[1]->Attributes().size());

    EXPECT_TRUE(document.Root()->Elements()[0]->Elements()[1]->Attributes()[0]->Match("name"));
    EXPECT_EQ("sub2", document.Root()->Elements()[0]->Elements()[1]->Attributes()[0]->Value());

    EXPECT_TRUE(document.Root()->Elements()[1]->Match("element"));
    EXPECT_EQ(size_t(1), document.Root()->Elements()[1]->Attributes().size());

    EXPECT_TRUE(document.Root()->Elements()[1]->Attributes()[0]->Match("name"));
    EXPECT_EQ("element2", document.Root()->Elements()[1]->Attributes()[0]->Value());

    EXPECT_EQ(size_t(3), document.Root()->Elements()[1]->Elements().size());

    EXPECT_TRUE(document.Root()->Elements()[1]->Elements()[0]->Match("subelement"));
    EXPECT_EQ(size_t(1), document.Root()->Elements()[1]->Elements()[0]->Attributes().size());

    EXPECT_TRUE(document.Root()->Elements()[1]->Elements()[0]->Attributes()[0]->Match("name"));
    EXPECT_EQ("sub1", document.Root()->Elements()[1]->Elements()[0]->Attributes()[0]->Value());

    EXPECT_TRUE(document.Root()->Elements()[1]->Elements()[1]->Match("subelement"));
    EXPECT_EQ(size_t(1), document.Root()->Elements()[1]->Elements()[1]->Attributes().size());

    EXPECT_TRUE(document.Root()->Elements()[1]->Elements()[1]->Attributes()[0]->Match("name"));
    EXPECT_EQ("sub2", document.Root()->Elements()[1]->Elements()[1]->Attributes()[0]->Value());

    EXPECT_TRUE(document.Root()->Elements()[1]->Elements()[2]->Match("subelement"));
    EXPECT_EQ(size_t(1), document.Root()->Elements()[1]->Elements()[2]->Attributes().size());

    EXPECT_TRUE(document.Root()->Elements()[1]->Elements()[2]->Attributes()[0]->Match("name"));
    EXPECT_EQ("sub3", document.Root()->Elements()[1]->Elements()[2]->Attributes()[0]->Value());

    EXPECT_TRUE(document.Root()->Elements()[2] == nullptr);
}

TEST_FIXTURE(XMLDocumentTest, SaveEmpty)
{
    XMLDocument document;

    OSAL::OS::TemporaryFile tempFile;
    string path = tempFile.GetPath();
    tempFile.Close();
    document.SaveFile(path);
    EXPECT_TRUE(CompareTextFiles(XMLParserTestData::EmptyXMLRef(), path));
}

TEST_FIXTURE(XMLDocumentTest, SaveDeclarationOnly)
{
    XMLDocument document;

    XMLDeclaration::Ptr declaration = make_shared<XMLDeclaration>("1.0", "", "no");
    document.AddChild(declaration);

    OSAL::OS::TemporaryFile tempFile;
    string path = tempFile.GetPath();
    tempFile.Close();
    document.SaveFile(path);
    EXPECT_TRUE(CompareTextFiles(XMLParserTestData::DeclarationOnlyXMLRef(), path));
}

TEST_FIXTURE(XMLDocumentTest, SaveRootOnly)
{
    XMLDocument document;

    XMLDeclaration::Ptr declaration = make_shared<XMLDeclaration>("1.0", "", "no");
    document.AddChild(declaration);
    XMLElement::Ptr element = make_shared<XMLElement>("root");
    document.AddChild(element);

    OSAL::OS::TemporaryFile tempFile;
    string path = tempFile.GetPath();
    tempFile.Close();
    document.SaveFile(path);
    EXPECT_TRUE(CompareTextFiles(XMLParserTestData::RootOnlyXMLRef(), path));
}

TEST_FIXTURE(XMLDocumentTest, SaveVerySimpleFile)
{
    XMLDocument document;

    XMLDeclaration::Ptr declaration = make_shared<XMLDeclaration>("1.0", "", "no");
    document.AddChild(declaration);
    XMLElement::Ptr root = make_shared<XMLElement>("root");
    document.AddChild(root);
    XMLElement::Ptr element1 = make_shared<XMLElement>("element");
    element1->AddAttribute("name", "element1");
    root->AddChild(element1);
    XMLElement::Ptr element2 = make_shared<XMLElement>("element");
    element2->AddAttribute("name", "element2");
    root->AddChild(element2);

    OSAL::OS::TemporaryFile tempFile;
    string path = tempFile.GetPath();
    tempFile.Close();
    document.SaveFile(path);
    EXPECT_TRUE(CompareTextFiles(XMLParserTestData::VerySimpleXMLRef(), path));
}

TEST_FIXTURE(XMLDocumentTest, SaveSimpleFile)
{
    XMLDocument document;

    XMLDeclaration::Ptr declaration = make_shared<XMLDeclaration>("1.0", "", "no");
    document.AddChild(declaration);
    XMLElement::Ptr root = make_shared<XMLElement>("root");
    document.AddChild(root);
    XMLElement::Ptr element1 = make_shared<XMLElement>("element");
    element1->AddAttribute("name", "element1");
    XMLElement::Ptr subElement11 = make_shared<XMLElement>("subelement");
    subElement11->AddAttribute("name", "sub1");
    element1->AddChild(subElement11);
    XMLElement::Ptr subElement12 = make_shared<XMLElement>("subelement");
    subElement12->AddAttribute("name", "sub2");
    element1->AddChild(subElement12);
    root->AddChild(element1);
    XMLElement::Ptr element2 = make_shared<XMLElement>("element");
    element2->AddAttribute("name", "element2");
    XMLElement::Ptr subElement21 = make_shared<XMLElement>("subelement");
    subElement21->AddAttribute("name", "sub1");
    element2->AddChild(subElement21);
    XMLElement::Ptr subElement22 = make_shared<XMLElement>("subelement");
    subElement22->AddAttribute("name", "sub2");
    element2->AddChild(subElement22);
    XMLElement::Ptr subElement23 = make_shared<XMLElement>("subelement");
    subElement23->AddAttribute("name", "sub3");
    element2->AddChild(subElement23);
    root->AddChild(element2);

    OSAL::OS::TemporaryFile tempFile;
    string path = tempFile.GetPath();
    tempFile.Close();
    document.SaveFile(path);
    EXPECT_TRUE(CompareTextFiles(XMLParserTestData::SimpleXMLRef(), path));
}

TEST_FIXTURE(XMLDocumentTest, ParseEmptyString)
{
    XMLDocument document;
    std::string xml = "";
    bool hadException = false;
    try
    {
        document.ParseString(xml);
    }
    catch (const XMLParseException & e)
    {
        hadException = true;
        EXPECT_TRUE(document.Root() == nullptr);
        EXPECT_EQ(XMLError::ErrorParsingEmpty, e.ErrorInfo().ErrorCode());
        EXPECT_EQ(0, e.ErrorInfo().ErrorRow());
        EXPECT_EQ(0, e.ErrorInfo().ErrorCol());
    }
    EXPECT_TRUE(hadException);
}

TEST_FIXTURE(XMLDocumentTest, ParseBOMUTF8OnlyString)
{
    XMLDocument document;
    std::string xml = "\xef\xbb\xbf";
    bool hadException = false;
    try
    {
        document.ParseString(xml);
    }
    catch (const XMLParseException & e)
    {
        hadException = true;
        EXPECT_TRUE(document.Root() == nullptr);
        EXPECT_EQ(XMLError::ErrorParsingEmpty, e.ErrorInfo().ErrorCode());
        EXPECT_EQ(0, e.ErrorInfo().ErrorRow());
        EXPECT_EQ(0, e.ErrorInfo().ErrorCol());
    }
    EXPECT_TRUE(hadException);
}

TEST_FIXTURE(XMLDocumentTest, ParseDeclarationOnlyString)
{
    XMLDocument document;
    std::string xml = "<?xml version=\"1.0\" standalone=\"no\"?>";
    document.ParseString(xml);

    EXPECT_TRUE(document.Root() == nullptr);
}

TEST_FIXTURE(XMLDocumentTest, ParseBOMUTF8DeclarationString)
{
    XMLDocument document;
    std::string xml = "\xef\xbb\xbf<?xml version=\"1.0\" standalone=\"no\"?>";
    document.ParseString(xml);

    EXPECT_TRUE(document.Root() == nullptr);
}

TEST_FIXTURE(XMLDocumentTest, ParseRootOnlyString)
{
    XMLDocument document;
    std::string xml = "<root />";
    document.ParseString(xml);

    EXPECT_FALSE(document.Root() == nullptr);
    EXPECT_TRUE(document.Root()->Match("root"));
    EXPECT_TRUE(document.Root()->Match("root", XMLIgnoreCase::IgnoreCase));
    EXPECT_FALSE(document.Root()->Match("Root"));
    EXPECT_TRUE(document.Root()->Match("Root", XMLIgnoreCase::IgnoreCase));
    EXPECT_EQ(size_t(0), document.Root()->Attributes().size());
    EXPECT_EQ(size_t(0), document.Root()->Children().size());
    EXPECT_EQ(size_t(0), document.Root()->Elements().size());
}

TEST_FIXTURE(XMLDocumentTest, ParseASimpleStringWithAttribute)
{
    XMLDocument document;
    std::string xml = "<tag id=\"name\"/>";
    document.ParseString(xml);

    EXPECT_FALSE(document.Root() == nullptr);

    EXPECT_TRUE(document.Root()->Match("tag", XMLIgnoreCase::IgnoreCase));
    EXPECT_TRUE(document.Root()->Match("tag"));
    EXPECT_TRUE(document.Root()->Match("Tag", XMLIgnoreCase::IgnoreCase));
    EXPECT_FALSE(document.Root()->Match("Tag"));
    EXPECT_EQ(size_t(0), document.Root()->Children().size());
    EXPECT_EQ(size_t(0), document.Root()->Elements().size());
    EXPECT_EQ(size_t(1), document.Root()->Attributes().size());

    EXPECT_TRUE(document.Root()->Attributes()[0]->Match("id"));
    EXPECT_TRUE(document.Root()->Attributes()[0]->Match("id", XMLIgnoreCase::IgnoreCase));
    EXPECT_FALSE(document.Root()->Attributes()[0]->Match("ID"));
    EXPECT_TRUE(document.Root()->Attributes()[0]->Match("ID", XMLIgnoreCase::IgnoreCase));
    EXPECT_EQ("name", document.Root()->Attributes()[0]->Value());
}

TEST_FIXTURE(XMLDocumentTest, ParseSimpleStringWithValidIntAttribute)
{
    XMLDocument document;
    std::string xml = "<tag id=\"1\"/>";
    document.ParseString(xml);

    EXPECT_FALSE(document.Root() == nullptr);

    EXPECT_TRUE(document.Root()->Match("tag"));
    EXPECT_EQ(size_t(0), document.Root()->Children().size());
    EXPECT_EQ(size_t(0), document.Root()->Elements().size());
    EXPECT_EQ(size_t(1), document.Root()->Attributes().size());

    EXPECT_TRUE(document.Root()->Attributes()[0]->Match("id"));
    EXPECT_EQ(1, document.Root()->Attributes()[0]->Value<int>());
    EXPECT_EQ(1.00, document.Root()->Attributes()[0]->Value<double>());
}

TEST_FIXTURE(XMLDocumentTest, ParseSimpleStringWithInvalidIntAttribute)
{
    XMLDocument document;
    std::string xml = "<tag id=\"abc\"/>";
    document.ParseString(xml);

    EXPECT_FALSE(document.Root() == nullptr);

    EXPECT_TRUE(document.Root()->Match("tag"));
    EXPECT_EQ(size_t(0), document.Root()->Children().size());
    EXPECT_EQ(size_t(0), document.Root()->Elements().size());
    EXPECT_EQ(size_t(1), document.Root()->Attributes().size());

    EXPECT_TRUE(document.Root()->Attributes()[0]->Match("id"));
    EXPECT_THROW(document.Root()->Attributes()[0]->Value<int>(), XMLValueParseException);
}

TEST_FIXTURE(XMLDocumentTest, ParseSimpleStringWithValidDoubleAttribute)
{
    XMLDocument document;
    std::string xml = "<tag id=\"1.23\"/>";
    document.ParseString(xml);

    EXPECT_FALSE(document.Root() == nullptr);

    EXPECT_TRUE(document.Root()->Match("tag"));
    EXPECT_EQ(size_t(0), document.Root()->Children().size());
    EXPECT_EQ(size_t(0), document.Root()->Elements().size());
    EXPECT_EQ(size_t(1), document.Root()->Attributes().size());

    EXPECT_TRUE(document.Root()->Attributes()[0]->Match("id"));
    EXPECT_EQ(1.23, document.Root()->Attributes()[0]->Value<double>());
    EXPECT_THROW(document.Root()->Attributes()[0]->Value<int>(), XMLValueParseException);
}

TEST_FIXTURE(XMLDocumentTest, CheckExpectAttributeDouble)
{
    XMLDocument document;
    std::string xml = "<tag id=\"1.23\"/>";
    document.ParseString(xml);

    EXPECT_FALSE(document.Root() == nullptr);

    double value;
    EXPECT_TRUE(document.Root()->CheckAttribute("id", value));
    EXPECT_EQ(1.23, value);
    EXPECT_EQ(1.23, document.Root()->ExpectAttribute<double>("id"));

    EXPECT_FALSE(document.Root()->CheckAttribute("ID", value));
    EXPECT_THROW(document.Root()->ExpectAttribute<double>("ID"), XMLException);

    EXPECT_TRUE(document.Root()->CheckAttribute("ID", value, XMLIgnoreCase::IgnoreCase));
    EXPECT_EQ(1.23, value);
    EXPECT_EQ(1.23, document.Root()->ExpectAttribute<double>("ID", XMLIgnoreCase::IgnoreCase));
}

TEST_FIXTURE(XMLDocumentTest, CheckExpectAttributeDoubleInvalid)
{
    XMLDocument document;
    std::string xml = "<tag id=\"abc\"/>";
    document.ParseString(xml);

    EXPECT_FALSE(document.Root() == nullptr);

    double value;
    EXPECT_FALSE(document.Root()->CheckAttribute("id", value));
    EXPECT_FALSE(document.Root()->CheckAttribute("ID", value));
    EXPECT_THROW(document.Root()->ExpectAttribute<double>("id"), XMLValueParseException);
}

TEST_FIXTURE(XMLDocumentTest, CheckExpectAttributeInt)
{
    XMLDocument document;
    std::string xml = "<tag id=\"123\"/>";
    document.ParseString(xml);

    EXPECT_FALSE(document.Root() == nullptr);

    int value;
    EXPECT_TRUE(document.Root()->CheckAttribute("id", value));
    EXPECT_EQ(123, value);
    EXPECT_EQ(123, document.Root()->ExpectAttribute<int>("id"));

    EXPECT_FALSE(document.Root()->CheckAttribute("ID", value));
    EXPECT_THROW(document.Root()->ExpectAttribute<int>("ID"), XMLException);

    EXPECT_TRUE(document.Root()->CheckAttribute("ID", value, XMLIgnoreCase::IgnoreCase));
    EXPECT_EQ(123, value);
    EXPECT_EQ(123, document.Root()->ExpectAttribute<int>("id"));
}

TEST_FIXTURE(XMLDocumentTest, CheckExpectAttributeIntInvalid)
{
    XMLDocument document;
    std::string xml = "<tag id=\"1.23\"/>";
    document.ParseString(xml);

    EXPECT_FALSE(document.Root() == nullptr);

    int value;
    EXPECT_FALSE(document.Root()->CheckAttribute("id", value));
    EXPECT_FALSE(document.Root()->CheckAttribute("ID", value));
    EXPECT_THROW(document.Root()->ExpectAttribute<int>("id"), XMLValueParseException);
}

TEST_FIXTURE(XMLDocumentTest, ParseSimpleStringWithInvalidDoubleAttribute)
{
    XMLDocument document;
    std::string xml = "<tag id=\"abc\"/>";
    document.ParseString(xml);

    EXPECT_FALSE(document.Root() == nullptr);

    EXPECT_TRUE(document.Root()->Match("tag"));
    EXPECT_EQ(size_t(0), document.Root()->Children().size());
    EXPECT_EQ(size_t(0), document.Root()->Elements().size());
    EXPECT_EQ(size_t(1), document.Root()->Attributes().size());

    EXPECT_TRUE(document.Root()->Attributes()[0]->Match("id"));
    EXPECT_THROW(document.Root()->Attributes()[0]->Value<double>(), XMLValueParseException);
}

TEST_FIXTURE(XMLDocumentTest, ParseVerySimpleString)
{
    XMLDocument document;
    std::string xml = "<?xml version=\"1.0\" standalone=\"no\"?>\n"
                        "\n"
                        "<root>\n"
                        "    <element name=\"element1\" />\n"
                        "    <element name=\"element2\" />\n"
                        "</root>\n";
    document.ParseString(xml);

    EXPECT_FALSE(document.Root() == nullptr);
    EXPECT_TRUE(document.Root()->Match("root"));
    EXPECT_EQ(size_t(2), document.Root()->Children().size());
    EXPECT_EQ(size_t(2), document.Root()->Elements().size());

    EXPECT_TRUE(document.Root()->Elements()[0]->Match("element"));
    EXPECT_EQ(size_t(1), document.Root()->Elements()[0]->Attributes().size());

    EXPECT_TRUE(document.Root()->Elements()[0]->Attributes()[0]->Match("name"));
    EXPECT_EQ("element1", document.Root()->Elements()[0]->Attributes()[0]->Value());

    EXPECT_TRUE(document.Root()->Elements()[1]->Match("element"));
    EXPECT_EQ(size_t(1), document.Root()->Elements()[1]->Attributes().size());
    EXPECT_EQ(size_t(0), document.Root()->Elements()[0]->Elements().size());

    EXPECT_TRUE(document.Root()->Elements()[1]->Attributes()[0]->Match("name"));
    EXPECT_EQ("element2", document.Root()->Elements()[1]->Attributes()[0]->Value());
    EXPECT_EQ(size_t(0), document.Root()->Elements()[1]->Elements().size());
}

TEST_FIXTURE(XMLDocumentTest, ParseSimpleString)
{
    XMLDocument document;
    std::string xml = "<?xml version=\"1.0\" standalone=\"no\"?>\n"
                        "\n"
                        "<root>\n"
                        "    <element name=\"element1\">\n"
                        "        <subelement name=\"sub1\" />\n"
                        "        <subelement name=\"sub2\" />\n"
                        "    </element>\n"
                        "    <element name=\"element2\">\n"
                        "        <subelement name=\"sub1\" />\n"
                        "        <subelement name=\"sub2\" />\n"
                        "        <subelement name=\"sub3\" />\n"
                        "    </element>\n"
                        "</root>\n";
    document.ParseString(xml);

    EXPECT_FALSE(document.Root() == nullptr);
    EXPECT_TRUE(document.Root()->Match("root"));
    EXPECT_EQ(size_t(2), document.Root()->Children().size());
    EXPECT_EQ(size_t(2), document.Root()->Elements().size());

    EXPECT_TRUE(document.Root()->Elements()[0]->Match("element"));
    EXPECT_EQ(size_t(1), document.Root()->Elements()[0]->Attributes().size());

    EXPECT_TRUE(document.Root()->Elements()[0]->Attributes()[0]->Match("name"));
    EXPECT_EQ("element1", document.Root()->Elements()[0]->Attributes()[0]->Value());

    EXPECT_EQ(size_t(2), document.Root()->Elements()[0]->Elements().size());

    EXPECT_TRUE(document.Root()->Elements()[0]->Elements()[0]->Match("subelement"));
    EXPECT_EQ(size_t(1), document.Root()->Elements()[0]->Elements()[0]->Attributes().size());

    EXPECT_TRUE(document.Root()->Elements()[0]->Elements()[0]->Attributes()[0]->Match("name"));
    EXPECT_EQ("sub1", document.Root()->Elements()[0]->Elements()[0]->Attributes()[0]->Value());

    EXPECT_TRUE(document.Root()->Elements()[0]->Elements()[1]->Match("subelement"));
    EXPECT_EQ(size_t(1), document.Root()->Elements()[0]->Elements()[1]->Attributes().size());

    EXPECT_TRUE(document.Root()->Elements()[0]->Elements()[1]->Attributes()[0]->Match("name"));
    EXPECT_EQ("sub2", document.Root()->Elements()[0]->Elements()[1]->Attributes()[0]->Value());

    EXPECT_TRUE(document.Root()->Elements()[1]->Match("element"));
    EXPECT_EQ(size_t(1), document.Root()->Elements()[1]->Attributes().size());

    EXPECT_TRUE(document.Root()->Elements()[1]->Attributes()[0]->Match("name"));
    EXPECT_EQ("element2", document.Root()->Elements()[1]->Attributes()[0]->Value());

    EXPECT_EQ(size_t(3), document.Root()->Elements()[1]->Elements().size());

    EXPECT_TRUE(document.Root()->Elements()[1]->Elements()[0]->Match("subelement"));
    EXPECT_EQ(size_t(1), document.Root()->Elements()[1]->Elements()[0]->Attributes().size());

    EXPECT_TRUE(document.Root()->Elements()[1]->Elements()[0]->Attributes()[0]->Match("name"));
    EXPECT_EQ("sub1", document.Root()->Elements()[1]->Elements()[0]->Attributes()[0]->Value());

    EXPECT_TRUE(document.Root()->Elements()[1]->Elements()[1]->Match("subelement"));
    EXPECT_EQ(size_t(1), document.Root()->Elements()[1]->Elements()[1]->Attributes().size());

    EXPECT_TRUE(document.Root()->Elements()[1]->Elements()[1]->Attributes()[0]->Match("name"));
    EXPECT_EQ("sub2", document.Root()->Elements()[1]->Elements()[1]->Attributes()[0]->Value());

    EXPECT_TRUE(document.Root()->Elements()[1]->Elements()[2]->Match("subelement"));
    EXPECT_EQ(size_t(1), document.Root()->Elements()[1]->Elements()[2]->Attributes().size());

    EXPECT_TRUE(document.Root()->Elements()[1]->Elements()[2]->Attributes()[0]->Match("name"));
    EXPECT_EQ("sub3", document.Root()->Elements()[1]->Elements()[2]->Attributes()[0]->Value());

    EXPECT_TRUE(document.Root()->Elements()[2] == nullptr);
}

} // TEST_SUITE(xml)
} // namespace Test
} // namespace XMLParser
