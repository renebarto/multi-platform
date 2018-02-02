#include <unittest-c++/UnitTestC++.h>

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

} // TEST_SUITE(osal)

} // namespace Test
} // namespace OSAL
