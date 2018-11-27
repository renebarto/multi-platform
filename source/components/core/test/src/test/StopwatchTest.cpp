#include <unittest-cpp/UnitTestC++.h>
#include <chrono>
#include <thread>

#include "core/Stopwatch.h"

using namespace std;

namespace Core {
namespace Test {

TEST_SUITE(core) {

class StopwatchTest : public UnitTestCpp::TestFixture
{
};

TEST_FIXTURE(StopwatchTest, Construct)
{
    Stopwatch stopwatch;
    EXPECT_EQ(0.0, stopwatch.GetElapsedTime());
    OSAL::Time::timespec expectedStartTime{};
    EXPECT_EQ(expectedStartTime, stopwatch.GetStartTime());
    OSAL::Time::timespec expectedLapTime{};
    EXPECT_EQ(expectedLapTime, stopwatch.GetLapTime());
    OSAL::Time::timespec expectedResolution{};
    // We expect at least some value for resolution, however this value is platform dependent.
    EXPECT_NE(expectedResolution, stopwatch.GetResolution());
    std::basic_ostringstream<char> stream;
    stopwatch.PrintTo(stream);
    EXPECT_EQ("start = 0.000000000 lap = 0.000000000", stream.str());
}

TEST_FIXTURE(StopwatchTest, Start)
{
    Stopwatch stopwatch;
    stopwatch.Start();
    EXPECT_NE(0.0, stopwatch.GetElapsedTime());
    OSAL::Time::timespec expectedStartTime{};
    EXPECT_NE(expectedStartTime, stopwatch.GetStartTime());
    OSAL::Time::timespec expectedLapTime{};
    EXPECT_EQ(expectedLapTime, stopwatch.GetLapTime());
    OSAL::Time::timespec expectedResolution{};
    // We expect at least some value for resolution, however this value is platform dependent.
    EXPECT_NE(expectedResolution, stopwatch.GetResolution());
}

TEST_FIXTURE(StopwatchTest, StartLap)
{
    Stopwatch stopwatch;
    stopwatch.Start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    stopwatch.Lap();
    EXPECT_NEAR(0.1, stopwatch.GetElapsedTime(), 0.01);
    OSAL::Time::timespec expectedStartTime{};
    EXPECT_NE(expectedStartTime, stopwatch.GetStartTime());
    OSAL::Time::timespec expectedLapTime{};
    EXPECT_NE(expectedLapTime, stopwatch.GetLapTime());
    OSAL::Time::timespec expectedResolution{};
    // We expect at least some value for resolution, however this value is platform dependent.
    EXPECT_NE(expectedResolution, stopwatch.GetResolution());
}

} // TEST_SUITE(core)

} // namespace Test

} // namespace Core
