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

class ObjectTest: public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void ObjectTest::SetUp()
{
}

void ObjectTest::TearDown()
{
}

TEST_FIXTURE(ObjectTest, Construct)
{
    Object target;

    EXPECT_EQ(ValueType::Object, target.Type());
    EXPECT_EQ(size_t {0}, target.Size());
}

TEST_FIXTURE(ObjectTest, ConstructInitializerSinglePair)
{
    OSAL::String key(_("key"));
    ValuePtr value = std::make_shared<Boolean>(true);

    KVPair pair(key, value);
    Object target({pair});

    EXPECT_EQ(ValueType::Object, target.Type());
    ASSERT_EQ(size_t {1}, target.Size());
    auto iterator = target.GetIterator();
    EXPECT_EQ(key, (*iterator).GetKey());
    EXPECT_EQ(value, (*iterator).GetValue());
}

TEST_FIXTURE(ObjectTest, ConstructInitializerMultiplePairs)
{
    OSAL::String key1(_("key"));
    ValuePtr value1 = std::make_shared<Boolean>(true);
    OSAL::String key2(_("key2"));
    ValuePtr value2 = std::make_shared<String>(_("Some text"));

    Object target({{key1, value1}, {key2, value2}});

    EXPECT_EQ(ValueType::Object, target.Type());
    ASSERT_EQ(size_t {2}, target.Size());
    auto iterator = target.GetIterator();
    EXPECT_EQ(key1, (*iterator).GetKey());
    EXPECT_EQ(value1, (*iterator).GetValue());
    ++iterator;
    EXPECT_EQ(key2, (*iterator).GetKey());
    EXPECT_EQ(value2, (*iterator).GetValue());
}

TEST_FIXTURE(ObjectTest, AddPairDifferentKeys)
{
    Object target;
    OSAL::String key1(_("key"));
    ValuePtr value1 = std::make_shared<Boolean>(true);
    OSAL::String key2(_("key2"));
    ValuePtr value2 = std::make_shared<String>(_("Some text"));
    ASSERT_EQ(size_t {0}, target.Size());
    EXPECT_TRUE(target.AddPair({key1, value1}));
    ASSERT_EQ(size_t {1}, target.Size());
    EXPECT_TRUE(target.AddPair({key2, value2}));
    ASSERT_EQ(size_t {2}, target.Size());
    auto iterator = target.GetIterator();
    EXPECT_EQ(key1, (*iterator).GetKey());
    EXPECT_EQ(value1, (*iterator).GetValue());
    ++iterator;
    EXPECT_EQ(key2, (*iterator).GetKey());
    EXPECT_EQ(value2, (*iterator).GetValue());
}

TEST_FIXTURE(ObjectTest, AddPairDuplicateKeys)
{
    Object target;
    OSAL::String key1(_("key"));
    ValuePtr value1 = std::make_shared<Boolean>(true);
    OSAL::String key2(_("key2"));
    ValuePtr value2 = std::make_shared<String>(_("Some text"));
    ASSERT_EQ(size_t {0}, target.Size());
    EXPECT_TRUE(target.AddPair({key1, value1}));
    ASSERT_EQ(size_t {1}, target.Size());
    EXPECT_TRUE(target.AddPair({key2, value2}));
    ASSERT_EQ(size_t {2}, target.Size());
    EXPECT_FALSE(target.AddPair({key2, value1}));
    ASSERT_EQ(size_t {2}, target.Size());
    auto iterator = target.GetIterator();
    EXPECT_EQ(key1, (*iterator).GetKey());
    EXPECT_EQ(value1, (*iterator).GetValue());
    ++iterator;
    EXPECT_EQ(key2, (*iterator).GetKey());
    EXPECT_EQ(value2, (*iterator).GetValue());
}

TEST_FIXTURE(ObjectTest, DeserializeSinglePair)
{
    Object target;

    OSAL::String key(_("key"));

    std::basic_istringstream<OSAL::Char> stream(_("{\"") + key + _("\":true}"));
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(ValueType::Object, target.Type());
    ASSERT_EQ(size_t {1}, target.Size());
    auto iterator = target.GetIterator();
    EXPECT_EQ(key, (*iterator).GetKey());
    ASSERT_NOT_NULL((*iterator).GetValue());
    ASSERT_EQ(ValueType::Boolean, (*iterator).GetValue()->Type());
    EXPECT_TRUE(dynamic_pointer_cast<Boolean>((*iterator).GetValue())->GetValue());
}

