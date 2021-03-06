#include <unittest-cpp/UnitTestC++.h>

#include <string>
//#include "xml/TestData.h"
#include "json/Boolean.h"

using namespace std;

namespace JSON {
namespace Test {

class BooleanTest: public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void BooleanTest::SetUp()
{
}

void BooleanTest::TearDown()
{
}

TEST_SUITE(json)
{

TEST_FIXTURE(BooleanTest, Construct)
{
    Boolean target;

    EXPECT_EQ(ValueType::Boolean, target.Type());
    EXPECT_FALSE(target.GetValue());
}

TEST_FIXTURE(BooleanTest, ConstructInitializer)
{
    bool value = true;
    Boolean target(value);

    EXPECT_EQ(ValueType::Boolean, target.Type());
    EXPECT_EQ(value, target.GetValue());
}

TEST_FIXTURE(BooleanTest, DeserializeFalse)
{
    Boolean target;

    EXPECT_EQ(ValueType::Boolean, target.Type());
    EXPECT_FALSE(target.GetValue());
    std::basic_istringstream<char> stream("false");
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_FALSE(target.GetValue());
}

TEST_FIXTURE(BooleanTest, DeserializeTrue)
{
    Boolean target;

    EXPECT_EQ(ValueType::Boolean, target.Type());
    EXPECT_FALSE(target.GetValue());
    std::basic_istringstream<char> stream("true");
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_TRUE(target.GetValue());
}

TEST_FIXTURE(BooleanTest, DeserializeInvalid)
{
    Boolean target;

    EXPECT_EQ(ValueType::Boolean, target.Type());
    EXPECT_FALSE(target.GetValue());
    std::basic_istringstream<char> stream("garbage");
    EXPECT_FALSE(target.Deserialize(stream));
    EXPECT_FALSE(target.GetValue());
}

TEST_FIXTURE(BooleanTest, DeserializeEmpty)
{
    Boolean target;

    EXPECT_EQ(ValueType::Boolean, target.Type());
    EXPECT_FALSE(target.GetValue());
    std::basic_istringstream<char> stream("");
    EXPECT_FALSE(target.Deserialize(stream));
    EXPECT_FALSE(target.GetValue());
}

TEST_FIXTURE(BooleanTest, SerializeFalse)
{
    Boolean target;

    std::basic_ostringstream<char> stream;
    target.SetValue(false);
    target.Serialize(stream);
    EXPECT_EQ("false", stream.str());
}

TEST_FIXTURE(BooleanTest, SerializeTrue)
{
    Boolean target;

    std::basic_ostringstream<char> stream;
    target.SetValue(true);
    target.Serialize(stream);
    EXPECT_EQ("true", stream.str());
}

} // TEST_SUITE(json)

} // namespace Test
} // namespace JSON

