#include <unittest-c++/UnitTestC++.h>

#include <string>
#include "core/Util.h"
#include "staruml/ClassDiagram.h"
#include "staruml/Model.h"

using namespace std;

namespace StarUML {
namespace Test {

class ModelTest: public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();

    const std::string ModelType = "UMLModel";
    const OSAL::ByteArray ModelID = {0x00, 0x00, 0x00, 0x00, 0x01, 0x45, 0xfa, 0xa0,
                                     0x56, 0x2b, 0xa3, 0x37, 0x67, 0xc6};
    const std::string ModelIDText = "AAAAAAFF+qBWK6M3Z8Y=";
    const std::string ModelName = "Model";
    const ObjectVisibility ModelVisibility = ObjectVisibility::Public;
    const std::string ClassDiagramType = "UMLClassDiagram";
    const std::string ClassDiagramID = "AAAAAAFF+qBtyKM79qY=";
    const std::string ClassDiagramName = "Main";

};

void ModelTest::SetUp()
{
}

void ModelTest::TearDown()
{
}

TEST_SUITE(staruml)
{

TEST_FIXTURE(ModelTest, Construct)
{
    auto target = make_shared<Model>(nullptr, ModelID, ModelName);

    EXPECT_EQ(ModelName, target->Name());
    EXPECT_EQ(ModelID, target->Id());
    EXPECT_EQ(ModelType, target->Type());
    EXPECT_EQ(nullptr, target->Parent());
    EXPECT_EQ(size_t {0}, target->OwnedElements().size());
}

TEST_FIXTURE(ModelTest, SerializeEmpty)
{
    auto target = make_shared<Model>(nullptr, ModelID, ModelName);

    std::stringstream streamExpected;
    streamExpected
        << "{" << endl
        << "    \"_id\" : \"" << ModelIDText << "\"," << endl
        << "    \"_type\" : \"" << ModelType << "\"," << endl
        << "    \"name\" : \"" << ModelName << "\"," << endl
        << "    \"visibility\" : \"" << ModelVisibility << "\"" << endl
        << "}";

    std::stringstream streamActual;
    target->Serialize(streamActual);

    EXPECT_EQ(streamExpected.str(), streamActual.str());
}

TEST_FIXTURE(ModelTest, SerializeDefaultModel)
{
    auto target = make_shared<Model>(nullptr, ModelID, ModelName);
    auto diagram = make_shared<ClassDiagram>(target, Core::Util::Base64Decode(ClassDiagramID), ClassDiagramName);
    diagram->Visible(true);
    diagram->DefaultDiagram(true);
    target->AddElement(diagram);

    std::stringstream streamExpected;
    streamExpected
        << "{" << endl
        << "    \"_id\" : \"" << ModelIDText << "\"," << endl
        << "    \"_type\" : \"" << ModelType << "\"," << endl
        << "    \"name\" : \"" << ModelName << "\"," << endl
        << "    \"ownedElements\" : [" << endl
        << "        {" << endl
        << "            \"_id\" : \"" << ClassDiagramID << "\"," << endl
        << "            \"_parent\" : {" << endl
        << "                \"$ref\" : \"" << ModelIDText << "\"" << endl
        << "            }," << endl
        << "            \"_type\" : \"" << ClassDiagramType << "\"," << endl
        << "            \"defaultDiagram\" : true," << endl
        << "            \"name\" : \"" << ClassDiagramName << "\"," << endl
        << "            \"visible\" : true" << endl
        << "        }" << endl
        << "    ]," << endl
        << "    \"visibility\" : \"" << ModelVisibility << "\"" << endl
        << "}";

    std::stringstream streamActual;
    target->Serialize(streamActual);

    EXPECT_EQ(streamExpected.str(), streamActual.str());
}

} // TEST_SUITE(staruml)

} // namespace Test
} // namespace StarUML

