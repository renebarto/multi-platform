#include "unit-test-c++/UnitTestC++.h"

#include <string>
//#include "xml/TestData.h"
#include "json/Null.h"

using namespace std;

namespace JSON {
namespace Test {

class NullTest: public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void NullTest::SetUp()
{
}

void NullTest::TearDown()
{
}

TEST_FIXTURE(NullTest, Construct)
{
    Null target;

    EXPECT_EQ(ValueType::Null, target.Type());
}

TEST_FIXTURE(NullTest, Deserialize)
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

TEST_FIXTURE(NullTest, Serialize)
{
    Null target;

    EXPECT_EQ(ValueType::Null, target.Type());
    std::basic_ostringstream<OSAL::Char> stream;
    target.Serialize(stream);
    EXPECT_EQ(_("null"), stream.str());
}

} // namespace Test
} // namespace JSON

