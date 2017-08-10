#include "unit-test-c++/UnitTestC++.h"

#include <string>
#include "json/Boolean.h"
#include "json/Null.h"
#include "json/Number.h"
#include "json/String.h"
#include "json/Object.h"
#include "json/Array.h"

using namespace std;

namespace JSON {
namespace Test {

class ArrayTest: public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void ArrayTest::SetUp()
{
}

void ArrayTest::TearDown()
{
}

TEST_FIXTURE(ArrayTest, Construct)
{
    Array target;

    EXPECT_EQ(ValueType::Array, target.Type());
    EXPECT_EQ(size_t {0}, target.Size());
}

TEST_FIXTURE(ArrayTest, ConstructInitializerSingleValue)
{
    ValuePtr value = std::make_shared<Boolean>(true);

    Array target({value});

    EXPECT_EQ(ValueType::Array, target.Type());
    ASSERT_EQ(size_t {1}, target.Size());
    auto iterator = target.GetIterator();
    EXPECT_EQ(value, (*iterator));
}

TEST_FIXTURE(ArrayTest, ConstructInitializerMultipleValues)
{
    ValuePtr value1 = std::make_shared<Boolean>(true);
    ValuePtr value2 = std::make_shared<String>(_("Some text"));

    Array target({value1, value2});

    EXPECT_EQ(ValueType::Array, target.Type());
    ASSERT_EQ(size_t {2}, target.Size());
    auto iterator = target.GetIterator();
    EXPECT_EQ(value1, (*iterator));
    ++iterator;
    EXPECT_EQ(value2, (*iterator));
}

TEST_FIXTURE(ArrayTest, AddValue)
{
    Array target;
    ValuePtr value1 = std::make_shared<Boolean>(true);
    ValuePtr value2 = std::make_shared<String>(_("Some text"));
    ASSERT_EQ(size_t {0}, target.Size());
    target.AddValue(value1);
    ASSERT_EQ(size_t {1}, target.Size());
    target.AddValue(value2);
    ASSERT_EQ(size_t {2}, target.Size());
    auto iterator = target.GetIterator();
    EXPECT_EQ(value1, (*iterator));
    ++iterator;
    EXPECT_EQ(value2, (*iterator));
}

TEST_FIXTURE(ArrayTest, DeserializeSingleValue)
{
    Array target;

    std::basic_istringstream<OSAL::Char> stream(_("[true]"));
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(ValueType::Array, target.Type());
    ASSERT_EQ(size_t {1}, target.Size());
    auto iterator = target.GetIterator();
    ASSERT_NOT_NULL(*iterator);
    ASSERT_EQ(ValueType::Boolean, (*iterator)->Type());
    EXPECT_TRUE(dynamic_pointer_cast<Boolean>(*iterator)->GetValue());
}

TEST_FIXTURE(ArrayTest, DeserializeMultipleValues)
{
    Array target;

    std::basic_istringstream<OSAL::Char> stream(_("[\n")
                                                _("    true,\n")
                                                _("    false,\n")
                                                _("    null,\n")
                                                _("    12345678\n")
                                                _("]"));
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(ValueType::Array, target.Type());
    ASSERT_EQ(size_t {4}, target.Size());

    auto iterator = target.GetIterator();
    ASSERT_NOT_NULL(*iterator);
    ASSERT_EQ(ValueType::Boolean, (*iterator)->Type());
    EXPECT_TRUE(dynamic_pointer_cast<Boolean>(*iterator)->GetValue());

    ++iterator;
    ASSERT_NOT_NULL(*iterator);
    ASSERT_EQ(ValueType::Boolean, (*iterator)->Type());
    EXPECT_FALSE(dynamic_pointer_cast<Boolean>(*iterator)->GetValue());

    ++iterator;
    ASSERT_NOT_NULL(*iterator);
    ASSERT_EQ(ValueType::Null, (*iterator)->Type());

    ++iterator;
    ASSERT_NOT_NULL(*iterator);
    ASSERT_EQ(ValueType::Number, (*iterator)->Type());
    EXPECT_EQ(_("12345678"), dynamic_pointer_cast<Number>(*iterator)->GetValue());
}

TEST_FIXTURE(ArrayTest, DeserializeMultipleValuesForgottenComma)
{
    Array target;

    std::basic_istringstream<OSAL::Char> stream(_("[\n")
                                                _("    true,\n")
                                                _("    false,\n")
                                                _("    null\n")
                                                _("    12345678\n")
                                                _("]"));
    EXPECT_FALSE(target.Deserialize(stream));
    EXPECT_EQ(ValueType::Array, target.Type());
    ASSERT_EQ(size_t {3}, target.Size());

    auto iterator = target.GetIterator();
    ASSERT_NOT_NULL(*iterator);
    ASSERT_EQ(ValueType::Boolean, (*iterator)->Type());
    EXPECT_TRUE(dynamic_pointer_cast<Boolean>(*iterator)->GetValue());

    ++iterator;
    ASSERT_NOT_NULL(*iterator);
    ASSERT_EQ(ValueType::Boolean, (*iterator)->Type());
    EXPECT_FALSE(dynamic_pointer_cast<Boolean>(*iterator)->GetValue());

    ++iterator;
    ASSERT_NOT_NULL(*iterator);
    ASSERT_EQ(ValueType::Null, (*iterator)->Type());
}

TEST_FIXTURE(ArrayTest, DeserializeMultipleValuesExtraComma)
{
    Array target;

    std::basic_istringstream<OSAL::Char> stream(_("[\n")
                                                _("    true,\n")
                                                _("    false,\n")
                                                _("    null,\n")
                                                _("    12345678,\n")
                                                _("]"));
    EXPECT_FALSE(target.Deserialize(stream));
    EXPECT_EQ(ValueType::Array, target.Type());
    ASSERT_EQ(size_t {4}, target.Size());

    auto iterator = target.GetIterator();
    ASSERT_NOT_NULL(*iterator);
    ASSERT_EQ(ValueType::Boolean, (*iterator)->Type());
    EXPECT_TRUE(dynamic_pointer_cast<Boolean>(*iterator)->GetValue());

    ++iterator;
    ASSERT_NOT_NULL(*iterator);
    ASSERT_EQ(ValueType::Boolean, (*iterator)->Type());
    EXPECT_FALSE(dynamic_pointer_cast<Boolean>(*iterator)->GetValue());

    ++iterator;
    ASSERT_NOT_NULL(*iterator);
    ASSERT_EQ(ValueType::Null, (*iterator)->Type());

    ++iterator;
    ASSERT_NOT_NULL(*iterator);
    ASSERT_EQ(ValueType::Number, (*iterator)->Type());
    EXPECT_EQ(_("12345678"), dynamic_pointer_cast<Number>(*iterator)->GetValue());
}

TEST_FIXTURE(ArrayTest, DeserializeEmptyArray)
{
    Array target;

    std::basic_istringstream<OSAL::Char> stream(_("[]"));
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(ValueType::Array, target.Type());
    ASSERT_EQ(size_t {0}, target.Size());
}

TEST_FIXTURE(ArrayTest, DeserializeNestedObject)
{
    Array target;

    std::basic_istringstream<OSAL::Char> stream(_("[ true, { \"z\" : 1234 }]"));
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(ValueType::Array, target.Type());
    ASSERT_EQ(size_t {2}, target.Size());

    auto iterator = target.GetIterator();
    ASSERT_EQ(ValueType::Boolean, (*iterator)->Type());
    EXPECT_TRUE(dynamic_pointer_cast<Boolean>(*iterator)->GetValue());

    ++iterator;
    ASSERT_EQ(ValueType::Object, (*iterator)->Type());

    auto object = dynamic_pointer_cast<Object>(*iterator);
    ASSERT_NOT_NULL(object);
    ASSERT_EQ(size_t {1}, object->Size());

    auto iteratorNested = object->GetIterator();
    EXPECT_EQ(_("z"), (*iteratorNested).GetKey());
    ASSERT_NOT_NULL((*iteratorNested).GetValue());
    ASSERT_EQ(ValueType::Number, (*iteratorNested).GetValue()->Type());
    EXPECT_EQ(_("1234"), dynamic_pointer_cast<Number>((*iteratorNested).GetValue())->GetValue());
}

TEST_FIXTURE(ArrayTest, DeserializeNestedArray)
{
    Array target;

    std::basic_istringstream<OSAL::Char> stream(_("[ true, [ \"z\", 1234 ]]"));
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(ValueType::Array, target.Type());
    ASSERT_EQ(size_t {2}, target.Size());

    auto iterator = target.GetIterator();
    ASSERT_EQ(ValueType::Boolean, (*iterator)->Type());
    EXPECT_TRUE(dynamic_pointer_cast<Boolean>(*iterator)->GetValue());

    ++iterator;
    ASSERT_EQ(ValueType::Array, (*iterator)->Type());

    auto object = dynamic_pointer_cast<Array>(*iterator);
    ASSERT_NOT_NULL(object);
    ASSERT_EQ(size_t {2}, object->Size());

    auto iteratorNested = object->GetIterator();
    ASSERT_EQ(ValueType::String, (*iteratorNested)->Type());
    EXPECT_EQ(_("z"), dynamic_pointer_cast<String>(*iteratorNested)->GetValue());
    iteratorNested++;
    ASSERT_EQ(ValueType::Number, (*iteratorNested)->Type());
    EXPECT_EQ(_("1234"), dynamic_pointer_cast<Number>(*iteratorNested)->GetValue());
}

TEST_FIXTURE(ArrayTest, Serialize)
{
    ValuePtr value1 = std::make_shared<Boolean>(true);
    ValuePtr value2 = std::make_shared<Boolean>(false);
    ValuePtr value3 = std::make_shared<Null>();
    ValuePtr value4 = std::make_shared<Number>(_("12345678"));
    auto value5 = std::make_shared<Object>();
    value5->AddPair(KVPair(_("x"), std::make_shared<Boolean>(true)));
    auto value6 = std::make_shared<Array>();
    value6->AddValue(std::make_shared<Boolean>(true));
    value6->AddValue(std::make_shared<Boolean>(false));

    Array target(
    {
        value1,
        value2,
        value3,
        value4,
        value5,
        value6
    });

    std::basic_ostringstream<OSAL::Char> stream;
    target.Serialize(stream);
    EXPECT_EQ(_("[\n")
              _("    true,\n")
              _("    false,\n")
              _("    null,\n")
              _("    12345678,\n")
              _("    {\n")
              _("        \"x\" : true\n")
              _("    },\n")
              _("    [\n")
              _("        true,\n")
              _("        false\n")
              _("    ]\n")
              _("]"), stream.str());
}

} // namespace Test
} // namespace JSON

