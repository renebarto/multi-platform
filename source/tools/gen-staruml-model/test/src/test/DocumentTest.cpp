#include "unit-test-c++/UnitTestC++.h"

#include <string>
#include "core/Util.h"
#include "staruml/Document.h"

using namespace std;

namespace StarUML {
namespace Test {

class DocumentTest: public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void DocumentTest::SetUp()
{
}

void DocumentTest::TearDown()
{
}

TEST_SUITE(json)
{

const std::string ProjectType = "Project";
const std::string ProjectID = "AAAAAAFF+h6SjaM2Hec=";
const std::string ProjectName = "MyProject";
const std::string ModelType = "UMLModel";
const std::string DefaultModelID = "AAAAAAFF+qBWK6M3Z8Y=";
const std::string DefaultModelName = "Model";
const std::string ClassDiagramType = "UMLClassDiagram";
const std::string DefaultClassDiagramID = "AAAAAAFF+qBtyKM79qY=";
const std::string DefaultClassDiagramName = "Main";

TEST_FIXTURE(DocumentTest, Construct)
{
    Document target;
    target.Name(ProjectName);

    EXPECT_EQ(ProjectName, target.Name());
    EXPECT_EQ(size_t {0}, target.OwnedElements().size());
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
        << "            \"_type\" : \"" << ModelType << "\"," << endl
        << "            \"name\" : \"" << DefaultModelName << "\"," << endl
        << "            \"ownedElements\" : [" << endl
        << "                {" << endl
        << "                    \"_id\" : \"" << DefaultClassDiagramID << "\"," << endl
        << "                    \"_type\" : \"" << ClassDiagramType << "\"," << endl
        << "                    \"name\" : \"" << DefaultClassDiagramName << "\"" << endl
        << "                }" << endl
        << "            ]" << endl
        << "        }" << endl
        << "    ]" << endl
        << "}";

    std::stringstream streamActual;
    target.Serialize(streamActual);

    EXPECT_EQ(streamExpected.str(), streamActual.str());
}

} // TEST_SUITE(json)

} // namespace Test
} // namespace StarUML

