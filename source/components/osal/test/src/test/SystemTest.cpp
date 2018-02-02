#include <unittest-c++/UnitTestC++.h>

#include "osal/System.h"

namespace OSAL
{
namespace Test
{

class SystemTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();

    string PlatformName = PLATFORM_NAME;
};

void SystemTest::SetUp()
{
    if (PlatformName.empty())
    {
        std::cerr << "PlatformName undefined" << std::endl;
    }
}

void SystemTest::TearDown()
{
}

static void f(int)
{
}

class A
{
public:
    A()
    {
    }
};

TEST_SUITE(osal)
{

#if defined(LINUX)
    TEST_FIXTURE(SystemTest, PlatformLinux)
    {
        {
            std::string actual   = System::Name();
            std::string expected = "Linux";
            EXPECT_EQ(expected, actual);
        }
        {
            std::string actual = System::Variant();
            EXPECT_TRUE(!actual.empty());
        }
        {
            std::string actual = System::Release();
            EXPECT_TRUE(!actual.empty());
        }
        {
            std::string actual = System::Version();
            EXPECT_TRUE(!actual.empty());
        }
        {
            std::string actual   = System::Platform();
            std::string expected = PlatformName;
            EXPECT_EQ(expected, actual);
        }
        {
            std::string actual   = System::DemangleName("_ZN4OSAL3fooEv");
            std::string expected = "OSAL::foo()";
            EXPECT_EQ(expected, actual);
        }
    }
#endif // defined(LINUX)
#if defined(WIN_MSVC)
    TEST_FIXTURE(SystemTest, PlatformLinux)
    {
        {
            std::string actual   = System::Name();
            std::string expected = "Linux";
            EXPECT_EQ(expected, actual);
        }
        {
            std::string actual = System::Variant();
            EXPECT_TRUE(!actual.empty());
        }
        {
            std::string actual = System::Release();
            EXPECT_TRUE(!actual.empty());
        }
        {
            std::string actual = System::Version();
            EXPECT_TRUE(!actual.empty());
        }
        {
            std::string actual   = System::Platform();
            std::string expected = PlatformName;
            EXPECT_EQ(expected, actual);
        }
        {
            std::string actual   = System::DemangleName("_ZN4OSAL3fooEv");
            std::string expected = "OSAL::foo()";
            EXPECT_EQ(expected, actual);
        }
    }
#endif // defined(LINUX)

#if defined(LINUX) || defined(DARWIN) || defined(WIN_MINGW)
    TEST_FIXTURE(SystemTest, LookupFunction)
    {
        std::string typeName = System::TypeName(f);

        EXPECT_EQ("void (int)", typeName);
    }

    TEST_FIXTURE(SystemTest, LookupClass)
    {
        A a;
        std::string typeName = System::TypeName(a);

        EXPECT_EQ("OSAL::Test::A", typeName);
    }
#endif // defined(LINUX) || defined(DARWIN) || defined(WIN_MINGW)

#if defined(WIN_MSVC)
    TEST_FIXTURE(SystemTest, LookupFunction)
    {
        OSAL::String typeName = OSAL::OS::TypeName(f);

        EXPECT_EQ(_("void __cdecl(int)"), typeName);
    }

    TEST_FIXTURE(SystemTest, LookupClass)
    {
        A a;
        OSAL::String typeName = OSAL::OS::TypeName(a);

        EXPECT_EQ(_("OSAL::Test::A"), typeName);
    }
#endif // defined(WIN_MSVC)

TEST_FIXTURE(SystemTest, GetEnvironmentVariable)
{
    const char * ptr = System::GetEnvironmentVariable("PATH");
    std::string path(ptr ? ptr : "");

    EXPECT_NE("", path);

    ptr = System::GetEnvironmentVariable("SOME_DUMMY");
    std::string dummy(ptr ? ptr : "");

    EXPECT_EQ("", dummy);
}

TEST_FIXTURE(SystemTest, SetEnvironmentVariable)
{
    const char * ptr = System::GetEnvironmentVariable("SOME_DUMMY");
    std::string dummy(ptr ? ptr : "");
    EXPECT_EQ("", dummy);

    int result = System::SetEnvironmentVariable("SOME_DUMMY", "1234");
    EXPECT_EQ(0, result);

    ptr = System::GetEnvironmentVariable("SOME_DUMMY");
    dummy = ptr ? ptr : "";
    EXPECT_EQ("1234", dummy);

    result = System::SetEnvironmentVariable("SOME_DUMMY", "2345", false);
    EXPECT_EQ(0, result);

    ptr = System::GetEnvironmentVariable("SOME_DUMMY");
    dummy = ptr ? ptr : "";
    EXPECT_EQ("1234", dummy);

    result = System::SetEnvironmentVariable("SOME_DUMMY", "2345", true);
    EXPECT_EQ(0, result);

    ptr = System::GetEnvironmentVariable("SOME_DUMMY");
    dummy = ptr ? ptr : "";
    EXPECT_EQ("2345", dummy);

    result = System::UnSetEnvironmentVariable("SOME_DUMMY");
    EXPECT_EQ(0, result);
}

TEST_FIXTURE(SystemTest, UnsSetEnvironmentVariable)
{
    const char * ptr = System::GetEnvironmentVariable("SOME_DUMMY");
    std::string dummy(ptr ? ptr : "");
    EXPECT_EQ("", dummy);

    int result = System::SetEnvironmentVariable("SOME_DUMMY", "1234");
    EXPECT_EQ(0, result);

    ptr = System::GetEnvironmentVariable("SOME_DUMMY");
    dummy = ptr ? ptr : "";
    EXPECT_EQ("1234", dummy);

    result = System::UnSetEnvironmentVariable("SOME_DUMMY");
    EXPECT_EQ(0, result);

    ptr = System::GetEnvironmentVariable("SOME_DUMMY");
    dummy = ptr ? ptr : "";
    EXPECT_EQ("", dummy);
}

} // TEST_SUITE(osal)

} // namespace Test
} // namespace OSAL