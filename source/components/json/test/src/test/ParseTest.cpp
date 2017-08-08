#include "unit-test-c++/UnitTestC++.h"

#include <string>
//#include "xml/TestData.h"
#include "Parse.h"
#include "json/Boolean.h"
#include "json/String.h"
#include "json/Number.h"
#include "json/Null.h"

using namespace std;

namespace JSON {
namespace Test {

class ParseTest: public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void ParseTest::SetUp()
{
}

void ParseTest::TearDown()
{
}

TEST_FIXTURE(ParseTest, ParseKeywords)
{
    std::basic_istringstream<OSAL::Char> stream(_("false true null garbage"));
    ValuePtr target = Parse(stream);
    EXPECT_NOT_NULL(target);
    EXPECT_EQ(ValueType::Boolean, target->Type());
    EXPECT_FALSE(static_pointer_cast<Boolean>(target)->GetValue());

    target = Parse(stream);
    EXPECT_NOT_NULL(target);
    EXPECT_EQ(ValueType::Boolean, target->Type());
    EXPECT_TRUE(static_pointer_cast<Boolean>(target)->GetValue());

    target = Parse(stream);
    EXPECT_NOT_NULL(target);
    EXPECT_EQ(ValueType::Null, target->Type());

    target = Parse(stream);
    EXPECT_NULL(target);
}

TEST_FIXTURE(ParseTest, ParseString)
{
    std::basic_istringstream<OSAL::Char> stream(_("\"false true null garbage\" \"\" "));
    ValuePtr target = Parse(stream);
    EXPECT_NOT_NULL(target);
    EXPECT_EQ(ValueType::String, target->Type());
    EXPECT_EQ(_("false true null garbage"), static_pointer_cast<String>(target)->GetValue());

    target = Parse(stream);
    EXPECT_NOT_NULL(target);
    EXPECT_EQ(ValueType::String, target->Type());
    EXPECT_EQ(_(""), static_pointer_cast<String>(target)->GetValue());

    target = Parse(stream);
    EXPECT_NULL(target);
}

TEST_FIXTURE(ParseTest, ParseNumber)
{
    std::basic_istringstream<OSAL::Char> stream(_("0123 123.456 1.001e+03 abc"));
    ValuePtr target = Parse(stream);
    EXPECT_NOT_NULL(target);
    EXPECT_EQ(ValueType::Number, target->Type());
    EXPECT_EQ(_("0123"), static_pointer_cast<Number>(target)->GetValue());

    target = Parse(stream);
    EXPECT_NOT_NULL(target);
    EXPECT_EQ(ValueType::Number, target->Type());
    EXPECT_EQ(_("123.456"), static_pointer_cast<Number>(target)->GetValue());

    target = Parse(stream);
    EXPECT_NOT_NULL(target);
    EXPECT_EQ(ValueType::Number, target->Type());
    EXPECT_EQ(_("1.001e+03"), static_pointer_cast<Number>(target)->GetValue());

    target = Parse(stream);
    EXPECT_NULL(target);
}

} // namespace Test
} // namespace JSON

