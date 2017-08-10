#include "unit-test-c++/UnitTestC++.h"

#include <string>
//#include "xml/TestData.h"
#include "json/Boolean.h"
#include "json/Null.h"
#include "json/Number.h"
#include "json/String.h"
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
    EXPECT_NOT_NULL(*iterator);
    EXPECT_EQ(ValueType::Boolean, (*iterator)->Type());
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
    EXPECT_NOT_NULL(*iterator);
    EXPECT_EQ(ValueType::Boolean, (*iterator)->Type());
    EXPECT_TRUE(dynamic_pointer_cast<Boolean>(*iterator)->GetValue());

    ++iterator;
    EXPECT_NOT_NULL(*iterator);
    EXPECT_EQ(ValueType::Boolean, (*iterator)->Type());
    EXPECT_FALSE(dynamic_pointer_cast<Boolean>(*iterator)->GetValue());

    ++iterator;
    EXPECT_NOT_NULL(*iterator);
    EXPECT_EQ(ValueType::Null, (*iterator)->Type());

    ++iterator;
    EXPECT_NOT_NULL(*iterator);
    EXPECT_EQ(ValueType::Number, (*iterator)->Type());
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
    EXPECT_NOT_NULL(*iterator);
    EXPECT_EQ(ValueType::Boolean, (*iterator)->Type());
    EXPECT_TRUE(dynamic_pointer_cast<Boolean>(*iterator)->GetValue());

    ++iterator;
    EXPECT_NOT_NULL(*iterator);
    EXPECT_EQ(ValueType::Boolean, (*iterator)->Type());
    EXPECT_FALSE(dynamic_pointer_cast<Boolean>(*iterator)->GetValue());

    ++iterator;
    EXPECT_NOT_NULL(*iterator);
    EXPECT_EQ(ValueType::Null, (*iterator)->Type());
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
    EXPECT_NOT_NULL(*iterator);
    EXPECT_EQ(ValueType::Boolean, (*iterator)->Type());
    EXPECT_TRUE(dynamic_pointer_cast<Boolean>(*iterator)->GetValue());

    ++iterator;
    EXPECT_NOT_NULL(*iterator);
    EXPECT_EQ(ValueType::Boolean, (*iterator)->Type());
    EXPECT_FALSE(dynamic_pointer_cast<Boolean>(*iterator)->GetValue());

    ++iterator;
    EXPECT_NOT_NULL(*iterator);
    EXPECT_EQ(ValueType::Null, (*iterator)->Type());

    ++iterator;
    EXPECT_NOT_NULL(*iterator);
    EXPECT_EQ(ValueType::Number, (*iterator)->Type());
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

TEST_FIXTURE(ArrayTest, Serialize)
{
    ValuePtr value1 = std::make_shared<Boolean>(true);
    ValuePtr value2 = std::make_shared<Boolean>(false);
    ValuePtr value3 = std::make_shared<Null>();
    ValuePtr value4 = std::make_shared<Number>(_("12345678"));

    Array target(
    {
        value1,
        value2,
        value3,
        value4,
    });

    std::basic_ostringstream<OSAL::Char> stream;
    target.Serialize(stream);
    EXPECT_EQ(_("[\n")
              _("    true,\n")
              _("    false,\n")
              _("    null,\n")
              _("    12345678\n")
              _("]"), stream.str());
}

} // namespace Test
} // namespace JSON

