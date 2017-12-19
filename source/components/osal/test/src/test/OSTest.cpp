#include <unittest-c++/UnitTestC++.h>

#include "osal/OS.h"

namespace OSAL {
namespace Test {

class OSTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();

    string PlatformName = PLATFORM_NAME;
};

void OSTest::SetUp()
{
    if (PlatformName.empty())
    {
        std::cerr << "PlatformName undefined" << std::endl;
    }
}

void OSTest::TearDown()
{
}

static void f(int)
{
}

class A
{
public:
    A() {}
};

TEST_SUITE(osal)
{

#if defined(LINUX)
TEST_FIXTURE(OSTest, PlatformLinux)
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
        std::string expected = PlatformName;
        EXPECT_EQ(expected, actual);
    }
    {
        std::string actual = OS::DemangleName("_ZN4OSAL3fooEv");
        std::string expected = "OSAL::foo()";
        EXPECT_EQ(expected, actual);
    }
}
#endif // defined(LINUX)

#if defined(LINUX) || defined(DARWIN) || defined(WIN_MINGW)
TEST_FIXTURE(OSTest, LookupFunction)
{
    std::string typeName = OS::TypeName(f);

    EXPECT_EQ("void (int)", typeName);
}

TEST_FIXTURE(OSTest, LookupClass)
{
    A a;
    std::string typeName = OS::TypeName(a);

    EXPECT_EQ("OSAL::Test::A", typeName);
}
#endif // defined(LINUX) || defined(DARWIN) || defined(WIN_MINGW)

#if defined(WIN_MSVC)
TEST_FIXTURE(OSTest, LookupFunction)
{
    OSAL::String typeName = OSAL::OS::TypeName(f);

    EXPECT_EQ(_("void __cdecl(int)"), typeName);
}

TEST_FIXTURE(OSTest, LookupClass)
{
    A a;
	OSAL::String typeName = OSAL::OS::TypeName(a);

    EXPECT_EQ(_("OSAL::Test::A"), typeName);
}
#endif // defined(WIN_MSVC)

} // TEST_SUITE(osal)

} // namespace Test
} // namespace OSAL