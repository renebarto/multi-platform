#include <unit-test-c++/UnitTestC++.h>

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

    static bool havethread;
    static Thread<bool, void *> * thread;

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
        return false;
    }
};

Thread<bool, void *> * ThreadTest::thread;
const string ThreadName = "MyThread";


void ThreadTest::SetUp()
{
    thread = nullptr;
}

void ThreadTest::TearDown()
{
    delete thread;
}

TEST_FIXTURE(ThreadTest, Construction)
{
    thread = new Thread<bool, void *>(ThreadName);
    EXPECT_FALSE(thread->IsAlive());
    EXPECT_FALSE(thread->IsRunning());
    EXPECT_FALSE(thread->IsFinished());
    EXPECT_EQ(ThreadName, thread->GetName());
    thread->WaitForDeath();
    EXPECT_FALSE(thread->IsAlive());
    EXPECT_FALSE(thread->IsRunning());
    EXPECT_FALSE(thread->IsFinished());
    EXPECT_FALSE(thread->HasDied());
    EXPECT_FALSE(thread->HaveResult());
    EXPECT_THROW(thread->GetResult(), std::future_error);
}

TEST_FIXTURE(ThreadTest, ConstructionInitFunctionParam)
{
    bool flag = false;
    thread = new Thread<bool, void *>(ThreadName, TestThread, &flag);
    EXPECT_TRUE(thread->IsAlive());
    EXPECT_TRUE(thread->IsRunning());
    EXPECT_FALSE(thread->IsFinished());
    EXPECT_EQ(ThreadName, thread->GetName());
    Core::Util::Sleep(SLEEP);
    EXPECT_TRUE(flag);
    Core::Util::Sleep(SLEEP);
    thread->WaitForDeath();
    EXPECT_FALSE(thread->IsAlive());
    EXPECT_FALSE(thread->IsRunning());
    EXPECT_TRUE(thread->IsFinished());
    EXPECT_TRUE(thread->HasDied());
    EXPECT_EQ(ThreadName, thread->GetName());
    EXPECT_TRUE(thread->HaveResult());
    EXPECT_TRUE(thread->GetResult());
}

TEST_FIXTURE(ThreadTest, GetSetName)
{
    const string AltName = "Thread";
    thread = new Thread<bool, void *>(ThreadName);
    EXPECT_EQ(ThreadName, thread->GetName());
    thread->SetName(AltName);
    EXPECT_EQ(AltName, thread->GetName());
    EXPECT_FALSE(thread->HaveResult());
    EXPECT_THROW(thread->GetResult(), std::future_error);
}

TEST_FIXTURE(ThreadTest, Create)
{
    bool flag = false;
    thread = new Thread<bool, void *>(ThreadName);
    EXPECT_FALSE(thread->IsAlive());
    EXPECT_FALSE(thread->IsRunning());
    EXPECT_FALSE(thread->IsFinished());
    EXPECT_EQ(ThreadName, thread->GetName());
    thread->Create(TestThread, &flag);
    EXPECT_TRUE(thread->IsAlive());
    EXPECT_TRUE(thread->IsRunning());
    EXPECT_FALSE(thread->IsFinished());
    EXPECT_EQ(ThreadName, thread->GetName());
    Core::Util::Sleep(SLEEP);
    EXPECT_TRUE(flag);
    Core::Util::Sleep(SLEEP);
    thread->WaitForDeath();
    EXPECT_FALSE(thread->IsAlive());
    EXPECT_FALSE(thread->IsRunning());
    EXPECT_TRUE(thread->IsFinished());
    EXPECT_TRUE(thread->HasDied());
    EXPECT_EQ(ThreadName, thread->GetName());
    EXPECT_TRUE(thread->HaveResult());
    EXPECT_TRUE(thread->GetResult());
}

TEST_FIXTURE(ThreadTest, Destroy)
{
    bool flag = false;
    thread = new Thread<bool, void *>(ThreadName, TestThread, &flag);
    EXPECT_TRUE(thread->IsAlive());
    EXPECT_TRUE(thread->IsRunning());
    EXPECT_FALSE(thread->IsFinished());
    EXPECT_EQ(ThreadName, thread->GetName());
    Core::Util::Sleep(SLEEP);
    EXPECT_TRUE(flag);
    thread->Destroy();
    EXPECT_FALSE(thread->IsAlive());
    EXPECT_FALSE(thread->IsRunning());
    EXPECT_FALSE(thread->IsFinished());
    EXPECT_TRUE(thread->HasDied());
    EXPECT_EQ(ThreadName, thread->GetName());
    EXPECT_TRUE(thread->HaveResult());
    EXPECT_TRUE(thread->GetResult());
}

TEST_FIXTURE(ThreadTest, CrashingThread)
{
    thread = new Thread<bool, void *>(ThreadName, CrashingThread, nullptr);
    EXPECT_TRUE(thread->IsAlive());
    EXPECT_TRUE(thread->IsRunning());
    EXPECT_FALSE(thread->IsFinished());
    EXPECT_EQ(ThreadName, thread->GetName());
    Core::Util::Sleep(SLEEP);
    thread->Destroy();
    EXPECT_FALSE(thread->IsAlive());
    EXPECT_FALSE(thread->IsRunning());
    EXPECT_FALSE(thread->IsFinished());
    EXPECT_TRUE(thread->HasDied());
    EXPECT_EQ(ThreadName, thread->GetName());
    EXPECT_TRUE(thread->HaveResult());
    EXPECT_THROW(thread->GetResult(), std::runtime_error);
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
