#include "unittest-cpp/UnitTestC++.h"

#include <cstring>
#include "osal/Process.h"
#include "osal/Path.h"

using namespace std;

namespace OSAL {
namespace Test {

class MyProcess : public Process
{
public:
    MyProcess(const std::string & path, const std::vector<std::string> & args) :
        Process(path, args)
    {
    }
    ProcessArguments & GetArguments() { return Process::_args; }
};

class ProcessTest : public UnitTestCpp::TestFixture
{
public:
};

TEST_FIXTURE(ProcessTest, Construct)
{
    string path = "test";
    vector<string> args { "-abc", "--d=efg", "x", "y", "z"};
    MyProcess process(path, args);
    const char ** argvExpected = new const char *[7];
    argvExpected[0] = "test";
    argvExpected[1] = "-abc";
    argvExpected[2] = "--d=efg";
    argvExpected[3] = "x";
    argvExpected[4] = "y";
    argvExpected[5] = "z";
    argvExpected[6] = 0;
    const char * const * argvActual = process.GetArguments().GetArgv();
    int index = 0;
    ASSERT_NOT_NULL(argvActual);
    while ((argvActual[index] != 0) && (argvExpected[index] != 0))
    {
        EXPECT_EQ(0, strcmp(argvExpected[index], argvActual[index]));
        index++;
    }
    EXPECT_NULL(argvActual[index]);
    delete [] argvExpected;
}

TEST_FIXTURE(ProcessTest, RunEcho)
{
    string path = "echo";
    vector<string> args { "abc" };
    MyProcess process(path, args);
    process.Start();
    process.WaitForExit();
    process.ReadStdOut();
    int result = process.ExitCode();
    EXPECT_EQ(0, result);
    string output = process.StdOut();
    EXPECT_EQ("abc\n", output);
}

TEST_FIXTURE(ProcessTest, RunEchoTestNoInput)
{
    string path = Path::FullPath("echotest");
    vector<string> args { "a", "b", "c"};
    MyProcess process(path, args);
    process.Start();
    process.WriteStdIn();
    process.WaitForExit();
    process.ReadStdOut();
    process.ReadStdErr();
    int result = process.ExitCode();
    EXPECT_EQ(0, result);
    EXPECT_EQ("a\n", process.StdOut());
    EXPECT_EQ("a\n", process.StdErr());
    process.ReadStdOut();
    process.ReadStdErr();
    EXPECT_EQ("b\n", process.StdOut());
    EXPECT_EQ("b\n", process.StdErr());
    process.ReadStdOut();
    process.ReadStdErr();
    EXPECT_EQ("c\n", process.StdOut());
    EXPECT_EQ("c\n", process.StdErr());
}

TEST_FIXTURE(ProcessTest, RunEchoTestWithInput)
{
    string path = Path::FullPath("echotest");
    vector<string> args { "a", "b", "c" };
    MyProcess process(path, args);
    process.WriteInputLine("def");
    process.Start();
    process.WriteStdIn();
    process.WaitForExit();
    process.ReadStdOut();
    process.ReadStdErr();
    int result = process.ExitCode();
    EXPECT_EQ(0, result);
    EXPECT_EQ("a\n", process.StdOut());
    EXPECT_EQ("a\n", process.StdErr());
    process.ReadStdOut();
    process.ReadStdErr();
    EXPECT_EQ("b\n", process.StdOut());
    EXPECT_EQ("b\n", process.StdErr());
    process.ReadStdOut();
    process.ReadStdErr();
    EXPECT_EQ("c\n", process.StdOut());
    EXPECT_EQ("c\n", process.StdErr());
    process.ReadStdOut();
    process.ReadStdErr();
    EXPECT_EQ("def\n", process.StdOut());
    EXPECT_EQ("", process.StdErr());
}

} // namespace Test

} // namespace Core
