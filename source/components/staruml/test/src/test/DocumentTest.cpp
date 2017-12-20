#include <unittest-c++/UnitTestC++.h>

#include <string>
#include "core/Util.h"
#include "staruml/Document.h"
#include "staruml/Model.h"
#include "staruml/ClassDiagram.h"

using namespace std;

namespace StarUML {
namespace Test {

class DocumentTest: public UnitTestCpp::TestFixture
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
    const std::string SubSystemID = "AAAAAAFgBGrRB0uLVvw=";
    const std::string PackageID = "AAAAAAFgBGrh30uOLFU=";
    const std::string PrimitiveTypeID = "AAAAAAFgBGr1V0uRbt8=";
    const std::string DataTypeID = "AAAAAAFgBGsLf0uUXPk=";
    const std::string Model2ID = "AAAAAAFgBNjW9EuYRnI=";
};

void DocumentTest::SetUp()
{
}

void DocumentTest::TearDown()
{
}

TEST_SUITE(staruml)
{

TEST_FIXTURE(DocumentTest, Construct)
{
    Document target;
    target.Name(ProjectName);

    EXPECT_EQ(ProjectName, target.Name());
}

TEST_FIXTURE(DocumentTest, SerializeEmpty)
{
    Document target;
    target.Name(ProjectName);

    std::stringstream streamExpected;
    streamExpected
        << "{" << endl
        << "    \"_id\" : \"" << ProjectIDText << "\"," << endl
        << "    \"_type\" : \"" << ProjectType << "\"," << endl
        << "    \"name\" : \"" << ProjectName << "\"" << endl
        << "}";

    std::stringstream streamActual;
    target.Serialize(streamActual);

    EXPECT_EQ(streamExpected.str(), streamActual.str());
}

TEST_FIXTURE(DocumentTest, SerializeDefaultProject)
{
    Document target;
    target.Name(ProjectName);
    target.SetupDefaultProject();

    const std::shared_ptr<Model> model = dynamic_pointer_cast<Model>(target.GetProject()->OwnedElements()[0]);
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
    target.Serialize(streamActual);

    EXPECT_EQ(streamExpected.str(), streamActual.str());
}

} // TEST_SUITE(staruml)

} // namespace Test
} // namespace StarUML