TEST_FIXTURE(ObjectTest, DeserializeMultiplePairs)
{
    Object target;

    OSAL::String key1(_("key1"));
    OSAL::String key2(_("key2"));
    OSAL::String key3(_("key3"));
    OSAL::String key4(_("key4"));

    std::basic_istringstream<OSAL::Char> stream(_("{\n")
                                                _("    \"") + key1 + _("\" : true,\n")
                                                _("    \"") + key2 + _("\" : false,\n")
                                                _("    \"") + key3 + _("\" : null,\n")
                                                _("    \"") + key4 + _("\" : 12345678\n")
                                                _("}"));
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(ValueType::Object, target.Type());
    ASSERT_EQ(size_t {4}, target.Size());

    auto iterator = target.GetIterator();
    EXPECT_EQ(key1, (*iterator).GetKey());
    ASSERT_NOT_NULL((*iterator).GetValue());
    ASSERT_EQ(ValueType::Boolean, (*iterator).GetValue()->Type());
    EXPECT_TRUE(dynamic_pointer_cast<Boolean>((*iterator).GetValue())->GetValue());

    ++iterator;
    EXPECT_EQ(key2, (*iterator).GetKey());
    ASSERT_NOT_NULL((*iterator).GetValue());
    ASSERT_EQ(ValueType::Boolean, (*iterator).GetValue()->Type());
    EXPECT_FALSE(dynamic_pointer_cast<Boolean>((*iterator).GetValue())->GetValue());

    ++iterator;
    EXPECT_EQ(key3, (*iterator).GetKey());
    ASSERT_NOT_NULL((*iterator).GetValue());
    ASSERT_EQ(ValueType::Null, (*iterator).GetValue()->Type());

    ++iterator;
    EXPECT_EQ(key4, (*iterator).GetKey());
    ASSERT_NOT_NULL((*iterator).GetValue());
    ASSERT_EQ(ValueType::Number, (*iterator).GetValue()->Type());
    EXPECT_EQ(_("12345678"), dynamic_pointer_cast<Number>((*iterator).GetValue())->GetValue());
}

TEST_FIXTURE(ObjectTest, DeserializeMultiplePairsForgottenComma)
{
    Object target;

    OSAL::String key1(_("key1"));
    OSAL::String key2(_("key2"));
    OSAL::String key3(_("key3"));
    OSAL::String key4(_("key4"));

    std::basic_istringstream<OSAL::Char> stream(_("{\n")
                                                _("    \"") + key1 + _("\" : true,\n")
                                                _("    \"") + key2 + _("\" : false,\n")
                                                _("    \"") + key3 + _("\" : null\n")
                                                _("    \"") + key4 + _("\" : 12345678\n")
                                                _("}"));
    EXPECT_FALSE(target.Deserialize(stream));
    EXPECT_EQ(ValueType::Object, target.Type());
    ASSERT_EQ(size_t {3}, target.Size());

    auto iterator = target.GetIterator();
    EXPECT_EQ(key1, (*iterator).GetKey());
    ASSERT_NOT_NULL((*iterator).GetValue());
    ASSERT_EQ(ValueType::Boolean, (*iterator).GetValue()->Type());
    EXPECT_TRUE(dynamic_pointer_cast<Boolean>((*iterator).GetValue())->GetValue());

    ++iterator;
    EXPECT_EQ(key2, (*iterator).GetKey());
    ASSERT_NOT_NULL((*iterator).GetValue());
    ASSERT_EQ(ValueType::Boolean, (*iterator).GetValue()->Type());
    EXPECT_FALSE(dynamic_pointer_cast<Boolean>((*iterator).GetValue())->GetValue());

    ++iterator;
    EXPECT_EQ(key3, (*iterator).GetKey());
    ASSERT_NOT_NULL((*iterator).GetValue());
    ASSERT_EQ(ValueType::Null, (*iterator).GetValue()->Type());
}

