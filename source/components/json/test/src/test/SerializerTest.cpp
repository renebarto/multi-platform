#include <unittest-cpp/UnitTestC++.h>

#include "json/StringSerializer.h"

using namespace std;

namespace JSON {
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

TEST_FIXTURE(SerializerTest, Construct)
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

} // TEST_SUITE(json)

} // namespace Test
} // namespace JSON

