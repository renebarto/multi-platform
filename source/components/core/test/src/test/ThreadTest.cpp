#include <unit-test-c++/UnitTestC++.h>

#include "core/core.h"
#include "core/Thread.h"
#include "core/Util.h"

using namespace std;

namespace Core
{

namespace Test
{

class ThreadTest : public UnitTestCpp::TestFixture
{
public:
    static const int SLEEP = 50;
    static const int TIMEOUT = 50;

    virtual void SetUp();
    virtual void TearDown();

    static bool havethread;
    static Thread * thread;

    static void TestThread(void * arg)
    {
        *(bool *)arg = true;
        Core::Util::Sleep(SLEEP);
    }
};

Thread * ThreadTest::thread;
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
    thread = new Thread(ThreadName);
    EXPECT_FALSE(thread->IsAlive());
    EXPECT_FALSE(thread->IsRunning());
    EXPECT_FALSE(thread->IsFinished());
    EXPECT_EQ(ThreadName, thread->GetName());
    thread->WaitForDeath();
    EXPECT_FALSE(thread->IsAlive());
    EXPECT_FALSE(thread->IsRunning());
    EXPECT_FALSE(thread->IsFinished());
    EXPECT_FALSE(thread->HasDied());
}

TEST_FIXTURE(ThreadTest, ConstructionInitFunctionParam)
{
    bool flag = false;
    thread = new Thread(ThreadName, TestThread, &flag);
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
}

TEST_FIXTURE(ThreadTest, GetSetName)
{
    const string AltName = "Thread";
    thread = new Thread(ThreadName);
    EXPECT_EQ(ThreadName, thread->GetName());
    thread->SetName(AltName);
    EXPECT_EQ(AltName, thread->GetName());
}

TEST_FIXTURE(ThreadTest, Create)
{
    bool flag = false;
    thread = new Thread(ThreadName);
    EXPECT_FALSE(thread->IsAlive());
    EXPECT_FALSE(thread->IsRunning());
    EXPECT_FALSE(thread->IsFinished());
    EXPECT_EQ(ThreadName, thread->GetName());
    dynamic_cast<Thread *>(thread)->CreateThread(TestThread, &flag);
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
}

TEST_FIXTURE(ThreadTest, CreateName)
{
    bool flag = false;
    thread = new Thread(ThreadName);
    EXPECT_FALSE(thread->IsAlive());
    EXPECT_FALSE(thread->IsRunning());
    EXPECT_FALSE(thread->IsFinished());
    EXPECT_EQ(ThreadName, thread->GetName());
    dynamic_cast<Thread *>(thread)->CreateThread(TestThread, &flag);
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
}

TEST_FIXTURE(ThreadTest, Destroy)
{
    bool flag = false;
    thread = new Thread(ThreadName, TestThread, &flag);
    EXPECT_TRUE(thread->IsAlive());
    EXPECT_TRUE(thread->IsRunning());
    EXPECT_FALSE(thread->IsFinished());
    EXPECT_EQ(ThreadName, thread->GetName());
    Core::Util::Sleep(SLEEP);
    EXPECT_TRUE(flag);
    thread->DestroyThread();
    EXPECT_FALSE(thread->IsAlive());
    EXPECT_FALSE(thread->IsRunning());
    EXPECT_FALSE(thread->IsFinished());
    EXPECT_TRUE(thread->HasDied());
    EXPECT_EQ(ThreadName, thread->GetName());
}

} // namespace Test

} // namespace Core
