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

TEST_FIXTURE(NumberTest, ConstructInitializerString)
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

TEST_FIXTURE(NumberTest, ConstructInitializerInt8)
{
    int8_t value = 123;
    Number target(value);

    EXPECT_EQ(ValueType::Number, target.Type());
    EXPECT_EQ(_("123"), target.GetValue());
    int8_t int8Value;
    EXPECT_TRUE(target.GetValue(int8Value));
    EXPECT_EQ(static_cast<int8_t>(value), int8Value);
    uint8_t uint8Value;
    EXPECT_TRUE(target.GetValue(uint8Value));
    EXPECT_EQ(static_cast<uint8_t>(value), uint8Value);
    int16_t int16Value;
    EXPECT_TRUE(target.GetValue(int16Value));
    EXPECT_EQ(static_cast<int16_t>(value), int16Value);
    uint16_t uint16Value;
    EXPECT_TRUE(target.GetValue(uint16Value));
    EXPECT_EQ(static_cast<uint16_t>(value), uint16Value);
    int32_t int32Value;
    EXPECT_TRUE(target.GetValue(int32Value));
    EXPECT_EQ(static_cast<int32_t>(value), int32Value);
    uint32_t uint32Value;
    EXPECT_TRUE(target.GetValue(uint32Value));
    EXPECT_EQ(static_cast<uint32_t>(value), uint32Value);
    int64_t int64Value;
    EXPECT_TRUE(target.GetValue(int64Value));
    EXPECT_EQ(static_cast<int64_t>(value), int64Value);
    uint64_t uint64Value;
    EXPECT_TRUE(target.GetValue(uint64Value));
    EXPECT_EQ(static_cast<uint64_t>(value), uint64Value);
    float floatValue;
    EXPECT_TRUE(target.GetValue(floatValue));
    EXPECT_EQ(static_cast<float>(value), floatValue);
    double doubleValue;
    EXPECT_TRUE(target.GetValue(doubleValue));
    EXPECT_EQ(static_cast<double>(value), doubleValue);
    long double longdoubleValue;
    EXPECT_TRUE(target.GetValue(longdoubleValue));
    EXPECT_EQ(static_cast<long double>(value), longdoubleValue);
}

TEST_FIXTURE(NumberTest, ConstructInitializerUInt8)
{
    uint8_t value = 234;
    Number target(value);

    EXPECT_EQ(ValueType::Number, target.Type());
    EXPECT_EQ(_("234"), target.GetValue());
    int8_t int8Value;
    EXPECT_FALSE(target.GetValue(int8Value));
    EXPECT_EQ(0, int8Value);
    uint8_t uint8Value;
    EXPECT_TRUE(target.GetValue(uint8Value));
    EXPECT_EQ(static_cast<uint8_t>(value), uint8Value);
    int16_t int16Value;
    EXPECT_TRUE(target.GetValue(int16Value));
    EXPECT_EQ(static_cast<int16_t>(value), int16Value);
    uint16_t uint16Value;
    EXPECT_TRUE(target.GetValue(uint16Value));
    EXPECT_EQ(static_cast<uint16_t>(value), uint16Value);
    int32_t int32Value;
    EXPECT_TRUE(target.GetValue(int32Value));
    EXPECT_EQ(static_cast<int32_t>(value), int32Value);
    uint32_t uint32Value;
    EXPECT_TRUE(target.GetValue(uint32Value));
    EXPECT_EQ(static_cast<uint32_t>(value), uint32Value);
    int64_t int64Value;
    EXPECT_TRUE(target.GetValue(int64Value));
    EXPECT_EQ(static_cast<int64_t>(value), int64Value);
    uint64_t uint64Value;
    EXPECT_TRUE(target.GetValue(uint64Value));
    EXPECT_EQ(static_cast<uint64_t>(value), uint64Value);
    float floatValue;
    EXPECT_TRUE(target.GetValue(floatValue));
    EXPECT_EQ(static_cast<float>(value), floatValue);
    double doubleValue;
    EXPECT_TRUE(target.GetValue(doubleValue));
    EXPECT_EQ(static_cast<double>(value), doubleValue);
    long double longdoubleValue;
    EXPECT_TRUE(target.GetValue(longdoubleValue));
    EXPECT_EQ(static_cast<long double>(value), longdoubleValue);
}

