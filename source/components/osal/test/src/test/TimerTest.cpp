#include "unittest-cpp/UnitTestC++.h"

#include <thread>
#include "osal/Timer.h"
#include "osal/Time.h"
#include "osal/Util.h"

using namespace std;

namespace OSAL {
namespace Time {
namespace Test {

class TimerTest : public UnitTestCpp::TestFixture
{
public:
    TimerTest()
        : _callbackInvoked()
    {}
    void TimerHandler()
    {
        ++_callbackInvoked;
    }

    int _callbackInvoked;
};

TEST_SUITE(osal)
{

// Implementation for dependable sleep. Due to timers firing, the normal sleep is interrupted,
// and it is advised to use clock_nanosleep with an absolute time to correctly wait without stalling.
#if defined(LINUX) || defined(DARWIN)
void Sleep(int ms)
{
    timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    timespec requested;
    requested.tv_sec = now.tv_sec + (ms / 1000);
    requested.tv_nsec = now.tv_nsec + ((ms % 1000) * 1000000);
    requested.tv_sec += requested.tv_nsec / 1000000000;
    requested.tv_nsec = requested.tv_nsec % 1000000000;

    bool done = false;
    int result {};
    do
    {
        result = ::clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &requested, nullptr);
        if (result == 0)
        {
            done = true;
            continue;
        }
        if (result == EINTR)
            continue;
        return;
    }
    while (!done);

}
#elif defined(WIN_MSVC) || defined(WIN_MINGW)
void Sleep(int ms)
{
    OSAL::Time::usleep(ms * 1000);
}
#endif

TEST_FIXTURE(TimerTest, Construct)
{
    Timer timer;

    EXPECT_FALSE(timer.IsExpired());
    std::string expected = "timer interval = 0 ns, repeat interval = 0 ns,  timed out = 0";
    std::ostringstream stream;
    stream << timer;
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(TimerTest, Start)
{
    Timer timer;

    timer.Start(std::chrono::milliseconds(100), std::bind(&TimerTest::TimerHandler, this));
    EXPECT_FALSE(timer.IsExpired());
    EXPECT_EQ(0, _callbackInvoked);
    std::string expected = "timer interval = 100000000 ns, repeat interval = 0 ns,  timed out = 0";
    std::ostringstream stream;
    stream << timer;
    EXPECT_EQ(expected, stream.str());
    Sleep(1000);
    EXPECT_TRUE(timer.IsExpired());
    EXPECT_EQ(1, _callbackInvoked);
    expected = "timer interval = 100000000 ns, repeat interval = 0 ns,  timed out = 1";
    stream.str("");
    stream << timer;
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(TimerTest, StartRepeat)
{
    Timer timer;

    timer.StartRepeat(std::chrono::milliseconds(100), std::bind(&TimerTest::TimerHandler, this));
    EXPECT_FALSE(timer.IsExpired());
    EXPECT_EQ(0, _callbackInvoked);
    std::string expected = "timer interval = 100000000 ns, repeat interval = 100000000 ns,  timed out = 0";
    std::ostringstream stream;
    stream << timer;
    EXPECT_EQ(expected, stream.str());
    Sleep(1000);
    EXPECT_TRUE(timer.IsExpired());
    EXPECT_EQ(10, _callbackInvoked);
    expected = "timer interval = 100000000 ns, repeat interval = 100000000 ns,  timed out = 1";
    stream.str("");
    stream << timer;
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(TimerTest, StartStop)
{
    Timer timer;

    timer.Start(std::chrono::milliseconds(500), std::bind(&TimerTest::TimerHandler, this));
    EXPECT_FALSE(timer.IsExpired());
    EXPECT_EQ(0, _callbackInvoked);
    Sleep(200);
    timer.Stop();
    EXPECT_FALSE(timer.IsExpired());
    EXPECT_EQ(0, _callbackInvoked);
    std::string expected = "timer interval = 500000000 ns, repeat interval = 0 ns,  timed out = 0";
    std::ostringstream stream;
    stream << timer;
    EXPECT_EQ(expected, stream.str());
    Sleep(500);
    EXPECT_FALSE(timer.IsExpired());
    EXPECT_EQ(0, _callbackInvoked);
    expected = "timer interval = 500000000 ns, repeat interval = 0 ns,  timed out = 0";
    stream.str("");
    stream << timer;
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(TimerTest, MultipleTimers)
{
    Timer timer1;
    Timer timer2;

    timer1.Start(std::chrono::milliseconds(500), std::bind(&TimerTest::TimerHandler, this));
    timer2.Start(std::chrono::milliseconds(1000), std::bind(&TimerTest::TimerHandler, this));
    EXPECT_FALSE(timer1.IsExpired());
    EXPECT_FALSE(timer2.IsExpired());
    EXPECT_EQ(0, _callbackInvoked);
    Sleep(600);
    EXPECT_TRUE(timer1.IsExpired());
    EXPECT_FALSE(timer2.IsExpired());
    EXPECT_EQ(1, _callbackInvoked);
    Sleep(500);
    EXPECT_TRUE(timer1.IsExpired());
    EXPECT_TRUE(timer2.IsExpired());
    EXPECT_EQ(2, _callbackInvoked);
}

TEST_FIXTURE(TimerTest, MultipleTimersRepeat)
{
    Timer timer1;
    Timer timer2;

    timer1.StartRepeat(std::chrono::milliseconds(500), std::bind(&TimerTest::TimerHandler, this));
    timer2.StartRepeat(std::chrono::milliseconds(1000), std::bind(&TimerTest::TimerHandler, this));
    EXPECT_FALSE(timer1.IsExpired());
    EXPECT_FALSE(timer2.IsExpired());
    EXPECT_EQ(0, _callbackInvoked);
    Sleep(600);
    EXPECT_TRUE(timer1.IsExpired());
    EXPECT_FALSE(timer2.IsExpired());
    EXPECT_EQ(1, _callbackInvoked);
    Sleep(500);
    EXPECT_TRUE(timer1.IsExpired());
    EXPECT_TRUE(timer2.IsExpired());
    EXPECT_EQ(3, _callbackInvoked);
}

} // TEST_SUITE(osal)

} // namespace Test
} // namespace Time
} // namespace OSAL
