#include <unittest-cpp/UnitTestC++.h>

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

TEST_SUITE(json)
{

TEST_FIXTURE(NullTest, Construct)
{
    Null target;

    EXPECT_EQ(ValueType::Null, target.Type());
}

TEST_FIXTURE(NullTest, Deserialize)
{
    Null target;

    EXPECT_EQ(ValueType::Null, target.Type());
    std::basic_istringstream<char> stream("null");
    EXPECT_TRUE(target.Deserialize(stream));
    stream.str("garbage");
    EXPECT_FALSE(target.Deserialize(stream));
    stream.str("");
    EXPECT_FALSE(target.Deserialize(stream));
}

TEST_FIXTURE(NullTest, Serialize)
{
    Null target;

    EXPECT_EQ(ValueType::Null, target.Type());
    std::basic_ostringstream<char> stream;
    target.Serialize(stream);
    EXPECT_EQ("null", stream.str());
}

} // TEST_SUITE(json)

} // namespace Test
} // namespace JSON

