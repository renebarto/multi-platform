#include "unit-test-c++/UnitTestC++.h"

#include <string>
//#include "xml/TestData.h"
#include "json/Boolean.h"
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
    OSAL::String key("key");
    ValuePtr value = std::make_shared<Boolean>(true);
    KVPair target(key, value);

    EXPECT_EQ(key, target.GetKey());
    EXPECT_NOT_NULL(target.GetValue());
    EXPECT_EQ(ValueType::Boolean, target.GetValue()->Type());
    EXPECT_TRUE(dynamic_pointer_cast<Boolean>(target.GetValue())->GetValue());
}

TEST_FIXTURE(KVPairTest, Deserialize)
{
//    KVPair target;
//
//    EXPECT_EQ(ValueType::KVPair, target.Type());
//    EXPECT_EQ(_(""), target.GetValue());
//    std::basic_istringstream<OSAL::Char> stream(_("\"\""));
//    EXPECT_TRUE(target.Deserialize(stream));
//    EXPECT_EQ(_(""), target.GetValue());
//    stream.str(_("\"Some text\""));
//    stream.clear();
//    EXPECT_TRUE(target.Deserialize(stream));
//    EXPECT_EQ(_("Some text"), target.GetValue());
//    stream.str(_("garbage"));
//    stream.clear();
//    EXPECT_FALSE(target.Deserialize(stream));
//    EXPECT_EQ(_(""), target.GetValue());
//    stream.str(_(""));
//    stream.clear();
//    EXPECT_FALSE(target.Deserialize(stream));
//    EXPECT_EQ(_(""), target.GetValue());
}

TEST_FIXTURE(KVPairTest, Serialize)
{
//    KVPair target;
//
//    EXPECT_EQ(ValueType::KVPair, target.Type());
//    EXPECT_EQ(_(""), target.GetValue());
//    std::basic_ostringstream<OSAL::Char> stream;
//    target.Serialize(stream);
//    EXPECT_EQ(_("\"\""), stream.str());
//    stream.str(_(""));
//    target.SetValue(_("Some text"));
//    target.Serialize(stream);
//    EXPECT_EQ(_("\"Some text\""), stream.str());
}

} // namespace Test
} // namespace JSON

