#include "unittest-cpp/UnitTestC++.h"

#include <cstring>
#include "osal/ProcessArguments.h"
//#include "Core/Util.h"

using namespace std;

namespace OSAL {
namespace Test {

class ProcessArgumentsTest : public UnitTestCpp::TestFixture
{
public:
};

TEST_FIXTURE(ProcessArgumentsTest, ConstructString)
{
    string programPath = "test";
    string commandLine = "-abc --d=efg x y z";
    ProcessArguments args(programPath, commandLine);
    const char ** argvExpected = new const char *[7];
    argvExpected[0] = "test";
    argvExpected[1] = "-abc";
    argvExpected[2] = "--d=efg";
    argvExpected[3] = "x";
    argvExpected[4] = "y";
    argvExpected[5] = "z";
    argvExpected[6] = 0;
    const char * const * argvActual = args.GetArgv();
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

TEST_FIXTURE(ProcessArgumentsTest, ConstructVector)
{
    string programPath = "test";
    vector<string> commandLine;
    commandLine.push_back("-abc");
    commandLine.push_back("--d=efg");
    commandLine.push_back("x");
    commandLine.push_back("y");
    commandLine.push_back("z");
    ProcessArguments args(programPath, commandLine);
    const char ** argvExpected = new const char *[7];
    argvExpected[0] = "test";
    argvExpected[1] = "-abc";
    argvExpected[2] = "--d=efg";
    argvExpected[3] = "x";
    argvExpected[4] = "y";
    argvExpected[5] = "z";
    argvExpected[6] = 0;
    const char * const * argvActual = args.GetArgv();
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

} // namespace Test
} // namespace OSAL
