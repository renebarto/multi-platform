#include <unittest-cpp/UnitTestC++.h>

#include "osal/System.h"

#if defined(WIN_MSVC)
#define IS_BIG_ENDIAN (*(uint16_t *)"\0\xff" < 0x100)
#define IS_LITTLE_ENDIAN (!IS_BIG_ENDIAN)
#elif defined(WIN_MINGW)
#define IS_BIG_ENDIAN (*(uint16_t *)"\0\xff" < 0x100)
#define IS_LITTLE_ENDIAN (!IS_BIG_ENDIAN)
#elif defined(DARWIN)
#elif defined(LINUX)
#define IS_BIG_ENDIAN (__BYTE_ORDER__ == __ORDER_NIG_ENDIAN__)
#define IS_LITTLE_ENDIAN (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#endif

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

TEST_FIXTURE(SystemTest, PlatformEndianness)
{
    {
        Endianness actual   = System::PlatformEndianness();
        Endianness expected = (IS_LITTLE_ENDIAN) ? Endianness::LittleEndian : Endianness::BigEndian;
        EXPECT_EQ(expected, actual);
    }
}

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
TEST_FIXTURE(SystemTest, PlatformWindows)
{
    {
        std::string actual   = System::Name();
        std::string expected = "Microsoft Windows";
        EXPECT_EQ(expected, actual);
    }
    {
        std::string actual = System::Variant();
        EXPECT_FALSE(!actual.empty());
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
        std::string expected = "";// PlatformName;
        EXPECT_EQ(expected, actual);
    }
    {
        std::string actual   = System::DemangleName("?foo@OSAL@@YXX@Z");
        std::string expected = "OSAL::foo";
        EXPECT_EQ(expected, actual);
    }
}
#endif // defined(WIN_MSVC)
#if defined(WIN_MINGW)
TEST_FIXTURE(SystemTest, PlatformMinGW)
{
    {
        std::string actual   = System::Name();
        std::string expected = "Microsoft Windows";
        EXPECT_EQ(expected, actual);
    }
    {
        std::string actual = System::Variant();
        EXPECT_FALSE(!actual.empty());
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
        std::string expected = "";//PlatformName;
        EXPECT_EQ(expected, actual);
    }
    {
        std::string actual   = System::DemangleName("_ZN4OSAL3fooEv");
        std::string expected = "OSAL::foo()";
        EXPECT_EQ(expected, actual);
    }
}
#endif // defined(WIN_MINGW)

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
        std::string typeName = System::TypeName(f);

        EXPECT_EQ("void __cdecl(int)", typeName);
    }

    TEST_FIXTURE(SystemTest, LookupClass)
    {
        A a;
        std::string typeName = System::TypeName(a);

        EXPECT_EQ("OSAL::Test::A", typeName);
    }
#endif // defined(WIN_MSVC)

TEST_FIXTURE(SystemTest, GetEnvironment)
{
    const char * ptr = System::GetEnvironment("PATH");
    std::string path(ptr ? ptr : "");

    EXPECT_NE("", path);

    ptr = System::GetEnvironment("SOME_DUMMY");
    std::string dummy(ptr ? ptr : "");

    EXPECT_EQ("", dummy);
}

TEST_FIXTURE(SystemTest, SetEnvironment)
{
    const char * ptr = System::GetEnvironment("SOME_DUMMY");
    std::string dummy(ptr ? ptr : "");
    EXPECT_EQ("", dummy);

    int result = System::SetEnvironment("SOME_DUMMY", "1234");
    EXPECT_EQ(0, result);

    ptr = System::GetEnvironment("SOME_DUMMY");
    dummy = ptr ? ptr : "";
    EXPECT_EQ("1234", dummy);

    result = System::SetEnvironment("SOME_DUMMY", "2345", false);
    EXPECT_EQ(0, result);

    ptr = System::GetEnvironment("SOME_DUMMY");
    dummy = ptr ? ptr : "";
    EXPECT_EQ("1234", dummy);

    result = System::SetEnvironment("SOME_DUMMY", "2345", true);
    EXPECT_EQ(0, result);

    ptr = System::GetEnvironment("SOME_DUMMY");
    dummy = ptr ? ptr : "";
    EXPECT_EQ("2345", dummy);

    result = System::UnSetEnvironment("SOME_DUMMY");
    EXPECT_EQ(0, result);
}

TEST_FIXTURE(SystemTest, UnsSetEnvironmentVariable)
{
    const char * ptr = System::GetEnvironment("SOME_DUMMY");
    std::string dummy(ptr ? ptr : "");
    EXPECT_EQ("", dummy);

    int result = System::SetEnvironment("SOME_DUMMY", "1234");
    EXPECT_EQ(0, result);

    ptr = System::GetEnvironment("SOME_DUMMY");
    dummy = ptr ? ptr : "";
    EXPECT_EQ("1234", dummy);

    result = System::UnSetEnvironment("SOME_DUMMY");
    EXPECT_EQ(0, result);

    ptr = System::GetEnvironment("SOME_DUMMY");
    dummy = ptr ? ptr : "";
    EXPECT_EQ("", dummy);
}

} // TEST_SUITE(osal)

} // namespace Test
} // namespace OSAL