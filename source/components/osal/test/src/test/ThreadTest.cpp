#include "unittest-cpp/UnitTestC++.h"

#include "osal/Thread.h"

using namespace std;

namespace OSAL {
namespace Thread {
namespace Test {

class ThreadTest : public UnitTestCpp::TestFixture
{
public:
    static bool thisThread;
    static std::thread theThread;
};

bool ThreadTest::thisThread = false;
std::thread ThreadTest::theThread;

void ThreadFunction()
{
    ThreadTest::thisThread = IsThreadSelf(ThreadTest::theThread);
}
TEST_SUITE(osal)
{

TEST_FIXTURE(ThreadTest, ThreadSelf)
{
    thisThread = false;

    EXPECT_FALSE(thisThread);

    theThread = std::thread(ThreadFunction);
    this_thread::sleep_for(std::chrono::milliseconds(500));

    EXPECT_TRUE(thisThread);

    theThread.join();
}

} // TEST_SUITE(osal)

} // namespace Test
} // namespace Thread
} // namespace OSAL
