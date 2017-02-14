#include <unit-test-c++/UnitTestC++.h>

#include "osal/osal.h"

namespace OSAL {
namespace Test {

class OSTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void OSTest::SetUp()
{
}

void OSTest::TearDown()
{
}

#ifdef LINUX
TEST_FIXTURE(OSTest, Linux)
{
    {
        std::string actual = OS::Name();
        std::string expected = "Linux";
        EXPECT_EQ(expected, actual);
    }
    {
        std::string actual = OS::Variant();
        EXPECT_TRUE(!actual.empty());
    }
    {
        std::string actual = OS::Release();
        EXPECT_TRUE(!actual.empty());
    }
    {
        std::string actual = OS::Version();
        EXPECT_TRUE(!actual.empty());
    }
    {
        std::string actual = OS::Platform();
        std::string expected = "x86_64";
        EXPECT_EQ(expected, actual);
    }
    {
        std::string actual = OS::DemangleName("_ZN4OSAL3fooEv");
        std::string expected = "OSAL::foo()";
        EXPECT_EQ(expected, actual);
    }
}
#endif // LINUX

} // namespace Test
} // namespace OSAL