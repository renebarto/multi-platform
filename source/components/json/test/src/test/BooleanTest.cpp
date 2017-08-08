#include "unit-test-c++/UnitTestC++.h"

#include <string>
//#include "xml/TestData.h"
#include "json/Boolean.h"

using namespace std;

namespace JSON {
namespace Test {

class BooleanTest: public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void BooleanTest::SetUp()
{
}

void BooleanTest::TearDown()
{
}

TEST_FIXTURE(BooleanTest, Construct)
{
    Boolean target;

    EXPECT_EQ(ValueType::Boolean, target.Type());
    EXPECT_FALSE(target.GetValue());
}

TEST_FIXTURE(BooleanTest, ConstructInitializer)
{
    bool value = true;
    Boolean target(value);

    EXPECT_EQ(ValueType::Boolean, target.Type());
    EXPECT_EQ(value, target.GetValue());
}

TEST_FIXTURE(BooleanTest, Deserialize)
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
    EXPECT_FALSE(target.GetValue());
    stream.str(_(""));
    stream.clear();
    EXPECT_FALSE(target.Deserialize(stream));
    EXPECT_FALSE(target.GetValue());
}

TEST_FIXTURE(BooleanTest, Serialize)
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

} // namespace Test
} // namespace JSON

