#include <unittest-c++/UnitTestC++.h>

#include <string>
#include "core/Util.h"
#include "staruml/ClassDiagram.h"

using namespace std;

namespace StarUML {
namespace Test {

class ClassDiagramTest: public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();

    const std::string ClassDiagramType = "UMLClassDiagram";
    const OSAL::GUID ClassDiagramID = OSAL::GUID({0x00, 0x00, 0x00, 0x00, 0x01, 0x45, 0xfa, 0xa0,
                                                  0x6d, 0xc8, 0xa3, 0x3b, 0xf6, 0xa6, 0x00, 0x00});
    const std::string ClassDiagramIDText = Core::Util::Base64Encode(ClassDiagramID.GetBytes());
    const std::string ClassDiagramName = "Main";

};

void ClassDiagramTest::SetUp()
{
}

void ClassDiagramTest::TearDown()
{
}

TEST_SUITE(staruml)
{

TEST_FIXTURE(ClassDiagramTest, Construct)
{
    auto target = make_shared<ClassDiagram>(nullptr, ClassDiagramID, ClassDiagramName);

    EXPECT_EQ(ClassDiagramName, target->Name());
    EXPECT_EQ(ClassDiagramID, target->Id());
    EXPECT_EQ(ClassDiagramType, target->Type());
    EXPECT_EQ(nullptr, target->Parent());
    EXPECT_EQ(size_t {0}, target->OwnedElements().size());
}

TEST_FIXTURE(ClassDiagramTest, SerializeEmpty)
{
    auto target = make_shared<ClassDiagram>(nullptr, ClassDiagramID, ClassDiagramName);

    std::stringstream streamExpected;
    streamExpected
        << "{" << endl
        << "    \"_id\" : \"" << ClassDiagramIDText << "\"," << endl
        << "    \"_type\" : \"" << ClassDiagramType << "\"," << endl
        << "    \"defaultDiagram\" : false," << endl
        << "    \"name\" : \"" << ClassDiagramName << "\"," << endl
        << "    \"visible\" : false" << endl
        << "}";

    std::stringstream streamActual;
    target->Serialize(streamActual);

    EXPECT_EQ(streamExpected.str(), streamActual.str());
}

TEST_FIXTURE(ClassDiagramTest, SerializeDefaultClassDiagram)
{
    auto target = make_shared<ClassDiagram>(nullptr, ClassDiagramID, ClassDiagramName);
    target->Visible(true);
    target->DefaultDiagram(true);

    std::stringstream streamExpected;
    streamExpected
        << "{" << endl
        << "    \"_id\" : \"" << ClassDiagramIDText << "\"," << endl
        << "    \"_type\" : \"" << ClassDiagramType << "\"," << endl
        << "    \"defaultDiagram\" : true," << endl
        << "    \"name\" : \"" << ClassDiagramName << "\"," << endl
        << "    \"visible\" : true" << endl
        << "}";

    std::stringstream streamActual;
    target->Serialize(streamActual);

    EXPECT_EQ(streamExpected.str(), streamActual.str());
}

} // TEST_SUITE(staruml)

} // namespace Test
} // namespace StarUML

