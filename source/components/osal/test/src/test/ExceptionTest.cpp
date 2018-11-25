#include <unittest-cpp/UnitTestC++.h>

#include "osal/Exception.h"
#include "osal/Strings.h"

using namespace std;

namespace OSAL {
namespace Test {

class ExceptionTest : public UnitTestCpp::TestFixture
{
};

TEST_SUITE(osal)
{

TEST_FIXTURE(ExceptionTest, ExceptionConstructorFunctionFileLine)
{
    OSAL::Exception target("function", "file", 123);
    const std::string expectedWhat = "OSAL::Exception - in function [file:123]";
    const std::string expectedMessage = "";
    const char * what = target.what();
    const std::string actualWhat = what ? what : "";
    const std::string actualMessage = target.GetMessage();
    EXPECT_EQ(expectedWhat, actualWhat);
    EXPECT_EQ(expectedMessage, actualMessage);
}

TEST_FIXTURE(ExceptionTest, ExceptionConstructorFunctionFileLineMessage)
{
    const std::string expectedMessage = "message";
    OSAL::Exception target("function", "file", 123, expectedMessage);
    const std::string expectedWhat = "OSAL::Exception - in function [file:123]: " + expectedMessage;
    const char * what = target.what();
    const std::string actualWhat = what ? what : "";
    const std::string actualMessage = target.GetMessage();
    EXPECT_EQ(expectedWhat, actualWhat);
    EXPECT_EQ(expectedMessage, actualMessage);
}

TEST_FIXTURE(ExceptionTest, ExceptionConstructorFunctionFileLineMessageInner)
{
    OSAL::Exception inner("innerfunction", "innerfile", 456);
    const std::string expectedMessage = "message";
    OSAL::Exception target("function", "file", 123, expectedMessage, inner);
    const std::string expectedWhat = "OSAL::Exception - in function [file:123]: " + expectedMessage +
                                     "\n-->OSAL::Exception - in innerfunction [innerfile:456]";
    const char * what = target.what();
    const std::string actualWhat = what ? what : "";
    const std::string actualMessage = target.GetMessage();
    EXPECT_EQ(expectedWhat, actualWhat);
    EXPECT_EQ(expectedMessage, actualMessage);
}

#if defined(LINUX) || defined(APPLE)

TEST_FIXTURE(ExceptionTest, SystemErrorConstructorFunctionFileLineErrorCode)
{
    OSAL::SystemError target("function", "file", 123, ENOENT);
    const std::string expectedWhat = "OSAL::SystemError - in function [file:123] errno=2 (0x00000002): \"No such file or directory\"";
    const std::string expectedMessage = "";
    const char * what = target.what();
    const std::string actualWhat = what ? what : "";
    const std::string actualMessage = target.GetMessage();
    EXPECT_EQ(expectedWhat, actualWhat);
    EXPECT_EQ(expectedMessage, actualMessage);
}

#else

TEST_FIXTURE(ExceptionTest, SystemErrorConstructorFunctionFileLineErrorCode)
{
    OSAL::SystemError target("function", "file", 123, ENOENT);
    const std::string expectedWhat = "OSAL::SystemError - in function [file:123] errno=2 (0x00000002): \"The system cannot find the file specified.\r\n\"";
    const std::string expectedMessage = "";
    const char * what = target.what();
    const std::string actualWhat = what ? what : "";
    const std::string actualMessage = target.GetMessage();
    EXPECT_EQ(expectedWhat, actualWhat);
    EXPECT_EQ(expectedMessage, actualMessage);
}

#endif

#if defined(LINUX) || defined(APPLE)

TEST_FIXTURE(ExceptionTest, SystemErrorConstructorFunctionFileLineErrorCodeMessage)
{
    const std::string expectedMessage = "message";
    OSAL::SystemError target("function", "file", 123, ENOENT, expectedMessage);
    const std::string expectedWhat = "OSAL::SystemError - in function [file:123] errno=2 (0x00000002): \"No such file or directory\": " + expectedMessage;
    const char * what = target.what();
    const std::string actualWhat = what ? what : "";
    const std::string actualMessage = target.GetMessage();
    EXPECT_EQ(expectedWhat, actualWhat);
    EXPECT_EQ(expectedMessage, actualMessage);
}

#else

TEST_FIXTURE(ExceptionTest, SystemErrorConstructorFunctionFileLineErrorCodeMessage)
{
    const std::string expectedMessage = "message";
    OSAL::SystemError target("function", "file", 123, ENOENT, expectedMessage);
    const std::string expectedWhat = "OSAL::SystemError - in function [file:123] errno=2 (0x00000002): \"The system cannot find the file specified.\r\n\": message";
    const char * what = target.what();
    const std::string actualWhat = what ? what : "";
    const std::string actualMessage = target.GetMessage();
    EXPECT_EQ(expectedWhat, actualWhat);
    EXPECT_EQ(expectedMessage, actualMessage);
}

#endif

TEST_FIXTURE(ExceptionTest, RuntimeErrorConstructorFunctionFileLineMessage)
{
    const std::string expectedMessage = "message";
    OSAL::RuntimeError target("function", "file", 123, expectedMessage);
    const std::string expectedWhat = "OSAL::RuntimeError - in function [file:123]: " + expectedMessage;
    const char * what = target.what();
    const std::string actualWhat = what ? what : "";
    const std::string actualMessage = target.GetMessage();
    EXPECT_EQ(expectedWhat, actualWhat);
    EXPECT_EQ(expectedMessage, actualMessage);
}

TEST_FIXTURE(ExceptionTest, ArgumentExceptionConstructorFunctionFileLineArgument)
{
    const std::string expectedMessage = "";
    const std::string argument = "arg";
    OSAL::ArgumentException target("function", "file", 123, argument);
    const std::string expectedWhat = "OSAL::ArgumentException - in function [file:123]: argument: " + argument;
    const char * what = target.what();
    const std::string actualWhat = what ? what : "";
    const std::string actualMessage = target.GetMessage();
    EXPECT_EQ(expectedWhat, actualWhat);
    EXPECT_EQ(expectedMessage, actualMessage);
}

TEST_FIXTURE(ExceptionTest, ArgumentNullExceptionConstructorFunctionFileLineArgument)
{
    const std::string expectedMessage = "";
    const std::string argument = "arg";
    OSAL::ArgumentNullException target("function", "file", 123, argument);
    const std::string expectedWhat = "OSAL::ArgumentNullException - in function [file:123]: argument null: " + argument;
    const char * what = target.what();
    const std::string actualWhat = what ? what : "";
    const std::string actualMessage = target.GetMessage();
    EXPECT_EQ(expectedWhat, actualWhat);
    EXPECT_EQ(expectedMessage, actualMessage);
}

TEST_FIXTURE(ExceptionTest, ThrowOnErrorNoError)
{
    EXPECT_NOTHROW(OSAL::ThrowOnError("function", "file", 123, 0));
}

#if defined(LINUX) || defined(APPLE)

TEST_FIXTURE(ExceptionTest, ThrowOnErrorNoFileError)
{
    try
    {
        OSAL::ThrowOnError("function", "file", 123, ENOENT);
    }
    catch (std::exception & e)
    {
        SystemError * systemError = dynamic_cast<SystemError *>(&e);
        const std::string expectedWhat = "OSAL::SystemError - in function [file:123] errno=2 (0x00000002): \"No such file or directory\"";
        const std::string expectedMessage = "";
        const char * what = systemError->what();
        const std::string actualWhat = what ? what : "";
        const std::string actualMessage = systemError->GetMessage();
        EXPECT_EQ(expectedWhat, actualWhat);
        EXPECT_EQ(expectedMessage, actualMessage);
    }
}

#else

TEST_FIXTURE(ExceptionTest, ThrowOnErrorNoFileError)
{
    try
    {
        OSAL::ThrowOnError("function", "file", 123, ENOENT);
    }
    catch (std::exception & e)
    {
        SystemError * systemError = dynamic_cast<SystemError *>(&e);
        const std::string expectedWhat = "OSAL::SystemError - in function [file:123] errno=2 (0x00000002): \"The system cannot find the file specified.\r\n\"";
        const std::string expectedMessage = "";
        const char * what = systemError->what();
        const std::string actualWhat = what ? what : "";
        const std::string actualMessage = systemError->GetMessage();
        EXPECT_EQ(expectedWhat, actualWhat);
        EXPECT_EQ(expectedMessage, actualMessage);
    }
}

#endif

} // TEST_SUITE(osal)

} // namespace Test
} // namespace OSAL
