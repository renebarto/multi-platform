#include <unittest-cpp/UnitTestC++.h>

#include <thread>
#include "osal/Time.h"
#include "osal/ManualEvent.h"

namespace OSAL
{

namespace Test
{

class ManualEventTest : public UnitTestCpp::TestFixture
{
public:
    static const std::chrono::nanoseconds SLEEP;
    static const std::chrono::nanoseconds TIMEOUT;

    virtual void SetUp();
    virtual void TearDown();

    static bool eventPassed;
    static ManualEvent * event1;
    static ManualEvent * event2;
    static ManualEvent * event3;
    static ManualEvent * event4;

    static void TestThread(void)
    {
        eventPassed = (event1 != nullptr) && event1->Wait(TIMEOUT);
        if (eventPassed)
        {
            if (event2 != nullptr)
                event2->Set();
            OSAL::Time::Sleep(SLEEP);
            if (event4 != nullptr)
                event4->Set();
        }
        if ((event3 != nullptr) && event3->Wait(TIMEOUT))
        {
            if (event2 != nullptr)
                event2->Reset();
        }
    }
};

bool ManualEventTest::eventPassed = false;
ManualEvent * ManualEventTest::event1;
ManualEvent * ManualEventTest::event2;
ManualEvent * ManualEventTest::event3;
ManualEvent * ManualEventTest::event4;
const std::chrono::nanoseconds ManualEventTest::SLEEP = std::chrono::milliseconds(10);
const std::chrono::nanoseconds ManualEventTest::TIMEOUT = std::chrono::milliseconds(100);

void ManualEventTest::SetUp()
{
    eventPassed = false;
    event1 = nullptr;
    event2 = nullptr;
    event3 = nullptr;
    event4 = nullptr;
}

void ManualEventTest::TearDown()
{
}

TEST_FIXTURE(ManualEventTest, Construction)
{
    ManualEvent event;
    EXPECT_FALSE(event.GetValue());
}

TEST_FIXTURE(ManualEventTest, SimpleEvent)
{
    ManualEvent target1;
    ManualEvent target2;
    event1 = &target1;
    event2 = &target2;

    std::thread thread(TestThread);

    target1.Set();
    OSAL::Time::Sleep(SLEEP);
    EXPECT_TRUE(target2.Wait(TIMEOUT));
    EXPECT_TRUE(eventPassed);

    thread.join();
}

TEST_FIXTURE(ManualEventTest, SetEvent)
{
    ManualEvent target1;
    ManualEvent target2;
    ManualEvent target3;
    ManualEvent target4;
    event1 = &target1;
    event2 = &target2;
    event3 = &target3;
    event4 = &target4;

    std::thread thread(TestThread);

    target1.Set();
    OSAL::Time::Sleep(SLEEP);
    EXPECT_TRUE(target1.Wait(TIMEOUT));
    EXPECT_TRUE(target2.Wait(TIMEOUT));
    EXPECT_TRUE(target4.Wait(TIMEOUT));

    target3.Set();
    OSAL::Time::Sleep(SLEEP);
    EXPECT_TRUE(target1.Wait(TIMEOUT));
    EXPECT_FALSE(target2.Wait(TIMEOUT));
    EXPECT_TRUE(target3.Wait(TIMEOUT));
    EXPECT_TRUE(target4.Wait(TIMEOUT));

    thread.join();
}

TEST_FIXTURE(ManualEventTest, WaitEventTimeout)
{
    ManualEvent target1;
    ManualEvent target2;
    event1 = &target1;
    event2 = &target2;

    std::thread thread(TestThread);

    EXPECT_FALSE(target2.Wait(TIMEOUT));

    thread.join();
}

TEST_FIXTURE(ManualEventTest, WaitEvent)
{
    ManualEvent target1;
    ManualEvent target2;
    event1 = &target1;
    event2 = &target2;

    std::thread thread(TestThread);

    target1.Set();
    target2.Wait();

    thread.join();
}

} // namespace Test

} // namespace OSAL
