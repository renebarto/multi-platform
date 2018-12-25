#include <unittest-cpp/UnitTestC++.h>

#include <string>
//#include "xml/TestData.h"
#include "json/String.h"

using namespace std;

namespace JSON {
namespace Test {

class StringTest: public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void StringTest::SetUp()
{
}

void StringTest::TearDown()
{
}

TEST_SUITE(json)
{

TEST_FIXTURE(StringTest, Construct)
{
    String target;

    EXPECT_EQ(ValueType::String, target.Type());
    EXPECT_EQ("", target.GetValue());
}

TEST_FIXTURE(StringTest, ConstructInitializer)
{
    std::string value("String");
    String target(value);

    EXPECT_EQ(ValueType::String, target.Type());
    EXPECT_EQ(value, target.GetValue());
}

TEST_FIXTURE(StringTest, DeserializeEmptyString)
{
    String target;

    EXPECT_EQ(ValueType::String, target.Type());
    EXPECT_EQ("", target.GetValue());
    std::basic_istringstream<char> stream("\"\"");
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ("", target.GetValue());
}

TEST_FIXTURE(StringTest, DeserializeString)
{
    String target;

    std::basic_istringstream<char> stream("\"Some text\"");
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ("Some text", target.GetValue());
}

TEST_FIXTURE(StringTest, DeserializeInvalid)
{
    String target;

    std::basic_istringstream<char> stream("garbage");
    EXPECT_FALSE(target.Deserialize(stream));
    EXPECT_EQ("", target.GetValue());
}

TEST_FIXTURE(StringTest, DeserializeEmpty)
{
    String target;

    std::basic_istringstream<char> stream("");
    EXPECT_FALSE(target.Deserialize(stream));
    EXPECT_EQ("", target.GetValue());
}

TEST_FIXTURE(StringTest, DeserializeSpecialChars)
{
    String target;

    std::basic_istringstream<char> stream("\"\\\"\\\\\\/\\b\\f\\n\\r\\t\\u0154\"");
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ("\"\\/\b\f\n\r\t\u0154", target.GetValue());
}

TEST_FIXTURE(StringTest, SerializeEmptyString)
{
    String target;

    std::basic_ostringstream<char> stream;
    target.Serialize(stream);
    EXPECT_EQ("\"\"", stream.str());
}

TEST_FIXTURE(StringTest, SerializeString)
{
    String target;

    std::basic_ostringstream<char> stream;
    target.SetValue("Some text");
    target.Serialize(stream);
    EXPECT_EQ("\"Some text\"", stream.str());
}

} // TEST_SUITE(json)

} // namespace Test
} // namespace JSON

