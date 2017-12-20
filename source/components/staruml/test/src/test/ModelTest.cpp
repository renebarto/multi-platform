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
    const OSAL::GUID ModelID  = OSAL::GUID({0x00, 0x00, 0x00, 0x00, 0x01, 0x45, 0xfa, 0xa0,
                                            0x56, 0x2b, 0xa3, 0x37, 0x67, 0xc6, 0x00, 0x00});
    const std::string ModelIDText = Core::Util::Base64Encode(ModelID.GetBytes());
    const std::string ModelName = "Model";
    const ObjectVisibility ModelVisibility = ObjectVisibility::Public;
    const std::string ClassDiagramType = "UMLClassDiagram";
    const OSAL::GUID ClassDiagramID = OSAL::GUID({0x00, 0x00, 0x00, 0x00, 0x01, 0x45, 0xfa, 0xa0,
                                                  0x6d, 0xc8, 0xa3, 0x3b, 0xf6, 0xa6, 0x00, 0x00});
    const std::string ClassDiagramIDText = Core::Util::Base64Encode(ClassDiagramID.GetBytes());
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
    auto diagram = make_shared<ClassDiagram>(target, ClassDiagramID, ClassDiagramName);
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
        << "            \"_id\" : \"" << ClassDiagramIDText << "\"," << endl
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

