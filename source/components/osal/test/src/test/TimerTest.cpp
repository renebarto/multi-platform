#include "unittest-cpp/UnitTestC++.h"

#include <thread>
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
        _callbackInvoked = true;
    }

    bool _callbackInvoked;
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

    timer.StartTimer(std::chrono::milliseconds(100), std::bind(&TimerTest::TimerHandler, this));
    EXPECT_FALSE(timer.IsExpired());
    this_thread::sleep_for(std::chrono::milliseconds(200));
    EXPECT_TRUE(timer.IsExpired());
}

TEST_FIXTURE(TimerTest, StartStop)
{
    Timer timer;

    timer.StartTimer(std::chrono::milliseconds(500), std::bind(&TimerTest::TimerHandler, this));
    EXPECT_FALSE(timer.IsExpired());
    this_thread::sleep_for(std::chrono::milliseconds(200));
    timer.StopTimer();
    EXPECT_FALSE(timer.IsExpired());
    this_thread::sleep_for(std::chrono::milliseconds(500));
    EXPECT_FALSE(timer.IsExpired());
}

} // TEST_SUITE(osal)

} // namespace Test
} // namespace Time
} // namespace OSAL
