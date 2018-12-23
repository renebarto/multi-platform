#include <unittest-cpp/UnitTestC++.h>

#include "core/DateTime.h"
#include "core/Trace.h"

namespace Core {
namespace Test {

TEST_SUITE(core) {

class TraceTest : public UnitTestCpp::TestFixture
{
public:
    void SetUp() override
    {
        InitializeTrace(&stream);
    }
    void TearDown() override
    {
        DeinitializeTrace();
    }
    std::ostringstream stream;
};

TEST_FIXTURE(TraceTest, TraceWithError)
{
    std::ostringstream stream;
    InitializeTrace(&stream);

    Trace(TraceClass::Error, "Test");
    const DateTime & refTimeStamp = GetTraceHandler().LastTimeStamp();
    std::ostringstream refStream;
    refStream << refTimeStamp << "|Error|Test" << std::endl;
    EXPECT_EQ(refStream.str(), stream.str());
}

TEST_FIXTURE(TraceTest, TraceWithWarning)
{
    std::ostringstream stream;
    InitializeTrace(&stream);

    Trace(TraceClass::Warning , "Test");
    const DateTime & refTimeStamp = GetTraceHandler().LastTimeStamp();
    std::ostringstream refStream;
    refStream << refTimeStamp << "|Warn |Test" << std::endl;
    EXPECT_EQ(refStream.str(), stream.str());
}

TEST_FIXTURE(TraceTest, TraceWithInfo)
{
    std::ostringstream stream;
    InitializeTrace(&stream);

    Trace(TraceClass::Info, "Test");
    const DateTime & refTimeStamp = GetTraceHandler().LastTimeStamp();
    std::ostringstream refStream;
    refStream << refTimeStamp << "|Info |Test" << std::endl;
    EXPECT_EQ(refStream.str(), stream.str());
}

TEST_FIXTURE(TraceTest, TraceWithDebug)
{
    std::ostringstream stream;
    InitializeTrace(&stream);

    Trace(TraceClass::Debug, "Test");
    const DateTime & refTimeStamp = GetTraceHandler().LastTimeStamp();
    std::ostringstream refStream;
    refStream << refTimeStamp << "|Debug|Test" << std::endl;
    EXPECT_EQ(refStream.str(), stream.str());
}

TEST_FIXTURE(TraceTest, TraceWithFunctionEnter)
{
    std::ostringstream stream;
    InitializeTrace(&stream);

    Trace(TraceClass::FunctionEnter, "Test");
    const DateTime & refTimeStamp = GetTraceHandler().LastTimeStamp();
    std::ostringstream refStream;
    refStream << refTimeStamp << "|Enter|Test" << std::endl;
    EXPECT_EQ(refStream.str(), stream.str());
}

TEST_FIXTURE(TraceTest, TraceWithFunctionLeave)
{
    std::ostringstream stream;
    InitializeTrace(&stream);

    Trace(TraceClass::FunctionLeave, "Test");
    const DateTime & refTimeStamp = GetTraceHandler().LastTimeStamp();
    std::ostringstream refStream;
    refStream << refTimeStamp << "|Leave|Test" << std::endl;
    EXPECT_EQ(refStream.str(), stream.str());
}

TEST_FIXTURE(TraceTest, TraceFunctionEnter)
{
    std::ostringstream stream;
    InitializeTrace(&stream);

    TraceFunctionEnter("Test");
    DateTime refTimeStamp = TraceHandler::LastTimeStamp();
    std::ostringstream refStream;
    refStream << refTimeStamp << "|Enter|Test" << std::endl;
    EXPECT_EQ(refStream.str(), stream.str());
}

TEST_FIXTURE(TraceTest, TraceFunctionLeave)
{
    std::ostringstream stream;
    InitializeTrace(&stream);

    TraceFunctionLeave("Test");
    DateTime refTimeStamp = TraceHandler::LastTimeStamp();
    std::ostringstream refStream;
    refStream << refTimeStamp << "|Leave|Test" << std::endl;
    EXPECT_EQ(refStream.str(), stream.str());
}

TEST_FIXTURE(TraceTest, TraceDebug)
{
    std::ostringstream stream;
    InitializeTrace(&stream);

    TraceDebug("Test");
    DateTime refTimeStamp = TraceHandler::LastTimeStamp();
    std::ostringstream refStream;
    refStream << refTimeStamp << "|Debug|Test" << std::endl;
    EXPECT_EQ(refStream.str(), stream.str());
}

TEST_FIXTURE(TraceTest, TraceInfo)
{
    std::ostringstream stream;
    InitializeTrace(&stream);

    TraceInfo("Test");
    DateTime refTimeStamp = TraceHandler::LastTimeStamp();
    std::ostringstream refStream;
    refStream << refTimeStamp << "|Info |Test" << std::endl;
    EXPECT_EQ(refStream.str(), stream.str());
}

TEST_FIXTURE(TraceTest, TraceWarning)
{
    std::ostringstream stream;
    InitializeTrace(&stream);

    TraceWarning("Test");
    DateTime refTimeStamp = TraceHandler::LastTimeStamp();
    std::ostringstream refStream;
    refStream << refTimeStamp << "|Warn |Test" << std::endl;
    EXPECT_EQ(refStream.str(), stream.str());
}

TEST_FIXTURE(TraceTest, TraceError)
{
    TraceError("Test");
    DateTime refTimeStamp = TraceHandler::LastTimeStamp();
    std::ostringstream refStream;
    refStream << refTimeStamp << "|Error|Test" << std::endl;
    EXPECT_EQ(refStream.str(), stream.str());
}

} // TEST_SUITE(core)

} // namespace Test

} // namespace Core
