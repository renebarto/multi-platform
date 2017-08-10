#include "unit-test-c++/UnitTestC++.h"

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

TEST_FIXTURE(StringTest, Construct)
{
    String target;

    EXPECT_EQ(ValueType::String, target.Type());
    EXPECT_EQ(_(""), target.GetValue());
}

TEST_FIXTURE(StringTest, ConstructInitializer)
{
    OSAL::String value("String");
    String target(value);

    EXPECT_EQ(ValueType::String, target.Type());
    EXPECT_EQ(value, target.GetValue());
}

TEST_FIXTURE(StringTest, DeserializeEmptyString)
{
    String target;

    EXPECT_EQ(ValueType::String, target.Type());
    EXPECT_EQ(_(""), target.GetValue());
    std::basic_istringstream<OSAL::Char> stream(_("\"\""));
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(_(""), target.GetValue());
}

TEST_FIXTURE(StringTest, DeserializeString)
{
    String target;

    std::basic_istringstream<OSAL::Char> stream(_("\"Some text\""));
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(_("Some text"), target.GetValue());
}

TEST_FIXTURE(StringTest, DeserializeInvalid)
{
    String target;

    std::basic_istringstream<OSAL::Char> stream(_("garbage"));
    EXPECT_FALSE(target.Deserialize(stream));
    EXPECT_EQ(_(""), target.GetValue());
}

TEST_FIXTURE(StringTest, DeserializeEmpty)
{
    String target;

    std::basic_istringstream<OSAL::Char> stream(_(""));
    EXPECT_FALSE(target.Deserialize(stream));
    EXPECT_EQ(_(""), target.GetValue());
}

TEST_FIXTURE(StringTest, SerializeEmptyString)
{
    String target;

    std::basic_ostringstream<OSAL::Char> stream;
    target.Serialize(stream);
    EXPECT_EQ(_("\"\""), stream.str());
}

TEST_FIXTURE(StringTest, SerializeString)
{
    String target;

    std::basic_ostringstream<OSAL::Char> stream;
    target.SetValue(_("Some text"));
    target.Serialize(stream);
    EXPECT_EQ(_("\"Some text\""), stream.str());
}

} // namespace Test
} // namespace JSON

