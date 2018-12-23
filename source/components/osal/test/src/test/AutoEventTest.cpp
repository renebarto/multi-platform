#include <unittest-cpp/UnitTestC++.h>

#include <thread>
#include "osal/Time.h"
#include "osal/AutoEvent.h"
#include "core/Trace.h"

using namespace std;

namespace OSAL
{

namespace Test
{

class AutoEventTest : public UnitTestCpp::TestFixture
{
public:
    static const std::chrono::nanoseconds SLEEP;
    static const std::chrono::nanoseconds TIMEOUT;

    virtual void SetUp();
    virtual void TearDown();

    static bool eventPassed;
    static AutoEventHandle eventHandle1;
    static AutoEventHandle eventHandle2;

    static void TestThread(void)
    {
        Core::TraceDebug("Started");

        AutoEvent event1;
        AutoEvent event2;
        if (eventHandle1 != 0)
            event1 = eventHandle1;
        if (eventHandle2 != 0)
            event2 = eventHandle2;

        ostringstream stream;
        stream << "Event 1: " << event1.Get() << endl;
        stream << "Event 2: " << event2.Get();
        Core::TraceDebug("TestThread", stream.str());

        eventPassed = event1.Wait(TIMEOUT);

        stream.str("");
        stream << "Event 1: " << event1.Get() << endl;
        stream << "Event 2: " << event2.Get();
        Core::TraceDebug("TestThread", stream.str());

        if (eventPassed)
        {
            event2.Set();
        }

        stream.str("");
        stream << "Event 1: " << event1.Get() << endl;
        stream << "Event 2: " << event2.Get();
        Core::TraceDebug("TestThread", stream.str());
        Core::TraceDebug("TestThread", "Ended");
    }
};

bool AutoEventTest::eventPassed = false;
AutoEventHandle AutoEventTest::eventHandle1;
AutoEventHandle AutoEventTest::eventHandle2;
const std::chrono::nanoseconds AutoEventTest::SLEEP = std::chrono::milliseconds(10);
const std::chrono::nanoseconds AutoEventTest::TIMEOUT = std::chrono::milliseconds(100);

void AutoEventTest::SetUp()
{
    eventPassed = false;
    eventHandle1 = 0;
    eventHandle2 = 0;
}

void AutoEventTest::TearDown()
{
}

TEST_FIXTURE(AutoEventTest, Construction)
{
    AutoEvent event1;
    EXPECT_TRUE(((AutoEventHandle)event1) != 0);
}

TEST_FIXTURE(AutoEventTest, ConstructionCopy)
{
    AutoEvent event1;
    AutoEvent event2(event1);
    EXPECT_EQ((AutoEventHandle)event1, (AutoEventHandle)event2);
}

TEST_FIXTURE(AutoEventTest, ConstructionInit)
{
    AutoEvent event1;
    AutoEventHandle eventHandle = event1;
    AutoEvent event2(eventHandle);
    EXPECT_EQ(eventHandle, (AutoEventHandle)event2);
}

TEST_FIXTURE(AutoEventTest, AssignmentEvent)
{
    AutoEvent event1;
    AutoEvent event2;
    event2 = event1;
    EXPECT_EQ((AutoEventHandle)event1, (AutoEventHandle)event2);
}

TEST_FIXTURE(AutoEventTest, AssignmentHandle)
{
    AutoEvent event1;
    AutoEventHandle eventHandle = event1;
    AutoEvent event2;
    event2 = eventHandle;
    EXPECT_EQ(eventHandle, (AutoEventHandle)event2);
}

TEST_FIXTURE(AutoEventTest, SetEvent)
{
    Core::TraceDebug("SetEvent start");

    AutoEvent event1;
    AutoEvent event2;
    eventHandle1 = (AutoEventHandle)event1;
    eventHandle2 = (AutoEventHandle)event2;

    std::thread thread(TestThread);

    ostringstream stream;
    stream << "Event 1: " << event1.Get() << endl;
    stream << "Event 2: " << event2.Get();
    Core::TraceDebug("TestThread", stream.str());

    event1.Set();

    stream.str("");
    stream << "Event 1: " << event1.Get() << endl;
    stream << "Event 2: " << event2.Get();
    Core::TraceDebug("TestThread", stream.str());

    OSAL::Time::Sleep(SLEEP);
    EXPECT_TRUE(event2.Wait(TIMEOUT));
    EXPECT_TRUE(eventPassed);

        stream.str("");
        stream << "Event 1: " << event1.Get() << endl;
        stream << "Event 2: " << event2.Get();
        Core::TraceDebug("TestThread", stream.str());

    thread.join();

    Core::TraceDebug("SetEvent end");
}

TEST_FIXTURE(AutoEventTest, WaitEventTimeout)
{
    Core::TraceDebug("WaitEventTimeout start");

    AutoEvent event1;
    AutoEvent event2;
    eventHandle1 = (AutoEventHandle)event1;
    eventHandle2 = (AutoEventHandle)event2;

    std::thread thread(TestThread);

    ostringstream stream;
    stream << "Event 1: " << event1.Get() << endl;
    stream << "Event 2: " << event2.Get();
    Core::TraceDebug("TestThread", stream.str());

    EXPECT_FALSE(event2.Wait(TIMEOUT));
    EXPECT_FALSE(eventPassed);

    stream.str("");
    stream << "Event 1: " << event1.Get() << endl;
    stream << "Event 2: " << event2.Get();
    Core::TraceDebug("TestThread", stream.str());

    thread.join();

    Core::TraceDebug("WaitEventTimeout end");
}

TEST_FIXTURE(AutoEventTest, WaitEvent)
{
    Core::TraceDebug("WaitEvent start");

    AutoEvent event1;
    AutoEvent event2;
    eventHandle1 = (AutoEventHandle)event1;
    eventHandle2 = (AutoEventHandle)event2;

    std::thread thread(TestThread);

    event1.Set();
    event2.Wait();
    EXPECT_TRUE(eventPassed);

    thread.join();

    Core::TraceDebug("WaitEvent start");
}

} // namespace Test

} // namespace OSAL
