#include <unittest-cpp/UnitTestC++.h>

#include "osal/Time.h"
#include "core/Core.h"
#include "core/Thread.h"

using namespace std;

namespace Core {
namespace Test {

TEST_SUITE(core) {

static const auto SLEEP = std::chrono::milliseconds(50);
static const auto TIMEOUT = std::chrono::milliseconds(50);

class ThreadTest : public UnitTestCpp::TestFixture
{
public:

    virtual void SetUp();
    virtual void TearDown();

    static bool _havethread;
    static Thread<bool, void *> * _thread;

    static bool TestThread(void * arg)
    {
        *(bool *)arg = true;
        OSAL::Time::Sleep(SLEEP);
        return true;
    }
    static bool CrashingThread(void * UNUSED(arg))
    {
        OSAL::Time::Sleep(SLEEP);
        throw runtime_error("Crash");
    }
};

Thread<bool, void *> * ThreadTest::_thread;
const std::string ThreadName = "MyThread";


void ThreadTest::SetUp()
{
    _thread = nullptr;
}

void ThreadTest::TearDown()
{
    delete _thread;
}

TEST_FIXTURE(ThreadTest, Construction)
{
    _thread = new Thread<bool, void *>(ThreadName);
    EXPECT_FALSE(_thread->IsAlive());
    EXPECT_FALSE(_thread->IsRunning());
    EXPECT_FALSE(_thread->IsFinished());
    EXPECT_EQ(ThreadName, _thread->GetName());
    _thread->WaitForDeath();
    EXPECT_FALSE(_thread->IsAlive());
    EXPECT_FALSE(_thread->IsRunning());
    EXPECT_FALSE(_thread->IsFinished());
    EXPECT_FALSE(_thread->HasDied());
    EXPECT_FALSE(_thread->HaveResult());
    EXPECT_THROW(_thread->GetResult(), std::future_error);
}

TEST_FIXTURE(ThreadTest, ConstructionInitFunctionParam)
{
    bool flag = false;
    _thread = new Thread<bool, void *>(ThreadName, TestThread, &flag);
    EXPECT_TRUE(_thread->IsAlive());
    EXPECT_TRUE(_thread->IsRunning());
    EXPECT_FALSE(_thread->IsFinished());
    EXPECT_EQ(ThreadName, _thread->GetName());
    OSAL::Time::Sleep(SLEEP);
    EXPECT_TRUE(flag);
    OSAL::Time::Sleep(SLEEP);
    _thread->WaitForDeath();
    EXPECT_FALSE(_thread->IsAlive());
    EXPECT_FALSE(_thread->IsRunning());
    EXPECT_TRUE(_thread->IsFinished());
    EXPECT_TRUE(_thread->HasDied());
    EXPECT_EQ(ThreadName, _thread->GetName());
    EXPECT_TRUE(_thread->HaveResult());
    EXPECT_TRUE(_thread->GetResult());
}

TEST_FIXTURE(ThreadTest, GetSetName)
{
    const std::string AltName = "Thread";
    _thread = new Thread<bool, void *>(ThreadName);
    EXPECT_EQ(ThreadName, _thread->GetName());
    _thread->SetName(AltName);
    EXPECT_EQ(AltName, _thread->GetName());
    EXPECT_FALSE(_thread->HaveResult());
    EXPECT_THROW(_thread->GetResult(), std::future_error);
}

TEST_FIXTURE(ThreadTest, Create)
{
    bool flag = false;
    _thread = new Thread<bool, void *>(ThreadName);
    EXPECT_FALSE(_thread->IsAlive());
    EXPECT_FALSE(_thread->IsRunning());
    EXPECT_FALSE(_thread->IsFinished());
    EXPECT_EQ(ThreadName, _thread->GetName());
    _thread->Create(TestThread, &flag);
    EXPECT_TRUE(_thread->IsAlive());
    EXPECT_TRUE(_thread->IsRunning());
    EXPECT_FALSE(_thread->IsFinished());
    EXPECT_EQ(ThreadName, _thread->GetName());
    OSAL::Time::Sleep(SLEEP);
    EXPECT_TRUE(flag);
    OSAL::Time::Sleep(SLEEP);
    _thread->WaitForDeath();
    EXPECT_FALSE(_thread->IsAlive());
    EXPECT_FALSE(_thread->IsRunning());
    EXPECT_TRUE(_thread->IsFinished());
    EXPECT_TRUE(_thread->HasDied());
    EXPECT_EQ(ThreadName, _thread->GetName());
    EXPECT_TRUE(_thread->HaveResult());
    EXPECT_TRUE(_thread->GetResult());
}

TEST_FIXTURE(ThreadTest, Destroy)
{
    bool flag = false;
    _thread = new Thread<bool, void *>(ThreadName, TestThread, &flag);
    EXPECT_TRUE(_thread->IsAlive());
    EXPECT_TRUE(_thread->IsRunning());
    EXPECT_FALSE(_thread->IsFinished());
    EXPECT_EQ(ThreadName, _thread->GetName());
    OSAL::Time::Sleep(SLEEP);
    EXPECT_TRUE(flag);
    _thread->Destroy();
    EXPECT_FALSE(_thread->IsAlive());
    EXPECT_FALSE(_thread->IsRunning());
    EXPECT_FALSE(_thread->IsFinished());
    EXPECT_TRUE(_thread->HasDied());
    EXPECT_EQ(ThreadName, _thread->GetName());
    EXPECT_TRUE(_thread->HaveResult());
    EXPECT_TRUE(_thread->GetResult());
}

TEST_FIXTURE(ThreadTest, CrashingThread)
{
    _thread = new Thread<bool, void *>(ThreadName, CrashingThread, nullptr);
    EXPECT_TRUE(_thread->IsAlive());
    EXPECT_TRUE(_thread->IsRunning());
    EXPECT_FALSE(_thread->IsFinished());
    EXPECT_EQ(ThreadName, _thread->GetName());
    OSAL::Time::Sleep(SLEEP);
    _thread->Destroy();
    EXPECT_FALSE(_thread->IsAlive());
    EXPECT_FALSE(_thread->IsRunning());
    EXPECT_FALSE(_thread->IsFinished());
    EXPECT_TRUE(_thread->HasDied());
    EXPECT_EQ(ThreadName, _thread->GetName());
    EXPECT_TRUE(_thread->HaveResult());
    EXPECT_THROW(_thread->GetResult(), std::runtime_error);
}

int IntThread(void * UNUSED(arg))
{
    OSAL::Time::Sleep(SLEEP);
    return 42;
}

TEST_FIXTURE(ThreadTest, ThreadReturningInt)
{
    Thread<int, void *> thread(ThreadName);
    EXPECT_FALSE(thread.IsAlive());
    EXPECT_FALSE(thread.IsRunning());
    EXPECT_FALSE(thread.IsFinished());
    EXPECT_FALSE(thread.HasDied());
    EXPECT_EQ(ThreadName, thread.GetName());
    EXPECT_FALSE(thread.HaveResult());
    EXPECT_THROW(thread.GetResult(), std::future_error);

    thread.Create(IntThread, nullptr);

    EXPECT_TRUE(thread.IsAlive());
    EXPECT_TRUE(thread.IsRunning());
    EXPECT_FALSE(thread.IsFinished());
    EXPECT_EQ(ThreadName, thread.GetName());

    thread.Destroy();
    EXPECT_FALSE(thread.IsAlive());
    EXPECT_FALSE(thread.IsRunning());
    EXPECT_FALSE(thread.IsFinished());
    EXPECT_TRUE(thread.HasDied());
    EXPECT_EQ(ThreadName, thread.GetName());
    EXPECT_TRUE(thread.HaveResult());
    EXPECT_EQ(42, thread.GetResult());
}

} // TEST_SUITE(core)

} // namespace Test

} // namespace Core