TEST_FIXTURE(NumberTest, ConstructInitializerInt16)
{
    int16_t value = 23456;
    Number target(value);

    EXPECT_EQ(ValueType::Number, target.Type());
    EXPECT_EQ(_("23456"), target.GetValue());
    int8_t int8Value;
    EXPECT_FALSE(target.GetValue(int8Value));
    EXPECT_EQ(0, int8Value);
    uint8_t uint8Value;
    EXPECT_FALSE(target.GetValue(uint8Value));
    EXPECT_EQ(0, uint8Value);
    int16_t int16Value;
    EXPECT_TRUE(target.GetValue(int16Value));
    EXPECT_EQ(static_cast<int16_t>(value), int16Value);
    uint16_t uint16Value;
    EXPECT_TRUE(target.GetValue(uint16Value));
    EXPECT_EQ(static_cast<uint16_t>(value), uint16Value);
    int32_t int32Value;
    EXPECT_TRUE(target.GetValue(int32Value));
    EXPECT_EQ(static_cast<int32_t>(value), int32Value);
    uint32_t uint32Value;
    EXPECT_TRUE(target.GetValue(uint32Value));
    EXPECT_EQ(static_cast<uint32_t>(value), uint32Value);
    int64_t int64Value;
    EXPECT_TRUE(target.GetValue(int64Value));
    EXPECT_EQ(static_cast<int64_t>(value), int64Value);
    uint64_t uint64Value;
    EXPECT_TRUE(target.GetValue(uint64Value));
    EXPECT_EQ(static_cast<uint64_t>(value), uint64Value);
    float floatValue;
    EXPECT_TRUE(target.GetValue(floatValue));
    EXPECT_EQ(static_cast<float>(value), floatValue);
    double doubleValue;
    EXPECT_TRUE(target.GetValue(doubleValue));
    EXPECT_EQ(static_cast<double>(value), doubleValue);
    long double longdoubleValue;
    EXPECT_TRUE(target.GetValue(longdoubleValue));
    EXPECT_EQ(static_cast<long double>(value), longdoubleValue);
}

