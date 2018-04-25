#include <unittest-cpp/UnitTestC++.h>
#include <limits>

#include "json/StringSerializer.h"
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
class StringSerializer<JSON::Test::MyClass> : public Core::IStringSerializer<JSON::Test::MyClass>
{
public:
    void Serialize(std::string & text, const JSON::Test::MyClass & value) override
    {
        Object variable;

        variable.AddPair(KVPair("x", std::make_shared<Number>(value.X())));
        variable.AddPair(KVPair("y", std::make_shared<Number>(value.Y())));

        std::ostringstream stream;
        variable.Serialize(stream);
        text = stream.str();
    }
};

namespace Test {

class SerializerTest: public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void SerializerTest::SetUp()
{
}

void SerializerTest::TearDown()
{
}

TEST_SUITE(json)
{

TEST_FIXTURE(SerializerTest, SerializeBool)
{
    std::string expected = "true";
    std::string serialized {};
    bool value {true};

    StringSerializer<bool> serializer;
    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);

    value = false;;
    expected = "false";

    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);
}

TEST_FIXTURE(SerializerTest, SerializeNumberInt8)
{
    std::string expected = "123";
    std::string serialized {};
    int8_t value {123};

    StringSerializer<int8_t> serializer;
    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);

    expected = "-128";
    value = std::numeric_limits<int8_t>::min();

    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);

    expected = "127";
    value = std::numeric_limits<int8_t>::max();

    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);
}

TEST_FIXTURE(SerializerTest, SerializeNumberUInt8)
{
    std::string expected = "234";
    std::string serialized {};
    uint8_t value {234};

    StringSerializer<uint8_t> serializer;
    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);

    expected = "0";
    value = std::numeric_limits<uint8_t>::min();

    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);

    expected = "255";
    value = std::numeric_limits<uint8_t>::max();

    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);
}

TEST_FIXTURE(SerializerTest, SerializeNumberInt16)
{
    std::string expected = "12345";
    std::string serialized {};
    int16_t value{12345};

    StringSerializer<int16_t> serializer;
    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);

    expected = "-32768";
    value = std::numeric_limits<int16_t>::min();

    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);

    expected = "32767";
    value = std::numeric_limits<int16_t>::max();

    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);
}

TEST_FIXTURE(SerializerTest, SerializeNumberUInt16)
{
    std::string expected = "34567";
    std::string serialized {};
    uint16_t value{34567};

    StringSerializer<uint16_t> serializer;
    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);

    expected = "0";
    value = std::numeric_limits<uint16_t>::min();

    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);

    expected = "65535";
    value = std::numeric_limits<uint16_t>::max();

    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);
}

TEST_FIXTURE(SerializerTest, SerializeNumberInt32)
{
    std::string expected = "12345678";
    std::string serialized {};
    int32_t value {12345678};

    StringSerializer<int32_t> serializer;
    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);

    expected = "-2147483648";
    value = std::numeric_limits<int32_t>::min();

    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);

    expected = "2147483647";
    value = std::numeric_limits<int32_t>::max();

    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);
}

TEST_FIXTURE(SerializerTest, SerializeNumberUInt32)
{
    std::string expected = "3217654321";
    std::string serialized {};
    uint32_t value {3217654321};

    StringSerializer<uint32_t> serializer;
    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);

    expected = "0";
    value = std::numeric_limits<uint32_t>::min();

    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);

    expected = "4294967295";
    value = std::numeric_limits<uint32_t>::max();

    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);
}

TEST_FIXTURE(SerializerTest, SerializeNumberInt64)
{
    std::string expected = "8765432109876543210";
    std::string serialized {};
    int64_t value {8765432109876543210};

    StringSerializer<int64_t> serializer;
    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);

    expected = "-9223372036854775808";
    value = std::numeric_limits<int64_t>::min();

    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);

    expected = "9223372036854775807";
    value = std::numeric_limits<int64_t>::max();

    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);
}

TEST_FIXTURE(SerializerTest, SerializeNumberUInt64)
{
    std::string expected = "9876543210987654321";
    std::string serialized {};
    uint64_t value {9876543210987654321ULL};

    StringSerializer<uint64_t> serializer;
    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);

    expected = "0";
    value = std::numeric_limits<uint64_t>::min();

    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);

    expected = "18446744073709551615";
    value = std::numeric_limits<uint64_t>::max();

    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);
}

TEST_FIXTURE(SerializerTest, SerializeNumberFloat)
{
    std::string expected = "123456.78";
    std::string serialized {};
    float value {123456.78F};

    StringSerializer<float> serializer;
    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);

    expected = "1.1754944e-38";
    value = std::numeric_limits<float>::min();

    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);

    expected = "3.4028235e+38";
    value = std::numeric_limits<float>::max();

    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);
}

TEST_FIXTURE(SerializerTest, SerializeNumberDouble)
{
    std::string expected = "1234.5678";
    std::string serialized {};
    double value {1234.5678};

    StringSerializer<double> serializer;
    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);

    expected = "2.225073858507201e-308";
    value = std::numeric_limits<double>::min();

    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);

    expected = "1.797693134862316e+308";
    value = std::numeric_limits<double>::max();

    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);
}

TEST_FIXTURE(SerializerTest, SerializeNumberLongDouble)
{
    std::string expected = "1234.5678";
    std::string serialized {};
    long double value {1234.5678L};

    StringSerializer<long double> serializer;
    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);

    expected = "3.362103143112094e-1000";
    value = 3.362103143112094e-1000L;

    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);

    expected = "1.189731495357231e+1000";
    value = 1.189731495357231e+1000L;

    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);
}

TEST_FIXTURE(SerializerTest, SerializeString)
{
    std::string expected = "\"Some text\"";
    std::string serialized {};
    std::string value {"Some text"};

    StringSerializer<std::string> serializer;
    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);

    expected = "\"Some text 2\"";
    value = "Some text 2";

    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);

    expected = "\"\"";
    value = {};

    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);
}

TEST_FIXTURE(SerializerTest, SerializeArray)
{
    std::string expected = "[\n    1,\n    2,\n    3\n]";
    std::string serialized {};
    Array value {std::make_shared<Number>(1),
                 std::make_shared<Number>(2),
                 std::make_shared<Number>(3)};

    StringSerializer<Array> serializer;
    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);

    expected = "[\n    12.34,\n    1234,\n    12345678\n]";
    value = {std::make_shared<Number>(float {12.34F}),
             std::make_shared<Number>(1234),
             std::make_shared<Number>(int64_t {12345678LL})};

    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);

    expected = "[\n]";
    value = {};

    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);
}

TEST_FIXTURE(SerializerTest, SerializeObject)
{
    std::string expected = "{\n    \"x\" : 123,\n    \"y\" : 456\n}";
    std::string serialized {};
    MyClass value(123, 456);

    StringSerializer<MyClass> serializer;
    serializer.Serialize(serialized, value);
    EXPECT_EQ(expected, serialized);

}

} // TEST_SUITE(json)

} // namespace Test
} // namespace JSON

