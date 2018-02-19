#include "unittest-cpp/UnitTestC++.h"

#include "osal/SharedLibrary.h"
#include "osal/TestData.h"

namespace OSAL {
namespace Test {

class SharedLibraryTest : public UnitTestCpp::TestFixture
{
public:
};

TEST_FIXTURE(SharedLibraryTest, ConstructDefault)
{
    const string ExeName = Data::ApplicationName();
    SharedLibrary testlib;
    EXPECT_TRUE(testlib.IsLoaded());
    EXPECT_NULL(testlib.Error());
    string actual = testlib.GetFileName();
    string expected = ExeName;
    EXPECT_EQ(expected, actual);
    actual = testlib.GetFullPath();
    EXPECT_NE(string::npos, actual.find(ExeName));
}

TEST_FIXTURE(SharedLibraryTest, LoadTestLib)
{
    const string TestLibName = "libtestlib.so";
    SharedLibrary testlib(TestLibName);
    EXPECT_TRUE(testlib.IsLoaded());
    EXPECT_NULL(testlib.Error());
    string actual = testlib.GetFileName();
    string expected = TestLibName;
    EXPECT_EQ(expected, actual);
    actual = testlib.GetFullPath();
    EXPECT_NE(string::npos, actual.find(TestLibName));
}

TEST_FIXTURE(SharedLibraryTest, FindSymbolNonExisting)
{
    const string TestLibName = "libtestlib.so";
    const string SymbolName = "Dummy";
    SharedLibrary testlib(TestLibName);
    EXPECT_NULL(testlib.FindSymbol(SymbolName));
}

TEST_FIXTURE(SharedLibraryTest, FindSymbolNonExistingWithFound)
{
    const string TestLibName = "libtestlib.so";
    const string SymbolName = "Dummy";
    SharedLibrary testlib(TestLibName);
    bool found;
    EXPECT_NULL(testlib.FindSymbol(SymbolName, found));
    EXPECT_FALSE(testlib.FindSymbol(SymbolName, found));
}

TEST_FIXTURE(SharedLibraryTest, FindSymbol)
{
    const string TestLibName = "libtestlib.so";
    const string SymbolName = "TheAnswer";
    SharedLibrary testlib(TestLibName);
    EXPECT_NOT_NULL(testlib.FindSymbol(SymbolName));
}

TEST_FIXTURE(SharedLibraryTest, FindSymbolWithFound)
{
    const string TestLibName = "libtestlib.so";
    const string SymbolName = "TheAnswer";
    SharedLibrary testlib(TestLibName);
    bool found;
    EXPECT_NOT_NULL(testlib.FindSymbol(SymbolName, found));
    EXPECT_TRUE(testlib.FindSymbol(SymbolName, found));
}

TEST_FIXTURE(SharedLibraryTest, CallLoadedFunction)
{
    const string TestLibName = "libtestlib.so";
    const string SymbolName = "TheAnswer";
    SharedLibrary testlib(TestLibName);
    using Function = int (void);
    Function * theAnswer = reinterpret_cast<Function *>(testlib.FindSymbol(SymbolName));
    EXPECT_NOT_NULL(theAnswer);
    EXPECT_EQ(int {42}, theAnswer());
}

} // namespace Test
} // namespace OSAL

