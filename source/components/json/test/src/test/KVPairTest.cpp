#include "unit-test-c++/UnitTestC++.h"

#include <string>
//#include "xml/TestData.h"
#include "json/Boolean.h"
#include "json/Null.h"
#include "json/Number.h"
#include "json/String.h"
#include "json/Object.h"

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
    EXPECT_NOT_NULL(target.GetValue());
    EXPECT_EQ(ValueType::Boolean, target.GetValue()->Type());
    EXPECT_TRUE(dynamic_pointer_cast<Boolean>(target.GetValue())->GetValue());
}

TEST_FIXTURE(KVPairTest, Deserialize)
{
    KVPair target;

    OSAL::String key(_("key"));
    EXPECT_EQ(_(""), target.GetKey());
    EXPECT_NULL(target.GetValue());
    std::basic_istringstream<OSAL::Char> stream(key + _(":true"));
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(key, target.GetKey());
    EXPECT_NOT_NULL(target.GetValue());
    EXPECT_EQ(ValueType::Boolean, target.GetValue()->Type());
    EXPECT_TRUE(dynamic_pointer_cast<Boolean>(target.GetValue())->GetValue());

    stream.str(key + _(":null"));
    stream.clear();
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(key, target.GetKey());
    EXPECT_NOT_NULL(target.GetValue());
    EXPECT_EQ(ValueType::Null, target.GetValue()->Type());

    stream.str(key + _(":123"));
    stream.clear();
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(key, target.GetKey());
    EXPECT_NOT_NULL(target.GetValue());
    EXPECT_EQ(ValueType::Number, target.GetValue()->Type());
    EXPECT_EQ(_("123"), dynamic_pointer_cast<Number>(target.GetValue())->GetValue());

    stream.str(key + _(":\"Some text\""));
    stream.clear();
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(key, target.GetKey());
    EXPECT_NOT_NULL(target.GetValue());
    EXPECT_EQ(ValueType::String, target.GetValue()->Type());
    EXPECT_EQ(_("Some text"), dynamic_pointer_cast<String>(target.GetValue())->GetValue());

    stream.str(key + _(":"));
    stream.clear();
    EXPECT_FALSE(target.Deserialize(stream));
    EXPECT_EQ(_(""), target.GetKey());
    EXPECT_NULL(target.GetValue());

    stream.str(_(""));
    stream.clear();
    EXPECT_FALSE(target.Deserialize(stream));
    EXPECT_EQ(_(""), target.GetKey());
    EXPECT_NULL(target.GetValue());
}

TEST_FIXTURE(KVPairTest, Serialize)
{
    OSAL::String key(_("key"));
    ValuePtr value = std::make_shared<Boolean>(true);
    KVPair target(key, value);

    std::basic_ostringstream<OSAL::Char> stream;
    target.Serialize(stream);
    EXPECT_EQ(key + _(" : true"), stream.str());

    stream.str(_(""));
    key = _("key2");
    value = std::make_shared<String>(_("Some text"));
    target.SetKey(key);
    target.SetValue(value);
    target.Serialize(stream);

    EXPECT_EQ(key + _(" : \"Some text\""), stream.str());

    stream.str(_(""));
    target.SetValue(nullptr);
    target.Serialize(stream);
    EXPECT_EQ(key + _(" : null"), stream.str());
}

} // namespace Test
} // namespace JSON