TEST_FIXTURE(NumberTest, ConstructInitializerUInt16)
{
    int16_t value = 34567;
    Number target(value);

    EXPECT_EQ(ValueType::Number, target.Type());
    EXPECT_EQ(_("34567"), target.GetValue());
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
    EXPECT_TRUE(target.GetValue(uint16Value));
    EXPECT_EQ(static_cast<uint16_t>(value), uint16Value);
    int32_t int32Value;
    EXPECT_TRUE(target.GetValue(int32Value));
    EXPECT_EQ(static_cast<int32_t>(value), int32Value);
    uint32_t uint32Value;
    EXPECT_TRUE(target.GetValue(uint32Value));
    EXPECT_EQ(static_cast<uint32_t>(value), uint32Value);
    int64_t int64Value;
    EXPECT_TRUE(target.GetValue(int64Value));
    EXPECT_EQ(static_cast<int64_t>(value), int64Value);
    uint64_t uint64Value;
    EXPECT_TRUE(target.GetValue(uint64Value));
    EXPECT_EQ(static_cast<uint64_t>(value), uint64Value);
    float floatValue;
    EXPECT_TRUE(target.GetValue(floatValue));
    EXPECT_EQ(static_cast<float>(value), floatValue);
    double doubleValue;
    EXPECT_TRUE(target.GetValue(doubleValue));
    EXPECT_EQ(static_cast<double>(value), doubleValue);
    long double longdoubleValue;
    EXPECT_TRUE(target.GetValue(longdoubleValue));
    EXPECT_EQ(static_cast<long double>(value), longdoubleValue);
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
    uint8_t uint8Value;
    int16_t int16Value;
    uint16_t uint16Value;
    int32_t int32Value;
    uint32_t uint32Value;
    int64_t int64Value;
    uint64_t uint64Value;
    float floatValue;
    double doubleValue;
    long double longdoubleValue;
    EXPECT_TRUE(target.GetValue(int8Value));
    EXPECT_EQ(123, int8Value);
    EXPECT_TRUE(target.GetValue(uint8Value));
    EXPECT_EQ(123, uint8Value);
    EXPECT_TRUE(target.GetValue(int16Value));
    EXPECT_EQ(123, int16Value);
    EXPECT_TRUE(target.GetValue(uint16Value));
    EXPECT_EQ(123, uint16Value);
    EXPECT_TRUE(target.GetValue(int32Value));
    EXPECT_EQ(123, int32Value);
    EXPECT_TRUE(target.GetValue(uint32Value));
    EXPECT_EQ(123u, uint32Value);
    EXPECT_TRUE(target.GetValue(int64Value));
    EXPECT_EQ(123, int64Value);
    EXPECT_TRUE(target.GetValue(uint64Value));
    EXPECT_EQ(123u, uint64Value);
    EXPECT_TRUE(target.GetValue(floatValue));
    EXPECT_EQ(123, floatValue);
    EXPECT_TRUE(target.GetValue(doubleValue));
    EXPECT_EQ(123, doubleValue);
    EXPECT_TRUE(target.GetValue(longdoubleValue));
    EXPECT_EQ(123, longdoubleValue);
}

TEST_FIXTURE(NumberTest, DeserializeIntegerNegative)
{
    Number target;

    EXPECT_EQ(ValueType::Number, target.Type());
    EXPECT_EQ(_(""), target.GetValue());
    std::basic_istringstream<OSAL::Char> stream(_("-123"));
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(_("-123"), target.GetValue());
    int8_t int8Value;
    uint8_t uint8Value;
    int16_t int16Value;
    uint16_t uint16Value;
    int32_t int32Value;
    uint32_t uint32Value;
    int64_t int64Value;
    uint64_t uint64Value;
    float floatValue;
    double doubleValue;
    long double longdoubleValue;
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
    EXPECT_TRUE(target.GetValue(longdoubleValue));
    EXPECT_EQ(-123, longdoubleValue);
}

TEST_FIXTURE(NumberTest, DeserializeInvalidBoolean)
{
    Number target;

    EXPECT_EQ(ValueType::Number, target.Type());
    EXPECT_EQ(_(""), target.GetValue());
    std::basic_istringstream<OSAL::Char> stream(_("true"));
    EXPECT_FALSE(target.Deserialize(stream));
    EXPECT_EQ(_(""), target.GetValue());
}

TEST_FIXTURE(NumberTest, DeserializeInvalidHex)
{
    Number target;

    EXPECT_EQ(ValueType::Number, target.Type());
    EXPECT_EQ(_(""), target.GetValue());
    std::basic_istringstream<OSAL::Char> stream(_("0x1234"));
    EXPECT_FALSE(target.Deserialize(stream));
    EXPECT_EQ(_(""), target.GetValue());
}

TEST_FIXTURE(NumberTest, DeserializeEmpty)
{
    Number target;

    EXPECT_EQ(ValueType::Number, target.Type());
    EXPECT_EQ(_(""), target.GetValue());
    std::basic_istringstream<OSAL::Char> stream(_(""));
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
    uint8_t uint8Value;
    int16_t int16Value;
    uint16_t uint16Value;
    int32_t int32Value;
    uint32_t uint32Value;
    int64_t int64Value;
    uint64_t uint64Value;
    float floatValue;
    double doubleValue;
    long double longdoubleValue;
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
    EXPECT_NEAR(123.456F, floatValue, 0.0001);
    EXPECT_TRUE(target.GetValue(doubleValue));
    EXPECT_NEAR(123.456, doubleValue, 0.0001);
    EXPECT_TRUE(target.GetValue(longdoubleValue));
    EXPECT_NEAR(123.456, longdoubleValue, 0.0001);
}

