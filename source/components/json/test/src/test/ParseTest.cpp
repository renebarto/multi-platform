#include <unittest-c++/UnitTestC++.h>

#include <string>
//#include "xml/TestData.h"
#include "Parse.h"
#include "json/Boolean.h"
#include "json/String.h"
#include "json/Number.h"
#include "json/Null.h"
#include "json/Object.h"
#include "json/Array.h"

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

TEST_SUITE(json)
{

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

TEST_FIXTURE(ParseTest, ParseObject)
{
    std::basic_istringstream<OSAL::Char> stream(_("{ \"x\":true, \"y\":1234 }"));
    ValuePtr target = Parse(stream);
    ASSERT_NOT_NULL(target);
    ASSERT_EQ(ValueType::Object, target->Type());
    auto object = static_pointer_cast<Object>(target);
    EXPECT_EQ(size_t(2), object->Size());
    auto iterator = object->GetIterator();
    EXPECT_EQ(_("x"), (*iterator).GetKey());
    EXPECT_EQ(ValueType::Boolean, (*iterator).GetValue()->Type());
    EXPECT_TRUE(dynamic_pointer_cast<Boolean>((*iterator).GetValue())->GetValue());
    iterator++;
    EXPECT_EQ(_("y"), (*iterator).GetKey());
    EXPECT_EQ(ValueType::Number, (*iterator).GetValue()->Type());
    EXPECT_EQ(_("1234"), dynamic_pointer_cast<Number>((*iterator).GetValue())->GetValue());
}

TEST_FIXTURE(ParseTest, ParseArray)
{
    std::basic_istringstream<OSAL::Char> stream(_("[ true, 1234 ]"));
    ValuePtr target = Parse(stream);
    ASSERT_NOT_NULL(target);
    ASSERT_EQ(ValueType::Array, target->Type());
    auto object = static_pointer_cast<Array>(target);
    EXPECT_EQ(size_t(2), object->Size());
    auto iterator = object->GetIterator();
    EXPECT_EQ(ValueType::Boolean, (*iterator)->Type());
    EXPECT_TRUE(dynamic_pointer_cast<Boolean>(*iterator)->GetValue());
    iterator++;
    EXPECT_EQ(ValueType::Number, (*iterator)->Type());
    EXPECT_EQ(_("1234"), dynamic_pointer_cast<Number>(*iterator)->GetValue());
}

} // TEST_SUITE(json)

} // namespace Test
} // namespace JSON

