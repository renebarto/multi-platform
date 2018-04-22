#include <unittest-cpp/UnitTestC++.h>
#include <limits>

#include "json/StringDeserializer.h"
#include "json/Boolean.h"
#include "json/Object.h"

using namespace std;

namespace JSON {

namespace Test {

class MyClass
{
public:
    MyClass()
        : _x(), _y()
    {}

    MyClass(int x, int y)
        : _x(x), _y(y)
    {}

    int X() const
    { return _x; }

    int Y() const
    { return _y; }

    bool operator == (const MyClass & other) const
    {
        return (_x == other._x) && (_y == other._y);
    }
    bool operator != (const MyClass & other) const
    {
        return ! operator == (other);
    }

private:
    int _x;
    int _y;
};

} // namespace Test

template<>
class StringDeserializer<JSON::Test::MyClass> : public Core::IStringDeserializer<JSON::Test::MyClass>
{
public:
    bool Deserialize(const std::string & text, JSON::Test::MyClass & result) override
    {
        Object value;
        std::istringstream stream(text);
        if (!value.Deserialize(stream))
            return false;
        auto it = value.GetIterator();
        int x {};
        int y {};
        while (!it.AtEnd())
        {
            if ((it.operator*().GetKey() == "x") && (it.operator*().GetValue()->Type() == ValueType::Number))
            {
                dynamic_pointer_cast<Number>(it.operator*().GetValue())->GetValue(x);
            } else if ((it.operator*().GetKey() == "y") && (it.operator*().GetValue()->Type() == ValueType::Number))
            {
                dynamic_pointer_cast<Number>(it.operator*().GetValue())->GetValue(y);
            } else
                return false;
            it++;
        }
        result = JSON::Test::MyClass(x, y);
        return true;
    }
};

namespace Test {

class DeserializerTest: public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void DeserializerTest::SetUp()
{
}

void DeserializerTest::TearDown()
{
}

TEST_SUITE(json) {

TEST_FIXTURE(DeserializerTest, DeserializeBool)
{
    std::string serialized = "True";
    bool value;

    StringDeserializer<bool> deserializer;
    EXPECT_TRUE(deserializer.Deserialize(serialized, value));
    EXPECT_TRUE(value);

    serialized = "False";

    EXPECT_TRUE(deserializer.Deserialize(serialized, value));
    EXPECT_FALSE(value);

    serialized = "Invalid";

    EXPECT_FALSE(deserializer.Deserialize(serialized, value));
    EXPECT_FALSE(value);

    serialized = "";
    value = true;

    EXPECT_FALSE(deserializer.Deserialize(serialized, value));
    EXPECT_TRUE(value);
}

TEST_FIXTURE(DeserializerTest, DeserializeNumberInt8)
{
    std::string serialized = "123";
    int8_t value;

    StringDeserializer<int8_t> deserializer;
    EXPECT_TRUE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(int8_t{123}, value);

    serialized = "-128";

    EXPECT_TRUE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(int8_t{-128}, value);

    serialized = "255";

    EXPECT_FALSE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(int8_t{0}, value);

    serialized = "";
    value = 127;

    EXPECT_FALSE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(int8_t{127}, value);
}

TEST_FIXTURE(DeserializerTest, DeserializeNumberUInt8)
{
    std::string serialized = "234";
    uint8_t value;

    StringDeserializer<uint8_t> deserializer;
    EXPECT_TRUE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(uint8_t{234}, value);

    serialized = "123";

    EXPECT_TRUE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(uint8_t{123}, value);

    serialized = "256";

    EXPECT_FALSE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(uint8_t{0}, value);

    serialized = "";
    value = 127;

    EXPECT_FALSE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(uint8_t{127}, value);
}

TEST_FIXTURE(DeserializerTest, DeserializeNumberInt16)
{
    std::string serialized = "12345";
    int16_t value;

    StringDeserializer<int16_t> deserializer;
    EXPECT_TRUE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(int16_t{12345}, value);

    serialized = "-32768";

    EXPECT_TRUE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(int16_t{-32768}, value);

    serialized = "65536";

    EXPECT_FALSE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(int16_t{0}, value);

    serialized = "";
    value = 32767;

    EXPECT_FALSE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(int16_t{32767}, value);
}

TEST_FIXTURE(DeserializerTest, DeserializeNumberUInt16)
{
    std::string serialized = "34567";
    uint16_t value;

    StringDeserializer<uint16_t> deserializer;
    EXPECT_TRUE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(uint16_t{34567}, value);

    serialized = "65432";

    EXPECT_TRUE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(uint16_t{65432}, value);

    serialized = "65536";

    EXPECT_FALSE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(uint16_t{0}, value);

    serialized = "";
    value = 65535;

    EXPECT_FALSE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(uint16_t{65535}, value);
}

TEST_FIXTURE(DeserializerTest, DeserializeNumberInt32)
{
    std::string serialized = "12345678";
    int32_t value;

    StringDeserializer<int32_t> deserializer;
    EXPECT_TRUE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(int32_t{12345678}, value);

    serialized = "-2147483648";

    EXPECT_TRUE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(int32_t{-2147483648}, value);

    serialized = "4294967295";

    EXPECT_FALSE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(int32_t{0}, value);

    serialized = "";
    value = 2147483647;

    EXPECT_FALSE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(int32_t{2147483647}, value);
}

TEST_FIXTURE(DeserializerTest, DeserializeNumberUInt32)
{
    std::string serialized = "3217654321";
    uint32_t value;

    StringDeserializer<uint32_t> deserializer;
    EXPECT_TRUE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(uint32_t{3217654321}, value);

    serialized = "1234567890";

    EXPECT_TRUE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(uint32_t{1234567890}, value);

    serialized = "4294967296";

    EXPECT_FALSE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(uint32_t{0}, value);

    serialized = "";
    value = 2147483647;

    EXPECT_FALSE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(uint32_t{2147483647}, value);
}

TEST_FIXTURE(DeserializerTest, DeserializeNumberInt64)
{
    std::string serialized = "9223372036854775807";
    int64_t value;

    StringDeserializer<int64_t> deserializer;
    EXPECT_TRUE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(int64_t{9223372036854775807LL}, value);

    serialized = "-9223372036854775807";

    EXPECT_TRUE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(int64_t{-9223372036854775807LL}, value);

    serialized = "18446744073709551616";

    EXPECT_FALSE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(int64_t{0}, value);

    serialized = "";
    value = 9223372036854775807LL;

    EXPECT_FALSE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(int64_t{9223372036854775807LL}, value);
}

TEST_FIXTURE(DeserializerTest, DeserializeNumberUInt64)
{
    std::string serialized = "18446744073709551615";
    uint64_t value;

    StringDeserializer<uint64_t> deserializer;
    EXPECT_TRUE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(uint64_t{18446744073709551615ULL}, value);

    serialized = "9446744073709551615";

    EXPECT_TRUE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(uint64_t{9446744073709551615ULL}, value);

    serialized = "18446744073709551616";

    // Value is rounded down to max for uint64_t
    EXPECT_TRUE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(std::numeric_limits<uint64_t>::max(), value);

    serialized = "";
    value = 9223372036854775807ULL;

    EXPECT_FALSE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(uint64_t{9223372036854775807ULL}, value);
}

TEST_FIXTURE(DeserializerTest, DeserializeNumberFloat)
{
    std::string serialized = "1234.5678";
    float value;

    StringDeserializer<float> deserializer;
    EXPECT_TRUE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(float{1234.5678F}, value);

    serialized = "1234.5678E+03";

    EXPECT_TRUE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(float{1234.5678E+03F}, value);

    serialized = "1234E1234";

    EXPECT_FALSE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(float{0.0F}, value);

    serialized = "";
    value = 3.40283E37F;

    EXPECT_FALSE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(float{3.40283E37F}, value);
}

TEST_FIXTURE(DeserializerTest, DeserializeNumberDouble)
{
    std::string serialized = "1234.5678";
    double value;

    StringDeserializer<double> deserializer;
    EXPECT_TRUE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(double{1234.5678}, value);

    serialized = "1234.5678E+03";

    EXPECT_TRUE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(double{1234.5678E+03}, value);

    serialized = "1234E1234";

    EXPECT_FALSE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(double{0.0}, value);

    serialized = "";
    value = 3.40283E37;

    EXPECT_FALSE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(double{3.40283E37}, value);
}

TEST_FIXTURE(DeserializerTest, DeserializeNumberLongDouble)
{
    std::string serialized = "1234.5678";
    long double value;

    StringDeserializer<long double> deserializer;
    EXPECT_TRUE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(1234.5678L, value);

    serialized = "1234.5678E+03";

    EXPECT_TRUE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(1234.5678E+03L, value);

    serialized = "1234E123456";

    // Value is rounded down to max for long double
    EXPECT_TRUE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(std::numeric_limits<long double>::max(), value);

    serialized = "";
    value = 3.40283E37L;

    EXPECT_FALSE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ(3.40283E37L, value);
}

TEST_FIXTURE(DeserializerTest, DeserializeString)
{
    std::string serialized = "\"Some text\"";
    std::string value;

    StringDeserializer<std::string> deserializer;
    EXPECT_TRUE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ("Some text", value);

    serialized = "Some text 2";

    EXPECT_FALSE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ("Some text", value);

    serialized = "";
    value = "Some other text";

    EXPECT_FALSE(deserializer.Deserialize(serialized, value));
    EXPECT_EQ("Some other text", value);
}

TEST_FIXTURE(DeserializerTest, DeserializeArray)
{
    std::string serialized = "[1,2,3]";
    Array value;

    StringDeserializer<Array> deserializer;
    EXPECT_TRUE(deserializer.Deserialize(serialized, value));
    Array expected{std::make_shared<Number>(1),
                   std::make_shared<Number>(2),
                   std::make_shared<Number>(3)};
    EXPECT_EQ(expected, value);

}

TEST_FIXTURE(DeserializerTest, DeserializeObject)
{
    std::string serialized = "{ \"x\" : 123, \"y\" : 456}";
    MyClass value;

    StringDeserializer<MyClass> deserializer;
    EXPECT_TRUE(deserializer.Deserialize(serialized, value));
    MyClass expected(123, 456);
    EXPECT_EQ(expected, value);

}

} // TEST_SUITE(json)

} // namespace Test
} // namespace JSON

