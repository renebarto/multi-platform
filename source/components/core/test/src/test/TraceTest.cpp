#include <unittest-cpp/UnitTestC++.h>

#include "core/DateTime.h"
#include "core/Trace.h"

namespace Core {
namespace Test {

TEST_SUITE(core) {

class TraceTest : public UnitTestCpp::TestFixture
{
};

TEST_FIXTURE(TraceTest, ConstructionDefault)
{
    std::ostringstream stream;
    InitializeTrace(&stream);

    Trace(TraceClass::Error, "Test");
    const DateTime & refTimeStamp = GetTraceHandler().LastTimeStamp();
    std::ostringstream refStream;
    refStream << refTimeStamp << "|Error|Test" << std::endl;
    EXPECT_EQ(refStream.str(), stream.str());

//    OSAL::Console console;
//    refTimeStamp.PrintTo(console);
}

//TEST_FIXTURE(TraceTest, SendWithString)
//{
//    std::ostringstream stream;
//    TraceHandler target(&stream);
//
//    TraceHandler::Send(TraceClass::Error, "Test") << "Hello";
//    DateTime refTimeStamp = DateTime::NowUTC();
//    std::ostringstream refStream;
//    refStream << refTimeStamp << "|Entry|Test" << std::endl;
//    EXPECT_EQ(refStream.str(), stream.str());
//}
//
//TEST_FIXTURE(TraceTest, SendFunctionEntry)
//{
//    std::ostringstream stream;
//    TraceHandler target(&stream);
//
//    TraceHandler::SendFunctionEntry("Test");
//    DateTime refTimeStamp = DateTime::NowUTC();
//    std::ostringstream refStream;
//    refStream << refTimeStamp << "|Entry|Test" << std::endl;
//    EXPECT_EQ(refStream.str(), stream.str());
//}
//
//TEST_FIXTURE(TraceTest, SendFunctionLeave)
//{
//    std::ostringstream stream;
//    TraceHandler target(&stream);
//
//    TraceHandler::SendFunctionLeave("Test");
//    DateTime refTimeStamp = DateTime::NowUTC();
//    std::ostringstream refStream;
//    refStream << refTimeStamp << "|Leave|Test" << std::endl;
//    EXPECT_EQ(refStream.str(), stream.str());
//}
//
//TEST_FIXTURE(TraceTest, SendDebug)
//{
//    std::ostringstream stream;
//    TraceHandler target(&stream);
//
//    TraceHandler::SendDebug("Test");
//    DateTime refTimeStamp = DateTime::NowUTC();
//    std::ostringstream refStream;
//    refStream << refTimeStamp << "|Debug|Test" << std::endl;
//    EXPECT_EQ(refStream.str(), stream.str());
//}
//
//TEST_FIXTURE(TraceTest, SendInfo)
//{
//    std::ostringstream stream;
//    TraceHandler target(&stream);
//
//    TraceHandler::SendInfo("Test");
//    DateTime refTimeStamp = DateTime::NowUTC();
//    std::ostringstream refStream;
//    refStream << refTimeStamp << "|Info|Test" << std::endl;
//    EXPECT_EQ(refStream.str(), stream.str());
//}
//
//TEST_FIXTURE(TraceTest, SendWarning)
//{
//    std::ostringstream stream;
//    TraceHandler target(&stream);
//
//    TraceHandler::SendWarning("Test");
//    DateTime refTimeStamp = DateTime::NowUTC();
//    std::ostringstream refStream;
//    refStream << refTimeStamp << "|Warn|Test" << std::endl;
//    EXPECT_EQ(refStream.str(), stream.str());
//}
//
//TEST_FIXTURE(TraceTest, SendError)
//{
//    std::ostringstream stream;
//    TraceHandler target(&stream);
//
//    TraceHandler::SendError("Test");
//    DateTime refTimeStamp = DateTime::NowUTC();
//    std::ostringstream refStream;
//    refStream << refTimeStamp << "|Error|Test" << std::endl;
//    EXPECT_EQ(refStream.str(), stream.str());
//}

} // TEST_SUITE(core)

} // namespace Test

} // namespace Core
