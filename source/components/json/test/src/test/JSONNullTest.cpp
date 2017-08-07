#include "unit-test-c++/UnitTestC++.h"

#include <string>
//#include "xml/TestData.h"
#include "json/JSONValue.h"

using namespace std;

namespace JSON {
namespace Test {

class JSONNullTest: public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void JSONNullTest::SetUp()
{
}

void JSONNullTest::TearDown()
{
}

TEST_FIXTURE(JSONNullTest, Construct)
{
    Null target;

    EXPECT_EQ(ValueType::Null, target.Type());
}

TEST_FIXTURE(JSONNullTest, Deserialize)
{
    Null target;

    EXPECT_EQ(ValueType::Null, target.Type());
    std::basic_istringstream<OSAL::Char> stream(_("null"));
    EXPECT_TRUE(target.Deserialize(stream));
    stream.str(_("garbage"));
    EXPECT_FALSE(target.Deserialize(stream));
    stream.str(_(""));
    EXPECT_FALSE(target.Deserialize(stream));
}

TEST_FIXTURE(JSONNullTest, Serialize)
{
    Null target;

    EXPECT_EQ(ValueType::Null, target.Type());
    std::basic_ostringstream<OSAL::Char> stream;
    target.Serialize(stream);
    EXPECT_EQ(_("null"), stream.str());
}

} // namespace Test
} // namespace JSON