TEST_FIXTURE(ObjectTest, DeserializeMultiplePairsExtraComma)
{
    Object target;

    OSAL::String key1(_("key1"));
    OSAL::String key2(_("key2"));
    OSAL::String key3(_("key3"));
    OSAL::String key4(_("key4"));

    std::basic_istringstream<OSAL::Char> stream(_("{\n")
                                                _("    \"") + key1 + _("\" : true,\n")
                                                _("    \"") + key2 + _("\" : false,\n")
                                                _("    \"") + key3 + _("\" : null,\n")
                                                _("    \"") + key4 + _("\" : 12345678,\n")
                                                _("}"));
    EXPECT_FALSE(target.Deserialize(stream));
    EXPECT_EQ(ValueType::Object, target.Type());
    ASSERT_EQ(size_t {4}, target.Size());

    auto iterator = target.GetIterator();
    EXPECT_EQ(key1, (*iterator).GetKey());
    ASSERT_NOT_NULL((*iterator).GetValue());
    ASSERT_EQ(ValueType::Boolean, (*iterator).GetValue()->Type());
    EXPECT_TRUE(dynamic_pointer_cast<Boolean>((*iterator).GetValue())->GetValue());

    ++iterator;
    EXPECT_EQ(key2, (*iterator).GetKey());
    ASSERT_NOT_NULL((*iterator).GetValue());
    ASSERT_EQ(ValueType::Boolean, (*iterator).GetValue()->Type());
    EXPECT_FALSE(dynamic_pointer_cast<Boolean>((*iterator).GetValue())->GetValue());

    ++iterator;
    EXPECT_EQ(key3, (*iterator).GetKey());
    ASSERT_NOT_NULL((*iterator).GetValue());
    ASSERT_EQ(ValueType::Null, (*iterator).GetValue()->Type());

    ++iterator;
    EXPECT_EQ(key4, (*iterator).GetKey());
    ASSERT_NOT_NULL((*iterator).GetValue());
    ASSERT_EQ(ValueType::Number, (*iterator).GetValue()->Type());
    EXPECT_EQ(_("12345678"), dynamic_pointer_cast<Number>((*iterator).GetValue())->GetValue());
}

TEST_FIXTURE(ObjectTest, DeserializeMultiplePairsIncompletePair)
{
    Object target;

    OSAL::String key1(_("key1"));
    OSAL::String key2(_("key2"));
    OSAL::String key3(_("key3"));
    OSAL::String key4(_("key4"));

    std::basic_istringstream<OSAL::Char> stream(_("{\n")
                                                    _("    \"") + key1 + _("\" : true,\n")
                                                    _("    \"") + key2 + _("\" : false,\n")
                                                    _("    \"") + key3 + _("\" :\n")
                                                    _("    \"") + key4 + _("\" : 12345678\n")
                                                    _("}"));
    EXPECT_FALSE(target.Deserialize(stream));
    EXPECT_EQ(ValueType::Object, target.Type());
    ASSERT_EQ(size_t {3}, target.Size());

    auto iterator = target.GetIterator();
    EXPECT_EQ(key1, (*iterator).GetKey());
    ASSERT_NOT_NULL((*iterator).GetValue());
    ASSERT_EQ(ValueType::Boolean, (*iterator).GetValue()->Type());
    EXPECT_TRUE(dynamic_pointer_cast<Boolean>((*iterator).GetValue())->GetValue());

    ++iterator;
    EXPECT_EQ(key2, (*iterator).GetKey());
    ASSERT_NOT_NULL((*iterator).GetValue());
    ASSERT_EQ(ValueType::Boolean, (*iterator).GetValue()->Type());
    EXPECT_FALSE(dynamic_pointer_cast<Boolean>((*iterator).GetValue())->GetValue());

    ++iterator;
    EXPECT_EQ(key3, (*iterator).GetKey());
    ASSERT_NOT_NULL((*iterator).GetValue());
    ASSERT_EQ(ValueType::String, (*iterator).GetValue()->Type());
    EXPECT_EQ(key4, dynamic_pointer_cast<String>((*iterator).GetValue())->GetValue());
}

TEST_FIXTURE(ObjectTest, DeserializeEmptyObject)
{
    Object target;

    std::basic_istringstream<OSAL::Char> stream(_("{}"));
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(ValueType::Object, target.Type());
    ASSERT_EQ(size_t {0}, target.Size());
}

