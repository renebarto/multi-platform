#include <unittest-c++/UnitTestC++.h>

#include <string>
#include "core/Util.h"
#include "staruml/Package.h"

using namespace std;

namespace StarUML {
namespace Test {

class PackageTest: public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();

    const std::string PackageType = "UMLPackage";
    const OSAL::GUID PackageID = OSAL::GUID({0x00, 0x00, 0x00, 0x00, 0x01, 0x45, 0xfa, 0xa0,
                                                  0x6d, 0xc8, 0xa3, 0x3b, 0xf6, 0xa6, 0x00, 0x00});
    const std::string PackageIDText = Core::Util::Base64Encode(PackageID.GetBytes());
    const std::string PackageName = "OSAL";

};

void PackageTest::SetUp()
{
}

void PackageTest::TearDown()
{
}

TEST_SUITE(staruml)
{

TEST_FIXTURE(PackageTest, ConstructParent)
{
    auto target = make_shared<Package>(nullptr);

    EXPECT_EQ("", target->Name());
    EXPECT_EQ(OSAL::GUID::Null, target->Id());
    EXPECT_EQ(PackageType, target->Type());
    EXPECT_EQ(nullptr, target->Parent());
    EXPECT_EQ(size_t {0}, target->OwnedElements().size());
    EXPECT_EQ(ObjectVisibility::Public, target->Visibility());
}

TEST_FIXTURE(PackageTest, ConstructParentID)
{
    auto target = make_shared<Package>(nullptr, PackageID);

    EXPECT_EQ("", target->Name());
    EXPECT_EQ(PackageID, target->Id());
    EXPECT_EQ(PackageType, target->Type());
    EXPECT_EQ(nullptr, target->Parent());
    EXPECT_EQ(size_t {0}, target->OwnedElements().size());
    EXPECT_EQ(ObjectVisibility::Public, target->Visibility());
}

TEST_FIXTURE(PackageTest, ConstructParentIDName)
{
    auto target = make_shared<Package>(nullptr, PackageID, PackageName);

    EXPECT_EQ(PackageName, target->Name());
    EXPECT_EQ(PackageID, target->Id());
    EXPECT_EQ(PackageType, target->Type());
    EXPECT_EQ(nullptr, target->Parent());
    EXPECT_EQ(size_t {0}, target->OwnedElements().size());
    EXPECT_EQ(ObjectVisibility::Public, target->Visibility());
}

TEST_FIXTURE(PackageTest, SerializeEmpty)
{
    auto target = make_shared<Package>(nullptr, PackageID, PackageName);

    std::stringstream streamExpected;
    streamExpected
        << "{" << endl
        << "    \"_id\" : \"" << PackageIDText << "\"," << endl
        << "    \"_type\" : \"" << PackageType << "\"," << endl
        << "    \"name\" : \"" << PackageName << "\"," << endl
        << "    \"visibility\" : \"public\"" << endl
        << "}";

    std::stringstream streamActual;
    target->Serialize(streamActual);

    EXPECT_EQ(streamExpected.str(), streamActual.str());
}

TEST_FIXTURE(PackageTest, SerializeDefaultPackage)
{
    auto target = make_shared<Package>(nullptr, PackageID, PackageName);
    target->Visibility(ObjectVisibility::Protected);

    std::stringstream streamExpected;
    streamExpected
        << "{" << endl
        << "    \"_id\" : \"" << PackageIDText << "\"," << endl
        << "    \"_type\" : \"" << PackageType << "\"," << endl
        << "    \"name\" : \"" << PackageName << "\"," << endl
        << "    \"visibility\" : \"protected\"" << endl
        << "}";

    std::stringstream streamActual;
    target->Serialize(streamActual);

    EXPECT_EQ(streamExpected.str(), streamActual.str());
}

} // TEST_SUITE(staruml)

} // namespace Test
} // namespace StarUML

