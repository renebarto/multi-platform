#include "unit-test-c++/UnitTestC++.h"

#include <string>
//#include "xml/TestData.h"
#include "json/Number.h"

using namespace std;

namespace JSON {
namespace Test {

class NumberTest: public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void NumberTest::SetUp()
{
}

void NumberTest::TearDown()
{
}

TEST_FIXTURE(NumberTest, Construct)
{
    Number target;

    EXPECT_EQ(ValueType::Number, target.Type());
    EXPECT_EQ(_(""), target.GetValue());
    int8_t int8Value;
    EXPECT_FALSE(target.GetValue(int8Value));
    uint8_t uint8Value;
    EXPECT_FALSE(target.GetValue(uint8Value));
    int16_t int16Value;
    EXPECT_FALSE(target.GetValue(int16Value));
    uint16_t uint16Value;
    EXPECT_FALSE(target.GetValue(uint16Value));
    int32_t int32Value;
    EXPECT_FALSE(target.GetValue(int32Value));
    uint32_t uint32Value;
    EXPECT_FALSE(target.GetValue(uint32Value));
    int64_t int64Value;
    EXPECT_FALSE(target.GetValue(int64Value));
    uint64_t uint64Value;
    EXPECT_FALSE(target.GetValue(uint64Value));
    float floatValue;
    EXPECT_FALSE(target.GetValue(floatValue));
    double doubleValue;
    EXPECT_FALSE(target.GetValue(doubleValue));
    long double longdoubleValue;
    EXPECT_FALSE(target.GetValue(longdoubleValue));
}

TEST_FIXTURE(NumberTest, ConstructInitializer)
{
    OSAL::String value("123");
    Number target(value);

    EXPECT_EQ(ValueType::Number, target.Type());
    EXPECT_EQ(value, target.GetValue());
    int8_t int8Value;
    EXPECT_TRUE(target.GetValue(int8Value));
    EXPECT_EQ(123, int8Value);
    uint8_t uint8Value;
    EXPECT_TRUE(target.GetValue(uint8Value));
    EXPECT_EQ(123, uint8Value);
    int16_t int16Value;
    EXPECT_TRUE(target.GetValue(int16Value));
    EXPECT_EQ(123, int16Value);
    uint16_t uint16Value;
    EXPECT_TRUE(target.GetValue(uint16Value));
    EXPECT_EQ(123, uint16Value);
    int32_t int32Value;
    EXPECT_TRUE(target.GetValue(int32Value));
    EXPECT_EQ(123, int32Value);
    uint32_t uint32Value;
    EXPECT_TRUE(target.GetValue(uint32Value));
    EXPECT_EQ(123U, uint32Value);
    int64_t int64Value;
    EXPECT_TRUE(target.GetValue(int64Value));
    EXPECT_EQ(123, int64Value);
    uint64_t uint64Value;
    EXPECT_TRUE(target.GetValue(uint64Value));
    EXPECT_EQ(123U, uint64Value);
    float floatValue;
    EXPECT_TRUE(target.GetValue(floatValue));
    EXPECT_EQ(123, floatValue);
    double doubleValue;
    EXPECT_TRUE(target.GetValue(doubleValue));
    EXPECT_EQ(123, doubleValue);
    long double longdoubleValue;
    EXPECT_TRUE(target.GetValue(longdoubleValue));
    EXPECT_EQ(123, longdoubleValue);
}

TEST_FIXTURE(NumberTest, DeserializeInteger)
{
    Number target;

    EXPECT_EQ(ValueType::Number, target.Type());
    EXPECT_EQ(_(""), target.GetValue());
    std::basic_istringstream<OSAL::Char> stream(_("123"));
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(_("123"), target.GetValue());
    int8_t int8Value;
    EXPECT_TRUE(target.GetValue(int8Value));
    EXPECT_EQ(123, int8Value);
    uint8_t uint8Value;
    EXPECT_TRUE(target.GetValue(uint8Value));
    EXPECT_EQ(123, uint8Value);
    int16_t int16Value;
    EXPECT_TRUE(target.GetValue(int16Value));
    EXPECT_EQ(123, int16Value);
    uint16_t uint16Value;
    EXPECT_TRUE(target.GetValue(uint16Value));
    EXPECT_EQ(123, uint16Value);
    int32_t int32Value;
    EXPECT_TRUE(target.GetValue(int32Value));
    EXPECT_EQ(123, int32Value);
    uint32_t uint32Value;
    EXPECT_TRUE(target.GetValue(uint32Value));
    EXPECT_EQ(123u, uint32Value);
    int64_t int64Value;
    EXPECT_TRUE(target.GetValue(int64Value));
    EXPECT_EQ(123, int64Value);
    uint64_t uint64Value;
    EXPECT_TRUE(target.GetValue(uint64Value));
    EXPECT_EQ(123u, uint64Value);
    float floatValue;
    EXPECT_TRUE(target.GetValue(floatValue));
    EXPECT_EQ(123, floatValue);
    double doubleValue;
    EXPECT_TRUE(target.GetValue(doubleValue));
    EXPECT_EQ(123, doubleValue);
    long double longdoubleValue;
    EXPECT_TRUE(target.GetValue(longdoubleValue));
    EXPECT_EQ(123, longdoubleValue);

    stream.str(_("-123"));
    stream.clear();
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(_("-123"), target.GetValue());
    EXPECT_TRUE(target.GetValue(int8Value));
    EXPECT_EQ(-123, int8Value);
    EXPECT_FALSE(target.GetValue(uint8Value));
    EXPECT_EQ(0, uint8Value);
    EXPECT_TRUE(target.GetValue(int16Value));
    EXPECT_EQ(-123, int16Value);
    EXPECT_FALSE(target.GetValue(uint16Value));
    EXPECT_EQ(0, uint16Value);
    EXPECT_TRUE(target.GetValue(int32Value));
    EXPECT_EQ(-123, int32Value);
    EXPECT_FALSE(target.GetValue(uint32Value));
    EXPECT_EQ(0u, uint32Value);
    EXPECT_TRUE(target.GetValue(int64Value));
    EXPECT_EQ(-123, int64Value);
    EXPECT_FALSE(target.GetValue(uint64Value));
    EXPECT_EQ(0u, uint64Value);
    EXPECT_TRUE(target.GetValue(floatValue));
    EXPECT_EQ(-123, floatValue);
    EXPECT_TRUE(target.GetValue(doubleValue));
    EXPECT_EQ(-123, doubleValue);

    stream.str(_("true"));
    stream.clear();
    EXPECT_FALSE(target.Deserialize(stream));
    EXPECT_EQ(_(""), target.GetValue());
    stream.str(_("0x1234"));
    stream.clear();
    EXPECT_FALSE(target.Deserialize(stream));
    EXPECT_EQ(_(""), target.GetValue());
    stream.str(_(""));
    stream.clear();
    EXPECT_FALSE(target.Deserialize(stream));
    EXPECT_EQ(_(""), target.GetValue());
}

TEST_FIXTURE(NumberTest, DeserializeFloatingPoint)
{
    Number target;

    EXPECT_EQ(ValueType::Number, target.Type());
    EXPECT_EQ(_(""), target.GetValue());
    std::basic_istringstream<OSAL::Char> stream(_("123.456"));
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(_("123.456"), target.GetValue());
    int8_t int8Value;
    EXPECT_FALSE(target.GetValue(int8Value));
    EXPECT_EQ(0, int8Value);
    uint8_t uint8Value;
    EXPECT_FALSE(target.GetValue(uint8Value));
    EXPECT_EQ(0, uint8Value);
    int16_t int16Value;
    EXPECT_FALSE(target.GetValue(int16Value));
    EXPECT_EQ(0, int16Value);
    uint16_t uint16Value;
    EXPECT_FALSE(target.GetValue(uint16Value));
    EXPECT_EQ(0, uint16Value);
    int32_t int32Value;
    EXPECT_FALSE(target.GetValue(int32Value));
    EXPECT_EQ(0, int32Value);
    uint32_t uint32Value;
    EXPECT_FALSE(target.GetValue(uint32Value));
    EXPECT_EQ(0u, uint32Value);
    int64_t int64Value;
    EXPECT_FALSE(target.GetValue(int64Value));
    EXPECT_EQ(0, int64Value);
    uint64_t uint64Value;
    EXPECT_FALSE(target.GetValue(uint64Value));
    EXPECT_EQ(0u, uint64Value);
    float floatValue;
    EXPECT_TRUE(target.GetValue(floatValue));
    EXPECT_NEAR(123.456F, floatValue, 0.0001);
    double doubleValue;
    EXPECT_TRUE(target.GetValue(doubleValue));
    EXPECT_NEAR(123.456, doubleValue, 0.0001);

    stream.str(_("-123.456"));
    stream.clear();
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(_("-123.456"), target.GetValue());
    EXPECT_FALSE(target.GetValue(int8Value));
    EXPECT_EQ(0, int8Value);
    EXPECT_FALSE(target.GetValue(uint8Value));
    EXPECT_EQ(0, uint8Value);
    EXPECT_FALSE(target.GetValue(int16Value));
    EXPECT_EQ(0, int16Value);
    EXPECT_FALSE(target.GetValue(uint16Value));
    EXPECT_EQ(0, uint16Value);
    EXPECT_FALSE(target.GetValue(int32Value));
    EXPECT_EQ(0, int32Value);
    EXPECT_FALSE(target.GetValue(uint32Value));
    EXPECT_EQ(0u, uint32Value);
    EXPECT_FALSE(target.GetValue(int64Value));
    EXPECT_EQ(0, int64Value);
    EXPECT_FALSE(target.GetValue(uint64Value));
    EXPECT_EQ(0u, uint64Value);
    EXPECT_TRUE(target.GetValue(floatValue));
    EXPECT_NEAR(-123.456F, floatValue, 0.0001);
    EXPECT_TRUE(target.GetValue(doubleValue));
    EXPECT_NEAR(-123.456, doubleValue, 0.0001);
    stream.str(_("0x1234"));
    stream.clear();
    EXPECT_FALSE(target.Deserialize(stream));
    EXPECT_EQ(_(""), target.GetValue());
    stream.str(_(""));
    stream.clear();
    EXPECT_FALSE(target.Deserialize(stream));
    EXPECT_EQ(_(""), target.GetValue());
}

TEST_FIXTURE(NumberTest, DeserializeFloatingPointScientific)
{
    Number target;

    EXPECT_EQ(ValueType::Number, target.Type());
    EXPECT_EQ(_(""), target.GetValue());
    std::basic_istringstream<OSAL::Char> stream(_("123.456e+29"));
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(_("123.456e+29"), target.GetValue());
    int8_t int8Value;
    EXPECT_FALSE(target.GetValue(int8Value));
    EXPECT_EQ(0, int8Value);
    uint8_t uint8Value;
    EXPECT_FALSE(target.GetValue(uint8Value));
    EXPECT_EQ(0, uint8Value);
    int16_t int16Value;
    EXPECT_FALSE(target.GetValue(int16Value));
    EXPECT_EQ(0, int16Value);
    uint16_t uint16Value;
    EXPECT_FALSE(target.GetValue(uint16Value));
    EXPECT_EQ(0, uint16Value);
    int32_t int32Value;
    EXPECT_FALSE(target.GetValue(int32Value));
    EXPECT_EQ(0, int32Value);
    uint32_t uint32Value;
    EXPECT_FALSE(target.GetValue(uint32Value));
    EXPECT_EQ(0u, uint32Value);
    int64_t int64Value;
    EXPECT_FALSE(target.GetValue(int64Value));
    EXPECT_EQ(0, int64Value);
    uint64_t uint64Value;
    EXPECT_FALSE(target.GetValue(uint64Value));
    EXPECT_EQ(0u, uint64Value);
    float floatValue;
    EXPECT_TRUE(target.GetValue(floatValue));
    EXPECT_NEAR(1.23456e+31F, floatValue, 0.0001);
    double doubleValue;
    EXPECT_TRUE(target.GetValue(doubleValue));
    EXPECT_NEAR(1.23456e+31, doubleValue, 0.0001);

    stream.str(_("123.456e-89"));
    stream.clear();
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(_("123.456e-89"), target.GetValue());
    stream.str(_("-123.456E89"));
    stream.clear();
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(_("-123.456E89"), target.GetValue());
    EXPECT_FALSE(target.GetValue(floatValue));
    EXPECT_EQ(0.0F, floatValue);
    EXPECT_TRUE(target.GetValue(doubleValue));
    EXPECT_NEAR(-1.23456e+91, doubleValue, 0.0001);
    stream.str(_("0x1234"));
    stream.clear();
    EXPECT_FALSE(target.Deserialize(stream));
    EXPECT_EQ(_(""), target.GetValue());
    stream.str(_(""));
    stream.clear();
    EXPECT_FALSE(target.Deserialize(stream));
    EXPECT_EQ(_(""), target.GetValue());
}

TEST_FIXTURE(NumberTest, Serialize)
{
    Number target;

    EXPECT_EQ(ValueType::Number, target.Type());
    EXPECT_EQ(_(""), target.GetValue());
    std::basic_ostringstream<OSAL::Char> stream;
    target.Serialize(stream);
    EXPECT_EQ(_(""), stream.str());

    stream.str(_(""));
    target.SetValue(static_cast<int8_t>(-123));
    target.Serialize(stream);
    EXPECT_EQ(_("-123"), stream.str());

    stream.str(_(""));
    target.SetValue(static_cast<uint8_t>(234));
    target.Serialize(stream);
    EXPECT_EQ(_("234"), stream.str());

    stream.str(_(""));
    target.SetValue(static_cast<int16_t>(-23456));
    target.Serialize(stream);
    EXPECT_EQ(_("-23456"), stream.str());

    stream.str(_(""));
    target.SetValue(static_cast<uint16_t>(34567));
    target.Serialize(stream);
    EXPECT_EQ(_("34567"), stream.str());

    stream.str(_(""));
    target.SetValue(static_cast<int32_t>(-1234567890));
    target.Serialize(stream);
    EXPECT_EQ(_("-1234567890"), stream.str());

    stream.str(_(""));
    target.SetValue(static_cast<uint32_t>(2345678901));
    target.Serialize(stream);
    EXPECT_EQ(_("2345678901"), stream.str());

    stream.str(_(""));
    target.SetValue(static_cast<int64_t>(-123456789012345));
    target.Serialize(stream);
    EXPECT_EQ(_("-123456789012345"), stream.str());

    stream.str(_(""));
    target.SetValue(static_cast<uint64_t>(123456789012345));
    target.Serialize(stream);
    EXPECT_EQ(_("123456789012345"), stream.str());

    stream.str(_(""));
    target.SetValue(static_cast<uint64_t>(123456789012345));
    target.Serialize(stream);
    EXPECT_EQ(_("123456789012345"), stream.str());

    stream.str(_(""));
    target.SetValue(static_cast<uint64_t>(123456789012345));
    target.Serialize(stream);
    EXPECT_EQ(_("123456789012345"), stream.str());

    stream.str(_(""));
    target.SetValue(static_cast<uint64_t>(123456789012345));
    target.Serialize(stream);
    EXPECT_EQ(_("123456789012345"), stream.str());

    stream.str(_(""));
    target.SetValue(123.456e23F);
    target.Serialize(stream);
    EXPECT_EQ(_("1.23456e+25"), stream.str());

    stream.str(_(""));
    target.SetValue(123.456789e123);
    target.Serialize(stream);
    EXPECT_EQ(_("1.23456789e+125"), stream.str());

    stream.str(_(""));
    target.SetValue(123.456789e123L);
    target.Serialize(stream);
    EXPECT_EQ(_("1.23456789e+125"), stream.str());
}

} // namespace Test
} // namespace JSON

