#include <unittest-c++/UnitTestC++.h>

#include "core/Core.h"
#include "core/Thread.h"
#include "core/Util.h"

using namespace std;

namespace Core {
namespace Test {

TEST_SUITE(core) {

class ThreadTest : public UnitTestCpp::TestFixture
{
public:
    static const int SLEEP = 50;
    static const int TIMEOUT = 50;

    virtual void SetUp();
    virtual void TearDown();

    static bool _havethread;
    static Thread<bool, void *> * _thread;

    static bool TestThread(void * arg)
    {
        *(bool *)arg = true;
        Core::Util::Sleep(SLEEP);
        return true;
    }
    static bool CrashingThread(void * UNUSED(arg))
    {
        Core::Util::Sleep(SLEEP);
        throw runtime_error("Crash");
    }
};

Thread<bool, void *> * ThreadTest::_thread;
const OSAL::String ThreadName = _("MyThread");


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
    Core::Util::Sleep(SLEEP);
    EXPECT_TRUE(flag);
    Core::Util::Sleep(SLEEP);
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
    const OSAL::String AltName = _("Thread");
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
    Core::Util::Sleep(SLEEP);
    EXPECT_TRUE(flag);
    Core::Util::Sleep(SLEEP);
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
    Core::Util::Sleep(SLEEP);
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
    Core::Util::Sleep(SLEEP);
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
    Util::Sleep(ThreadTest::SLEEP);
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
