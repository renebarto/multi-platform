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

class KVPairTest: public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void KVPairTest::SetUp()
{
}

void KVPairTest::TearDown()
{
}

TEST_SUITE(json)
{

TEST_FIXTURE(KVPairTest, Construct)
{
    KVPair target;

    EXPECT_EQ(_(""), target.GetKey());
    EXPECT_NULL(target.GetValue());
}

TEST_FIXTURE(KVPairTest, ConstructInitializer)
{
    OSAL::String key(_("key"));
    ValuePtr value = std::make_shared<Boolean>(true);
    KVPair target(key, value);

    EXPECT_EQ(key, target.GetKey());
    ASSERT_NOT_NULL(target.GetValue());
    ASSERT_EQ(ValueType::Boolean, target.GetValue()->Type());
    EXPECT_TRUE(dynamic_pointer_cast<Boolean>(target.GetValue())->GetValue());
}

TEST_FIXTURE(KVPairTest, DeserializeKeyBoolean)
{
    KVPair target;

    OSAL::String key(_("key"));
    std::basic_istringstream<OSAL::Char> stream(_("\"") + key + _("\":true"));
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(key, target.GetKey());
    ASSERT_NOT_NULL(target.GetValue());
    ASSERT_EQ(ValueType::Boolean, target.GetValue()->Type());
    EXPECT_TRUE(dynamic_pointer_cast<Boolean>(target.GetValue())->GetValue());
}

TEST_FIXTURE(KVPairTest, DeserializeKeyNull)
{
    KVPair target;

    OSAL::String key(_("key"));
    std::basic_istringstream<OSAL::Char> stream(_("\"") + key + _("\":null"));
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(key, target.GetKey());
    ASSERT_NOT_NULL(target.GetValue());
    ASSERT_EQ(ValueType::Null, target.GetValue()->Type());
}

TEST_FIXTURE(KVPairTest, DeserializeKeyNumber)
{
    KVPair target;

    OSAL::String key(_("key"));
    std::basic_istringstream<OSAL::Char> stream(_("\"") + key + _("\":123"));
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(key, target.GetKey());
    ASSERT_NOT_NULL(target.GetValue());
    ASSERT_EQ(ValueType::Number, target.GetValue()->Type());
    EXPECT_EQ(_("123"), dynamic_pointer_cast<Number>(target.GetValue())->GetValue());
}

TEST_FIXTURE(KVPairTest, DeserializeKeyString)
{
    KVPair target;

    OSAL::String key(_("key"));
    std::basic_istringstream<OSAL::Char> stream(_("\"") + key + _("\" : \"Some text\""));
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(key, target.GetKey());
    ASSERT_NOT_NULL(target.GetValue());
    ASSERT_EQ(ValueType::String, target.GetValue()->Type());
    EXPECT_EQ(_("Some text"), dynamic_pointer_cast<String>(target.GetValue())->GetValue());
}

TEST_FIXTURE(KVPairTest, DeserializeKeyObject)
{
    KVPair target;

    std::basic_istringstream<OSAL::Char> stream(_("\"y\" : { \"z\" : 1234 }"));
    EXPECT_TRUE(target.Deserialize(stream));

    EXPECT_EQ(_("y"), target.GetKey());
    ASSERT_NOT_NULL(target.GetValue());
    ASSERT_EQ(ValueType::Object, target.GetValue()->Type());

    auto object = dynamic_pointer_cast<Object>(target.GetValue());
    ASSERT_NOT_NULL(object);
    ASSERT_EQ(size_t {1}, object->Size());

    auto iterator = object->GetIterator();
    EXPECT_EQ(_("z"), (*iterator).GetKey());
    ASSERT_NOT_NULL((*iterator).GetValue());
    ASSERT_EQ(ValueType::Number, (*iterator).GetValue()->Type());
    EXPECT_EQ(_("1234"), dynamic_pointer_cast<Number>((*iterator).GetValue())->GetValue());
}

TEST_FIXTURE(KVPairTest, DeserializeKeyArray)
{
    KVPair target;

    std::basic_istringstream<OSAL::Char> stream(_("\"y\" : [ \"z\", 1234 ]"));
    EXPECT_TRUE(target.Deserialize(stream));

    EXPECT_EQ(_("y"), target.GetKey());
    ASSERT_NOT_NULL(target.GetValue());
    ASSERT_EQ(ValueType::Array, target.GetValue()->Type());

    auto object = dynamic_pointer_cast<Array>(target.GetValue());
    ASSERT_NOT_NULL(object);
    ASSERT_EQ(size_t {2}, object->Size());

    auto iterator = object->GetIterator();
    ASSERT_EQ(ValueType::String, (*iterator)->Type());
    EXPECT_EQ(_("z"), dynamic_pointer_cast<String>(*iterator)->GetValue());
    iterator++;
    ASSERT_EQ(ValueType::Number, (*iterator)->Type());
    EXPECT_EQ(_("1234"), dynamic_pointer_cast<Number>(*iterator)->GetValue());
}

TEST_FIXTURE(KVPairTest, DeserializeKeyOnly)
{
    KVPair target;

    OSAL::String key(_("key"));
    std::basic_istringstream<OSAL::Char> stream(_("\"") + key + _("\":"));
    EXPECT_FALSE(target.Deserialize(stream));
    EXPECT_EQ(_(""), target.GetKey());
    EXPECT_NULL(target.GetValue());
}

TEST_FIXTURE(KVPairTest, DeserializeUnquotedKey)
{
    KVPair target;

    OSAL::String key(_("key"));
    std::basic_istringstream<OSAL::Char> stream(key + _(":false"));
    EXPECT_FALSE(target.Deserialize(stream));
    EXPECT_EQ(_(""), target.GetKey());
    EXPECT_NULL(target.GetValue());
}

TEST_FIXTURE(KVPairTest, DeserializeEmpty)
{
    KVPair target;

    OSAL::String key(_("key"));
    std::basic_istringstream<OSAL::Char> stream(_(""));
    EXPECT_FALSE(target.Deserialize(stream));
    EXPECT_EQ(_(""), target.GetKey());
    EXPECT_NULL(target.GetValue());
}

TEST_FIXTURE(KVPairTest, SerializeKeyBoolean)
{
    OSAL::String key(_("key"));
    ValuePtr value = std::make_shared<Boolean>(true);
    KVPair target(key, value);

    std::basic_ostringstream<OSAL::Char> stream;
    target.Serialize(stream);
    EXPECT_EQ(_("\"") + key + _("\" : true"), stream.str());
}

TEST_FIXTURE(KVPairTest, SerializeKeyNumber)
{
    OSAL::String key(_("key"));
    std::shared_ptr<Number> value = std::make_shared<Number>();
    value->SetValue(12345678);

    KVPair target(key, value);

    std::basic_ostringstream<OSAL::Char> stream;
    target.Serialize(stream);
    EXPECT_EQ(_("\"") + key + _("\" : 12345678"), stream.str());
}

TEST_FIXTURE(KVPairTest, SerializeKeyString)
{
    OSAL::String key(_("key"));
    ValuePtr value = std::make_shared<String>(_("Some text"));
    KVPair target(key, value);

    std::basic_ostringstream<OSAL::Char> stream;
    target.Serialize(stream);
    EXPECT_EQ(_("\"") + key + _("\" : \"Some text\""), stream.str());
}

TEST_FIXTURE(KVPairTest, SerializeKeyNoValue)
{
    OSAL::String key(_("key"));
    ValuePtr value = nullptr;
    KVPair target(key, value);

    std::basic_ostringstream<OSAL::Char> stream;
    target.Serialize(stream);
    EXPECT_EQ(_("\"") + key + _("\" : null"), stream.str());
}

TEST_FIXTURE(KVPairTest, SerializeObject)
{
    OSAL::String key(_("key"));
    std::shared_ptr<Object> value = std::make_shared<Object>();
    OSAL::String keySub(_("keySub"));
    ValuePtr valueSub = std::make_shared<Boolean>(true);
    KVPair targetSub(keySub, valueSub);
    value->AddPair(targetSub);
    KVPair target(key, value);

    std::basic_ostringstream<OSAL::Char> stream;
    target.Serialize(stream);
    EXPECT_EQ(_("\"") + key + _("\" : {\n    \"") + keySub + _("\" : true\n") + _("}"), stream.str());
}

TEST_FIXTURE(KVPairTest, SerializeArray)
{
    OSAL::String key(_("key"));
    std::shared_ptr<Array> value = std::make_shared<Array>();
    ValuePtr valueSub = std::make_shared<Boolean>(true);
    value->AddValue(valueSub);
    KVPair target(key, value);

    std::basic_ostringstream<OSAL::Char> stream;
    target.Serialize(stream);
    EXPECT_EQ(_("\"") + key + _("\" : [\n    true\n") + _("]"), stream.str());
}

} // TEST_SUITE(json)

} // namespace Test
} // namespace JSON

