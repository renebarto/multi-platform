#include <unittest-c++/UnitTestC++.h>

#include <string>
#include "core/Util.h"
#include "staruml/Project.h"
#include "staruml/Model.h"
#include "staruml/ClassDiagram.h"

using namespace std;

namespace StarUML {
namespace Test {

class ProjectTest: public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();

    const std::string ProjectType = "Project";
    const OSAL::GUID ProjectID = OSAL::GUID({0x00, 0x00, 0x00, 0x00, 0x01, 0x45, 0xfa, 0x1e,
                                             0x92, 0x8d, 0xa3, 0x36, 0x1d, 0xe7, 0x00, 0x00});
    const std::string ProjectIDText = Core::Util::Base64Encode(ProjectID.GetBytes());
    const std::string ProjectName = "MyProject";
    const std::string ModelType = "UMLModel";
    const std::string DefaultModelName = "Model";
    const ObjectVisibility ModelVisibility = ObjectVisibility::Public;
    const std::string ClassDiagramType = "UMLClassDiagram";
    const std::string DefaultClassDiagramName = "Main";

};

void ProjectTest::SetUp()
{
}

void ProjectTest::TearDown()
{
}

TEST_SUITE(staruml)
{

TEST_FIXTURE(ProjectTest, Construct)
{
    auto target = make_shared<Project>(ProjectID, ProjectName);

    EXPECT_EQ(ProjectName, target->Name());
    EXPECT_EQ(size_t {0}, target->OwnedElements().size());
}

TEST_FIXTURE(ProjectTest, SerializeEmpty)
{
    auto target = make_shared<Project>(ProjectID, ProjectName);

    std::stringstream streamExpected;
    streamExpected
        << "{" << endl
        << "    \"_id\" : \"" << ProjectIDText << "\"," << endl
        << "    \"_type\" : \"" << ProjectType << "\"," << endl
        << "    \"name\" : \"" << ProjectName << "\"" << endl
        << "}";

    std::stringstream streamActual;
    target->Serialize(streamActual);

    EXPECT_EQ(streamExpected.str(), streamActual.str());
}

TEST_FIXTURE(ProjectTest, SerializeDefaultProject)
{
    auto target = make_shared<Project>(ProjectID, ProjectName);
    target->SetupDefault();

    const std::shared_ptr<Model> model = dynamic_pointer_cast<Model>(target->OwnedElements()[0]);
    const OSAL::GUID ModelID = model->Id();
    const std::string ModelIDText = Core::Util::Base64Encode(ModelID.GetBytes());
    const std::shared_ptr<ClassDiagram> classDiagram = dynamic_pointer_cast<ClassDiagram>(model->OwnedElements()[0]);
    const OSAL::GUID ClassDiagramID = classDiagram->Id();
    const std::string ClassDiagramIDText = Core::Util::Base64Encode(ClassDiagramID.GetBytes());

    std::stringstream streamExpected;
    streamExpected
        << "{" << endl
        << "    \"_id\" : \"" << ProjectIDText << "\"," << endl
        << "    \"_type\" : \"" << ProjectType << "\"," << endl
        << "    \"name\" : \"" << ProjectName << "\"," << endl
        << "    \"ownedElements\" : [" << endl
        << "        {" << endl
        << "            \"_id\" : \"" << ModelIDText << "\"," << endl
        << "            \"_parent\" : {" << endl
        << "                \"$ref\" : \"" << ProjectIDText << "\"" << endl
        << "            }," << endl
        << "            \"_type\" : \"" << ModelType << "\"," << endl
        << "            \"name\" : \"" << DefaultModelName << "\"," << endl
        << "            \"ownedElements\" : [" << endl
        << "                {" << endl
        << "                    \"_id\" : \"" << ClassDiagramIDText << "\"," << endl
        << "                    \"_parent\" : {" << endl
        << "                        \"$ref\" : \"" << ModelIDText << "\"" << endl
        << "                    }," << endl
        << "                    \"_type\" : \"" << ClassDiagramType << "\"," << endl
        << "                    \"defaultDiagram\" : true," << endl
        << "                    \"name\" : \"" << DefaultClassDiagramName << "\"," << endl
        << "                    \"visible\" : true" << endl
        << "                }" << endl
        << "            ]," << endl
        << "            \"visibility\" : \"" << ModelVisibility << "\"" << endl
        << "        }" << endl
        << "    ]" << endl
        << "}";

    std::stringstream streamActual;
    target->Serialize(streamActual);

    EXPECT_EQ(streamExpected.str(), streamActual.str());
}

} // TEST_SUITE(staruml)

} // namespace Test
} // namespace StarUML

