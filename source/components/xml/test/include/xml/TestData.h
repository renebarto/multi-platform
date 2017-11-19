#pragma once

#include <osal/Strings.h>
#include <osal/Path.h>

namespace XMLParser
{

namespace Test
{

class XMLParserTestData
{
public:
    static const OSAL::String ProjectName() { return "xml"; }

    static OSAL::String TestDirectory()
    {
        return OSAL::ToString(TEST_DATA_ROOT);
    }

    static std::string TestDirectoryXMLParser() { return OSAL::Path::CombinePath(TestDirectory(), ProjectName()); }

    static const std::string EmptyXML() { return OSAL::Path::CombinePath(TestDirectoryXMLParser(), "empty.xml"); }
    static const std::string DeclarationOnlyXML() { return OSAL::Path::CombinePath(TestDirectoryXMLParser(), "declarationOnly.xml"); }
    static const std::string RootOnlyXML() { return OSAL::Path::CombinePath(TestDirectoryXMLParser(), "rootOnly.xml"); }
    static const std::string VerySimpleXML() { return OSAL::Path::CombinePath(TestDirectoryXMLParser(), "verySimple.xml"); }
    static const std::string SimpleXML() { return OSAL::Path::CombinePath(TestDirectoryXMLParser(), "simple.xml"); }
    static const std::string EmptyXMLRef() { return OSAL::Path::CombinePath(TestDirectoryXMLParser(), "empty.ref.xml"); }
    static const std::string DeclarationOnlyXMLRef() { return OSAL::Path::CombinePath(TestDirectoryXMLParser(), "declarationOnly.ref.xml"); }
    static const std::string RootOnlyXMLRef() { return OSAL::Path::CombinePath(TestDirectoryXMLParser(), "rootOnly.ref.xml"); }
    static const std::string VerySimpleXMLRef() { return OSAL::Path::CombinePath(TestDirectoryXMLParser(), "verySimple.ref.xml"); }
    static const std::string SimpleXMLRef() { return OSAL::Path::CombinePath(TestDirectoryXMLParser(), "simple.ref.xml"); }
};

} // namespace Test

} // namespace XMLParser

