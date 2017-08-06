#include <unit-test-c++/UnitTestC++.h>

WARNING_PUSH
WARNING_DISABLE(4265)
#include <thread>
WARNING_POP
#include "core/ManualEvent.h"
#include "core/Util.h"

namespace Core {
namespace Test {

TEST_SUITE(core) {

class ManualEventTest : public UnitTestCpp::TestFixture
{
public:
    static const int SLEEP = 10;
    static const int TIMEOUT = 100;

    virtual void SetUp();
    virtual void TearDown();

    static bool _eventPassed;
    static ManualEvent * _event1;
    static ManualEvent * _event2;
    static ManualEvent * _event3;
    static ManualEvent * _event4;

    static void TestThread(void)
    {
        _eventPassed = (_event1 != nullptr) && _event1->Wait(TIMEOUT);
        if (_eventPassed)
        {
            if (_event2 != nullptr)
                _event2->Set();
            Core::Util::Sleep(SLEEP);
            if (_event4 != nullptr)
                _event4->Set();
        }
        if ((_event3 != nullptr) && _event3->Wait(TIMEOUT))
        {
            if (_event2 != nullptr)
                _event2->Reset();
        }
    }
};

bool ManualEventTest::_eventPassed = false;
ManualEvent * ManualEventTest::_event1;
ManualEvent * ManualEventTest::_event2;
ManualEvent * ManualEventTest::_event3;
ManualEvent * ManualEventTest::_event4;

void ManualEventTest::SetUp()
{
    _eventPassed = false;
    _event1 = nullptr;
    _event2 = nullptr;
    _event3 = nullptr;
    _event4 = nullptr;
}

void ManualEventTest::TearDown()
{
}

TEST_FIXTURE(ManualEventTest, Construction)
{
    ManualEvent event1;
    EXPECT_FALSE(event1.GetValue());
}

TEST_FIXTURE(ManualEventTest, SimpleEvent)
{
    ManualEvent target1;
    ManualEvent target2;
    _event1 = &target1;
    _event2 = &target2;

    std::thread thread(TestThread);

    target1.Set();
    Core::Util::Sleep(SLEEP);
    EXPECT_TRUE(target2.Wait(TIMEOUT));
    EXPECT_TRUE(_eventPassed);

    thread.join();
}

TEST_FIXTURE(ManualEventTest, SetEvent)
{
    ManualEvent target1;
    ManualEvent target2;
    ManualEvent target3;
    ManualEvent target4;
    _event1 = &target1;
    _event2 = &target2;
    _event3 = &target3;
    _event4 = &target4;

    std::thread thread(TestThread);

    target1.Set();
    Core::Util::Sleep(SLEEP);
    EXPECT_TRUE(target1.Wait(TIMEOUT));
    EXPECT_TRUE(target2.Wait(TIMEOUT));
    EXPECT_TRUE(target4.Wait(TIMEOUT));

    target3.Set();
    Core::Util::Sleep(SLEEP);
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
    _event1 = &target1;
    _event2 = &target2;

    std::thread thread(TestThread);

    EXPECT_FALSE(target2.Wait(TIMEOUT));

    thread.join();
}

TEST_FIXTURE(ManualEventTest, WaitEvent)
{
    ManualEvent target1;
    ManualEvent target2;
    _event1 = &target1;
    _event2 = &target2;

    std::thread thread(TestThread);

    target1.Set();
    target2.Wait();

    thread.join();
}

} // TEST_SUITE(core)

} // namespace Test

} // namespace Core
