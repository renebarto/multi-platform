#include "unit-test-c++/UnitTestC++.h"

#include <string>
//#include "xml/TestData.h"
#include "json/JSONValue.h"

using namespace std;

namespace JSON {
namespace Test {

class JSONBooleanTest: public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void JSONBooleanTest::SetUp()
{
}

void JSONBooleanTest::TearDown()
{
}

TEST_FIXTURE(JSONBooleanTest, Construct)
{
    Boolean target;

    EXPECT_EQ(ValueType::Boolean, target.Type());
    EXPECT_FALSE(target.GetValue());
}

TEST_FIXTURE(JSONBooleanTest, Deserialize)
{
    Boolean target;

    EXPECT_EQ(ValueType::Boolean, target.Type());
    EXPECT_FALSE(target.GetValue());
    std::basic_istringstream<OSAL::Char> stream(_("false"));
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_FALSE(target.GetValue());
    stream.str(_("true"));
    stream.clear();
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_TRUE(target.GetValue());
    stream.str(_("garbage"));
    stream.clear();
    EXPECT_FALSE(target.Deserialize(stream));
    EXPECT_TRUE(target.GetValue());
    stream.str(_(""));
    stream.clear();
    EXPECT_FALSE(target.Deserialize(stream));
    EXPECT_TRUE(target.GetValue());
}

TEST_FIXTURE(JSONBooleanTest, Serialize)
{
    Boolean target;

    EXPECT_EQ(ValueType::Boolean, target.Type());
    EXPECT_FALSE(target.GetValue());
    std::basic_ostringstream<OSAL::Char> stream;
    target.Serialize(stream);
    EXPECT_EQ(_("false"), stream.str());
    stream.str(_(""));
    target.SetValue(true);
    target.Serialize(stream);
    EXPECT_EQ(_("true"), stream.str());
}

TEST_FIXTURE(JSONBooleanTest, Parse)
{
    std::basic_istringstream<OSAL::Char> stream(_("false true garbage"));
    ValuePtr target = Parse(stream);
    EXPECT_NOT_NULL(target);
    EXPECT_EQ(ValueType::Boolean, target->Type());
    EXPECT_FALSE(static_pointer_cast<Boolean>(target)->GetValue());

    target = Parse(stream);
    EXPECT_NOT_NULL(target);
    EXPECT_EQ(ValueType::Boolean, target->Type());
    EXPECT_TRUE(static_pointer_cast<Boolean>(target)->GetValue());

    target = Parse(stream);
    EXPECT_NULL(target);
}

} // namespace Test
} // namespace JSON

