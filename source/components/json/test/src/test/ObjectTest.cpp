#include <unittest-cpp/UnitTestC++.h>

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

TEST_SUITE(json)
{

TEST_FIXTURE(ObjectTest, Construct)
{
    Object target;

    EXPECT_EQ(ValueType::Object, target.Type());
    EXPECT_EQ(size_t {0}, target.Size());
}

TEST_FIXTURE(ObjectTest, ConstructInitializerSinglePair)
{
    std::string key("key");
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
    std::string key1("key");
    ValuePtr value1 = std::make_shared<Boolean>(true);
    std::string key2("key2");
    ValuePtr value2 = std::make_shared<String>("Some text");

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
    std::string key1("key");
    ValuePtr value1 = std::make_shared<Boolean>(true);
    std::string key2("key2");
    ValuePtr value2 = std::make_shared<String>("Some text");
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
    std::string key1("key");
    ValuePtr value1 = std::make_shared<Boolean>(true);
    std::string key2("key2");
    ValuePtr value2 = std::make_shared<String>("Some text");
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

    std::string key("key");

    std::istringstream stream("{\"" + key + "\":true}");
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

    std::string key1("key1");
    std::string key2("key2");
    std::string key3("key3");
    std::string key4("key4");

    std::istringstream stream("{\n"
                                                "    \"" + key1 + "\" : true,\n"
                                                "    \"" + key2 + "\" : false,\n"
                                                "    \"" + key3 + "\" : null,\n"
                                                "    \"" + key4 + "\" : 12345678\n"
                                                "}");
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
    EXPECT_EQ("12345678", dynamic_pointer_cast<Number>((*iterator).GetValue())->GetValue());
}

TEST_FIXTURE(ObjectTest, DeserializeMultiplePairsForgottenComma)
{
    Object target;

    std::string key1("key1");
    std::string key2("key2");
    std::string key3("key3");
    std::string key4("key4");

    std::istringstream stream("{\n"
                                                "    \"" + key1 + "\" : true,\n"
                                                "    \"" + key2 + "\" : false,\n"
                                                "    \"" + key3 + "\" : null\n"
                                                "    \"" + key4 + "\" : 12345678\n"
                                                "}");
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

    std::string key1("key1");
    std::string key2("key2");
    std::string key3("key3");
    std::string key4("key4");

    std::istringstream stream("{\n"
                                                "    \"" + key1 + "\" : true,\n"
                                                "    \"" + key2 + "\" : false,\n"
                                                "    \"" + key3 + "\" : null,\n"
                                                "    \"" + key4 + "\" : 12345678,\n"
                                                "}");
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
    EXPECT_EQ("12345678", dynamic_pointer_cast<Number>((*iterator).GetValue())->GetValue());
}

TEST_FIXTURE(ObjectTest, DeserializeMultiplePairsIncompletePair)
{
    Object target;

    std::string key1("key1");
    std::string key2("key2");
    std::string key3("key3");
    std::string key4("key4");

    std::istringstream stream("{\n"
                                                    "    \"" + key1 + "\" : true,\n"
                                                    "    \"" + key2 + "\" : false,\n"
                                                    "    \"" + key3 + "\" :\n"
                                                    "    \"" + key4 + "\" : 12345678\n"
                                                    "}");
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

    std::istringstream stream("{}");
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(ValueType::Object, target.Type());
    ASSERT_EQ(size_t {0}, target.Size());
}

TEST_FIXTURE(ObjectTest, DeserializeNestedObject)
{
    Object target;

    std::istringstream stream("{ \"x\" : true, \"y\" : { \"z\" : 1234 }}");
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(ValueType::Object, target.Type());
    ASSERT_EQ(size_t {2}, target.Size());

    auto iterator = target.GetIterator();
    EXPECT_EQ("x", (*iterator).GetKey());
    ASSERT_NOT_NULL((*iterator).GetValue());
    ASSERT_EQ(ValueType::Boolean, (*iterator).GetValue()->Type());
    EXPECT_TRUE(dynamic_pointer_cast<Boolean>((*iterator).GetValue())->GetValue());

    ++iterator;
    EXPECT_EQ("y", (*iterator).GetKey());
    ASSERT_NOT_NULL((*iterator).GetValue());
    ASSERT_EQ(ValueType::Object, (*iterator).GetValue()->Type());

    auto object = dynamic_pointer_cast<Object>((*iterator).GetValue());
    ASSERT_NOT_NULL(object);
    ASSERT_EQ(size_t {1}, object->Size());

    auto iteratorNested = object->GetIterator();
    EXPECT_EQ("z", (*iteratorNested).GetKey());
    ASSERT_NOT_NULL((*iteratorNested).GetValue());
    ASSERT_EQ(ValueType::Number, (*iteratorNested).GetValue()->Type());
    EXPECT_EQ("1234", dynamic_pointer_cast<Number>((*iteratorNested).GetValue())->GetValue());
}

TEST_FIXTURE(ObjectTest, DeserializeNestedArray)
{
    Object target;

    std::istringstream stream("{ \"x\" : true, \"y\" : [ \"z\", 1234 ]}");
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(ValueType::Object, target.Type());
    ASSERT_EQ(size_t {2}, target.Size());

    auto iterator = target.GetIterator();
    EXPECT_EQ("x", (*iterator).GetKey());
    ASSERT_NOT_NULL((*iterator).GetValue());
    ASSERT_EQ(ValueType::Boolean, (*iterator).GetValue()->Type());
    EXPECT_TRUE(dynamic_pointer_cast<Boolean>((*iterator).GetValue())->GetValue());

    ++iterator;
    EXPECT_EQ("y", (*iterator).GetKey());
    ASSERT_NOT_NULL((*iterator).GetValue());
    ASSERT_EQ(ValueType::Array, (*iterator).GetValue()->Type());

    auto object = dynamic_pointer_cast<Array>((*iterator).GetValue());
    ASSERT_NOT_NULL(object);
    ASSERT_EQ(size_t {2}, object->Size());

    auto iteratorNested = object->GetIterator();
    ASSERT_EQ(ValueType::String, (*iteratorNested)->Type());
    EXPECT_EQ("z", dynamic_pointer_cast<String>(*iteratorNested)->GetValue());
    iteratorNested++;
    ASSERT_EQ(ValueType::Number, (*iteratorNested)->Type());
    EXPECT_EQ("1234", dynamic_pointer_cast<Number>(*iteratorNested)->GetValue());
}

TEST_FIXTURE(ObjectTest, Serialize)
{
    std::string key1("key1");
    std::string key2("key2");
    std::string key3("key3");
    std::string key4("key4");
    std::string key5("key5");
    std::string key6("key6");
    ValuePtr value1 = std::make_shared<Boolean>(true);
    ValuePtr value2 = std::make_shared<Boolean>(false);
    ValuePtr value3 = std::make_shared<Null>();
    ValuePtr value4 = std::make_shared<Number>("12345678");
    auto value5 = std::make_shared<Object>();
    value5->AddPair(KVPair("x", std::make_shared<Boolean>(true)));
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

    std::ostringstream stream;
    target.Serialize(stream);
    EXPECT_EQ("{\n"
              "    \"" + key1 + "\" : true,\n"
              "    \"" + key2 + "\" : false,\n"
              "    \"" + key3 + "\" : null,\n"
              "    \"" + key4 + "\" : 12345678,\n"
              "    \"" + key5 + "\" : {\n"
              "        \"x\" : true\n"
              "    },\n"
              "    \"" + key6 + "\" : [\n"
              "        true,\n"
              "        false\n"
              "    ]\n"
              "}", stream.str());
}

TEST_FIXTURE(ObjectTest, FindConst)
{
    std::string key1("key1");
    std::string key2("key2");
    std::string key3("key3");
    ValuePtr value1 = std::make_shared<Boolean>(true);
    ValuePtr value2 = std::make_shared<Boolean>(false);

    Object target(
        {
            { key1, value1 },
            { key2, value2 },
        });
    const Object constTarget = target;
    EXPECT_NOT_NULL(constTarget.Find(key1));
    EXPECT_NOT_NULL(constTarget.Find(key2));
    EXPECT_NULL(constTarget.Find(key3));
    // Careful, using dynamic_pointer_cast can easily cast away const-ness!
    auto valuePtr = constTarget.Find(key1);
    EXPECT_TRUE(std::dynamic_pointer_cast<Boolean>(valuePtr)->GetValue());
    std::dynamic_pointer_cast<Boolean>(valuePtr)->SetValue(false);
    EXPECT_FALSE(std::dynamic_pointer_cast<Boolean>(valuePtr)->GetValue());
}

TEST_FIXTURE(ObjectTest, FindNonConst)
{
    std::string key1("key1");
    std::string key2("key2");
    std::string key3("key3");
    ValuePtr value1 = std::make_shared<Boolean>(true);
    ValuePtr value2 = std::make_shared<Boolean>(false);

    Object target(
        {
            { key1, value1 },
            { key2, value2 },
        });
    EXPECT_NOT_NULL(target.Find(key1));
    EXPECT_NOT_NULL(target.Find(key2));
    EXPECT_NULL(target.Find(key3));
    auto valuePtr = target.Find(key1);
    EXPECT_TRUE(std::dynamic_pointer_cast<Boolean>(valuePtr)->GetValue());
    std::dynamic_pointer_cast<Boolean>(valuePtr)->SetValue(false);
    EXPECT_FALSE(std::dynamic_pointer_cast<Boolean>(valuePtr)->GetValue());
}

} // TEST_SUITE(json)

} // namespace Test
} // namespace JSON

