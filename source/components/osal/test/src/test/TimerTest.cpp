#include "unittest-cpp/UnitTestC++.h"

#include <thread>
#include <sys/poll.h>
#include "osal/Timer.h"
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

TEST_FIXTURE(TimerTest, Construct)
{
    Timer timer;

    EXPECT_FALSE(timer.IsExpired());
}

TEST_FIXTURE(TimerTest, Start)
{
    Timer timer;

    timer.Start(std::chrono::milliseconds(100), std::bind(&TimerTest::TimerHandler, this));
    EXPECT_FALSE(timer.IsExpired());
    EXPECT_EQ(0, _callbackInvoked);
    this_thread::sleep_for(std::chrono::milliseconds(2000));
    EXPECT_TRUE(timer.IsExpired());
    EXPECT_EQ(1, _callbackInvoked);
}

TEST_FIXTURE(TimerTest, StartStop)
{
    Timer timer;

    timer.Start(std::chrono::milliseconds(500), std::bind(&TimerTest::TimerHandler, this));
    EXPECT_FALSE(timer.IsExpired());
    EXPECT_EQ(0, _callbackInvoked);
    this_thread::sleep_for(std::chrono::milliseconds(200));
    timer.Stop();
    EXPECT_FALSE(timer.IsExpired());
    EXPECT_EQ(0, _callbackInvoked);
    this_thread::sleep_for(std::chrono::milliseconds(500));
    EXPECT_FALSE(timer.IsExpired());
    EXPECT_EQ(0, _callbackInvoked);
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
    this_thread::sleep_for(std::chrono::milliseconds(600));
    EXPECT_TRUE(timer1.IsExpired());
    EXPECT_FALSE(timer2.IsExpired());
    EXPECT_EQ(1, _callbackInvoked);
    this_thread::sleep_for(std::chrono::milliseconds(500));
    EXPECT_TRUE(timer1.IsExpired());
    EXPECT_TRUE(timer2.IsExpired());
    EXPECT_EQ(2, _callbackInvoked);
}

} // TEST_SUITE(osal)

} // namespace Test
} // namespace Time
} // namespace OSAL
