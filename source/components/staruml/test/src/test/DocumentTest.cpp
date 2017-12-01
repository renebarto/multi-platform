#include "unit-test-c++/UnitTestC++.h"

#include <string>
#include "core/Util.h"
#include "staruml/Document.h"
#include "staruml/Model.h"

using namespace std;

namespace StarUML {
namespace Test {

class DocumentTest: public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();

    const OSAL::ByteArray ProjectIDBytes = {0x00, 0x00, 0x00, 0x00, 0x01, 0x45, 0xfa, 0x1e, 0x92, 0x8d, 0xa3, 0x36, 0x1d, 0xe7};
    const OSAL::ByteArray ModelIDBytes = {0x00, 0x00, 0x00, 0x00, 0x01, 0x45, 0xfa, 0xa0, 0x56, 0x2b, 0xa3, 0x37, 0x67, 0xc6};
    const OSAL::ByteArray ClassDiagramIDBytes = {0x00, 0x00, 0x00, 0x00, 0x01, 0x45, 0xfa, 0xa0, 0x6d, 0xc8, 0xa3, 0x3b, 0xf6, 0xa6};

    const std::string ProjectType = "Project";
    const std::string ProjectID = "AAAAAAFF+h6SjaM2Hec=";
    const std::string ProjectName = "MyProject";
    const std::string ModelType = "UMLModel";
    const std::string DefaultModelID = "AAAAAAFF+qBWK6M3Z8Y=";
    const std::string DefaultModelName = "Model";
    const ObjectVisibility ModelVisibility = ObjectVisibility::Public;
    const std::string ClassDiagramType = "UMLClassDiagram";
    const std::string DefaultClassDiagramID = "AAAAAAFF+qBtyKM79qY=";
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
        << "    \"_id\" : \"" << ProjectID << "\"," << endl
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

    std::stringstream streamExpected;
    streamExpected
        << "{" << endl
        << "    \"_id\" : \"" << ProjectID << "\"," << endl
        << "    \"_type\" : \"" << ProjectType << "\"," << endl
        << "    \"name\" : \"" << ProjectName << "\"," << endl
        << "    \"ownedElements\" : [" << endl
        << "        {" << endl
        << "            \"_id\" : \"" << DefaultModelID << "\"," << endl
        << "            \"_parent\" : {" << endl
        << "                \"$ref\" : \"" << ProjectID << "\"" << endl
        << "            }," << endl
        << "            \"_type\" : \"" << ModelType << "\"," << endl
        << "            \"name\" : \"" << DefaultModelName << "\"," << endl
        << "            \"ownedElements\" : [" << endl
        << "                {" << endl
        << "                    \"_id\" : \"" << DefaultClassDiagramID << "\"," << endl
        << "                    \"_parent\" : {" << endl
        << "                        \"$ref\" : \"" << DefaultModelID << "\"" << endl
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

