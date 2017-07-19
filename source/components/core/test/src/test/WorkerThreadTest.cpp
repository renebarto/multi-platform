#include <unit-test-c++/UnitTestC++.h>

#include "core/Core.h"
#include "core/Util.h"
#include "core/WorkerThread.h"

namespace Core {
namespace Test {

TEST_SUITE(core) {

static const int SLEEP = 200;
static const int TIMEOUT = 50;

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
        Core::Util::Sleep(SLEEP);
        return 0;
    }
};

bool MyWorkerThread::isDeleted;

class WorkerThreadTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();

    MyWorkerThread * thread;
};

void WorkerThreadTest::SetUp()
{
    thread = 0;
}

void WorkerThreadTest::TearDown()
{
    delete thread;
}

TEST_FIXTURE(WorkerThreadTest, Construction)
{
    MyWorkerThread thread;
    EXPECT_FALSE(thread.IsRunning());
    EXPECT_FALSE(thread.WaitForBirth(TIMEOUT));
    Core::Util::Sleep(SLEEP);
    EXPECT_FALSE(thread.isFlagged);
    Core::Util::Sleep(SLEEP);
    EXPECT_FALSE(MyWorkerThread::isDeleted);
}

TEST_FIXTURE(WorkerThreadTest, Create)
{
    MyWorkerThread thread;

    thread.Create();
    EXPECT_TRUE(thread.IsRunning());
    EXPECT_TRUE(thread.WaitForBirth(TIMEOUT));
    Core::Util::Sleep(SLEEP);
    EXPECT_TRUE(thread.isFlagged);
    Core::Util::Sleep(SLEEP);
    EXPECT_FALSE(MyWorkerThread::isDeleted);
}

} // TEST_SUITE(core)

} // namespace Test

} // namespace Core