TEST_FIXTURE(NumberTest, DeserializeFloatingPointNegative)
{
    Number target;

    EXPECT_EQ(ValueType::Number, target.Type());
    EXPECT_EQ(_(""), target.GetValue());
    std::basic_istringstream<OSAL::Char> stream(_("-123.456"));
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(_("-123.456"), target.GetValue());
    int8_t int8Value;
    uint8_t uint8Value;
    int16_t int16Value;
    uint16_t uint16Value;
    int32_t int32Value;
    uint32_t uint32Value;
    int64_t int64Value;
    uint64_t uint64Value;
    float floatValue;
    double doubleValue;
    long double longdoubleValue;
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
    EXPECT_TRUE(target.GetValue(longdoubleValue));
    EXPECT_NEAR(-123.456, longdoubleValue, 0.0001);
}

TEST_FIXTURE(NumberTest, DeserializeFloatingPointScientificInFloatRange)
{
    Number target;

    EXPECT_EQ(ValueType::Number, target.Type());
    EXPECT_EQ(_(""), target.GetValue());
    std::basic_istringstream<OSAL::Char> stream(_("123.456e+29"));
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(_("123.456e+29"), target.GetValue());
    int8_t int8Value;
    uint8_t uint8Value;
    int16_t int16Value;
    uint16_t uint16Value;
    int32_t int32Value;
    uint32_t uint32Value;
    int64_t int64Value;
    uint64_t uint64Value;
    float floatValue;
    double doubleValue;
    long double longdoubleValue;
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
    EXPECT_NEAR(1.23456e+31F, floatValue, 0.0001F);
    EXPECT_TRUE(target.GetValue(doubleValue));
    EXPECT_NEAR(1.23456e+31, doubleValue, 0.0001);
    EXPECT_TRUE(target.GetValue(longdoubleValue));
    EXPECT_NEAR(1.23456e+31L, longdoubleValue, 0.0001L);
}

TEST_FIXTURE(NumberTest, DeserializeFloatingPointScientificOutsideFloatRange1)
{
    Number target;

    EXPECT_EQ(ValueType::Number, target.Type());
    EXPECT_EQ(_(""), target.GetValue());
    std::basic_istringstream<OSAL::Char> stream(_("123.456e-89"));
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(_("123.456e-89"), target.GetValue());
    float floatValue;
    EXPECT_FALSE(target.GetValue(floatValue));
    EXPECT_EQ(0.0F, floatValue);
    double doubleValue;
    EXPECT_TRUE(target.GetValue(doubleValue));
    EXPECT_NEAR(1.23456e-87, doubleValue, 0.0001);
    long double longdoubleValue;
    EXPECT_TRUE(target.GetValue(longdoubleValue));
    EXPECT_NEAR(1.23456e-87, longdoubleValue, 0.0001);
}

TEST_FIXTURE(NumberTest, DeserializeFloatingPointScientificOutsideFloatRange2)
{
    Number target;

    EXPECT_EQ(ValueType::Number, target.Type());
    EXPECT_EQ(_(""), target.GetValue());
    std::basic_istringstream<OSAL::Char> stream(_("-123.456E89"));
    EXPECT_TRUE(target.Deserialize(stream));
    EXPECT_EQ(_("-123.456E89"), target.GetValue());
    float floatValue;
    EXPECT_FALSE(target.GetValue(floatValue));
    EXPECT_EQ(0.0F, floatValue);
    double doubleValue;
    EXPECT_TRUE(target.GetValue(doubleValue));
    EXPECT_NEAR(-1.23456e+91, doubleValue, 0.0001);
    long double longdoubleValue;
    EXPECT_TRUE(target.GetValue(longdoubleValue));
    EXPECT_NEAR(-1.23456e+91L, longdoubleValue, 0.0001L);
}