TEST_FIXTURE(ObjectTest, DeserializeNestedObject)
{
    Object target;

    std::basic_istringstream<OSAL::Char> stream(_("{ \"x\" : true, \"y\" : { \"z\" : 1234 }}"));
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(ValueType::Object, target.Type());
    ASSERT_EQ(size_t {2}, target.Size());

    auto iterator = target.GetIterator();
    EXPECT_EQ(_("x"), (*iterator).GetKey());
    ASSERT_NOT_NULL((*iterator).GetValue());
    ASSERT_EQ(ValueType::Boolean, (*iterator).GetValue()->Type());
    EXPECT_TRUE(dynamic_pointer_cast<Boolean>((*iterator).GetValue())->GetValue());

    ++iterator;
    EXPECT_EQ(_("y"), (*iterator).GetKey());
    ASSERT_NOT_NULL((*iterator).GetValue());
    ASSERT_EQ(ValueType::Object, (*iterator).GetValue()->Type());

    auto object = dynamic_pointer_cast<Object>((*iterator).GetValue());
    ASSERT_NOT_NULL(object);
    ASSERT_EQ(size_t {1}, object->Size());

    auto iteratorNested = object->GetIterator();
    EXPECT_EQ(_("z"), (*iteratorNested).GetKey());
    ASSERT_NOT_NULL((*iteratorNested).GetValue());
    ASSERT_EQ(ValueType::Number, (*iteratorNested).GetValue()->Type());
    EXPECT_EQ(_("1234"), dynamic_pointer_cast<Number>((*iteratorNested).GetValue())->GetValue());
}

TEST_FIXTURE(ObjectTest, DeserializeNestedArray)
{
    Object target;

    std::basic_istringstream<OSAL::Char> stream(_("{ \"x\" : true, \"y\" : [ \"z\", 1234 ]}"));
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(ValueType::Object, target.Type());
    ASSERT_EQ(size_t {2}, target.Size());

    auto iterator = target.GetIterator();
    EXPECT_EQ(_("x"), (*iterator).GetKey());
    ASSERT_NOT_NULL((*iterator).GetValue());
    ASSERT_EQ(ValueType::Boolean, (*iterator).GetValue()->Type());
    EXPECT_TRUE(dynamic_pointer_cast<Boolean>((*iterator).GetValue())->GetValue());

    ++iterator;
    EXPECT_EQ(_("y"), (*iterator).GetKey());
    ASSERT_NOT_NULL((*iterator).GetValue());
    ASSERT_EQ(ValueType::Array, (*iterator).GetValue()->Type());

    auto object = dynamic_pointer_cast<Array>((*iterator).GetValue());
    ASSERT_NOT_NULL(object);
    ASSERT_EQ(size_t {2}, object->Size());

    auto iteratorNested = object->GetIterator();
    ASSERT_EQ(ValueType::String, (*iteratorNested)->Type());
    EXPECT_EQ(_("z"), dynamic_pointer_cast<String>(*iteratorNested)->GetValue());
    iteratorNested++;
    ASSERT_EQ(ValueType::Number, (*iteratorNested)->Type());
    EXPECT_EQ(_("1234"), dynamic_pointer_cast<Number>(*iteratorNested)->GetValue());
}

TEST_FIXTURE(ObjectTest, Serialize)
{
    OSAL::String key1(_("key1"));
    OSAL::String key2(_("key2"));
    OSAL::String key3(_("key3"));
    OSAL::String key4(_("key4"));
    OSAL::String key5(_("key5"));
    OSAL::String key6(_("key6"));
    ValuePtr value1 = std::make_shared<Boolean>(true);
    ValuePtr value2 = std::make_shared<Boolean>(false);
    ValuePtr value3 = std::make_shared<Null>();
    ValuePtr value4 = std::make_shared<Number>(_("12345678"));
    auto value5 = std::make_shared<Object>();
    value5->AddPair(KVPair(_("x"), std::make_shared<Boolean>(true)));
    auto value6 = std::make_shared<Array>();
    value6->AddValue(std::make_shared<Boolean>(true));
    value6->AddValue(std::make_shared<Boolean>(false));

    Object target(
    {
        { key1, value1 },
        { key2, value2 },
        { key3, value3 },
        { key4, value4 },
        { key5, value5 },
        { key6, value6 },
    });

    std::basic_ostringstream<OSAL::Char> stream;
    target.Serialize(stream);
    EXPECT_EQ(_("{\n")
              _("    \"") + key1 + _("\" : true,\n")
              _("    \"") + key2 + _("\" : false,\n")
              _("    \"") + key3 + _("\" : null,\n")
              _("    \"") + key4 + _("\" : 12345678,\n")
              _("    \"") + key5 + _("\" : {\n")
              _("        \"x\" : true\n")
              _("    },\n")
              _("    \"") + key6 + _("\" : [\n")
              _("        true,\n")
              _("        false\n")
              _("    ]\n")
              _("}"), stream.str());
}

} // namespace Test
} // namespace JSON

