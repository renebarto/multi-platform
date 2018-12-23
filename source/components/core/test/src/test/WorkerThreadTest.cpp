#include <unittest-cpp/UnitTestC++.h>

#include "osal/Time.h"
#include "core/Core.h"
#include "core/WorkerThread.h"

namespace Core {
namespace Test {

TEST_SUITE(core) {

static auto SLEEP = std::chrono::milliseconds(200);
static auto TIMEOUT = std::chrono::milliseconds(50);

class MyWorkerThread: public WorkerThread<int>
{
public:
    static bool isDeleted;
    bool isFlagged;

    MyWorkerThread():
        WorkerThread<int>("MyWorkerThread"),
        isFlagged(false)
    {
        MyWorkerThread::isDeleted = false;
    }
    ~MyWorkerThread()
    {
        isDeleted = true;
    }
    int Thread()
    {
        isFlagged = true;
        OSAL::Time::Sleep(SLEEP);
        return 0;
    }
};

bool MyWorkerThread::isDeleted;

class WorkerThreadTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void WorkerThreadTest::SetUp()
{
}

void WorkerThreadTest::TearDown()
{
}

TEST_FIXTURE(WorkerThreadTest, Construction)
{
    MyWorkerThread thread;
    EXPECT_FALSE(thread.IsRunning());
    EXPECT_FALSE(thread.WaitForBirth(TIMEOUT));
    OSAL::Time::Sleep(SLEEP);
    EXPECT_FALSE(thread.isFlagged);
    OSAL::Time::Sleep(SLEEP);
    EXPECT_FALSE(MyWorkerThread::isDeleted);
}

TEST_FIXTURE(WorkerThreadTest, Create)
{
    MyWorkerThread thread;

    thread.Create();
    EXPECT_TRUE(thread.IsRunning());
    EXPECT_TRUE(thread.WaitForBirth(TIMEOUT));
    OSAL::Time::Sleep(SLEEP);
    EXPECT_TRUE(thread.isFlagged);
    OSAL::Time::Sleep(SLEEP);
    EXPECT_FALSE(MyWorkerThread::isDeleted);
}

} // TEST_SUITE(core)

} // namespace Test

} // namespace Core