TEST_FIXTURE(NumberTest, SerializeUninitialized)
{
    Number target;

    std::basic_ostringstream<OSAL::Char> stream;
    target.Serialize(stream);
    EXPECT_EQ(_(""), stream.str());
}

TEST_FIXTURE(NumberTest, SerializeInt8)
{
    Number target;

    std::basic_ostringstream<OSAL::Char> stream;
    target.SetValue(static_cast<int8_t>(-123));
    target.Serialize(stream);
    EXPECT_EQ(_("-123"), stream.str());
}

TEST_FIXTURE(NumberTest, SerializeUInt8)
{
    Number target;

    std::basic_ostringstream<OSAL::Char> stream;
    target.SetValue(static_cast<uint8_t>(234));
    target.Serialize(stream);
    EXPECT_EQ(_("234"), stream.str());
}

TEST_FIXTURE(NumberTest, SerializeInt16)
{
    Number target;

    std::basic_ostringstream<OSAL::Char> stream;
    target.Serialize(stream);
    target.SetValue(static_cast<int16_t>(-23456));
    target.Serialize(stream);
    EXPECT_EQ(_("-23456"), stream.str());
}

TEST_FIXTURE(NumberTest, SerializeUInt16)
{
    Number target;

    std::basic_ostringstream<OSAL::Char> stream;
    target.SetValue(static_cast<uint16_t>(34567));
    target.Serialize(stream);
    EXPECT_EQ(_("34567"), stream.str());
}

TEST_FIXTURE(NumberTest, SerializeInt32)
{
    Number target;

    std::basic_ostringstream<OSAL::Char> stream;
    target.Serialize(stream);
    target.SetValue(static_cast<int32_t>(-1234567890));
    target.Serialize(stream);
    EXPECT_EQ(_("-1234567890"), stream.str());
}

TEST_FIXTURE(NumberTest, SerializeUInt32)
{
    Number target;

    std::basic_ostringstream<OSAL::Char> stream;
    target.SetValue(static_cast<uint32_t>(2345678901));
    target.Serialize(stream);
    EXPECT_EQ(_("2345678901"), stream.str());
}

TEST_FIXTURE(NumberTest, SerializeInt64)
{
    Number target;

    std::basic_ostringstream<OSAL::Char> stream;
    target.SetValue(static_cast<int64_t>(-123456789012345));
    target.Serialize(stream);
    EXPECT_EQ(_("-123456789012345"), stream.str());
}

TEST_FIXTURE(NumberTest, SerializeUInt64)
{
    Number target;

    std::basic_ostringstream<OSAL::Char> stream;
    target.SetValue(static_cast<uint64_t>(123456789012345));
    target.Serialize(stream);
    EXPECT_EQ(_("123456789012345"), stream.str());
}

TEST_FIXTURE(NumberTest, SerializeFloat)
{
    Number target;

    std::basic_ostringstream<OSAL::Char> stream;
    target.SetValue(123.456e23F);
    target.Serialize(stream);
    EXPECT_EQ(_("1.23456e+25"), stream.str());
}

TEST_FIXTURE(NumberTest, SerializeDouble)
{
    Number target;

    std::basic_ostringstream<OSAL::Char> stream;
    target.SetValue(123.456789e123);
    target.Serialize(stream);
    EXPECT_EQ(_("1.23456789e+125"), stream.str());
}

TEST_FIXTURE(NumberTest, SerializeLongDouble)
{
    Number target;

    std::basic_ostringstream<OSAL::Char> stream;
    target.SetValue(123.456789e123L);
    target.Serialize(stream);
    EXPECT_EQ(_("1.23456789e+125"), stream.str());
}

} // namespace Test
} // namespace JSON

