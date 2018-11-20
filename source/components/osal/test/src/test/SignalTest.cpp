#include "unittest-cpp/UnitTestC++.h"

#include "osal/Signal.h"

using namespace std;

namespace OSAL {
namespace Signal {
namespace Test {

class SignalTest : public UnitTestCpp::TestFixture
{
public:
    static int signalValue;
    static bool hadSignal;
};

int SignalTest::signalValue = -1;
bool SignalTest::hadSignal = false;

void SignalHandler(int signal)
{
    SignalTest::signalValue = signal;
    SignalTest::hadSignal = true;
};

TEST_SUITE(osal)
{

TEST_FIXTURE(SignalTest, GenerateSigAbort)
{
    signalValue = -1;
    hadSignal = false;

    signal(SIGABRT, SignalHandler);

    EXPECT_EQ(-1, signalValue);
    EXPECT_FALSE(hadSignal);

    raise(SIGABRT);

    EXPECT_EQ(SIGABRT, signalValue);
    EXPECT_TRUE(hadSignal);

    signal(SIGABRT, SIG_DFL);
}

TEST_FIXTURE(SignalTest, GenerateSigIll)
{
    signalValue = -1;
    hadSignal = false;

    signal(SIGILL, SignalHandler);

    EXPECT_EQ(-1, signalValue);
    EXPECT_FALSE(hadSignal);

    raise(SIGILL);

    EXPECT_EQ(SIGILL, signalValue);
    EXPECT_TRUE(hadSignal);

    signal(SIGILL, SIG_DFL);
}

TEST_FIXTURE(SignalTest, GenerateSigInt)
{
    signalValue = -1;
    hadSignal = false;

    signal(SIGINT, SignalHandler);

    EXPECT_EQ(-1, signalValue);
    EXPECT_FALSE(hadSignal);

    raise(SIGINT);

    EXPECT_EQ(SIGINT, signalValue);
    EXPECT_TRUE(hadSignal);

    signal(SIGINT, SIG_DFL);
}

TEST_FIXTURE(SignalTest, GenerateSigSegv)
{
    signalValue = -1;
    hadSignal = false;

    signal(SIGSEGV, SignalHandler);

    EXPECT_EQ(-1, signalValue);
    EXPECT_FALSE(hadSignal);

    raise(SIGSEGV);

    EXPECT_EQ(SIGSEGV, signalValue);
    EXPECT_TRUE(hadSignal);

    signal(SIGSEGV, SIG_DFL);
}

TEST_FIXTURE(SignalTest, GenerateSigTerm)
{
    signalValue = -1;
    hadSignal = false;

    signal(SIGTERM, SignalHandler);

    EXPECT_EQ(-1, signalValue);
    EXPECT_FALSE(hadSignal);

    raise(SIGTERM);

    EXPECT_EQ(SIGTERM, signalValue);
    EXPECT_TRUE(hadSignal);

    signal(SIGTERM, SIG_DFL);
}

} // TEST_SUITE(osal)

} // namespace Test
} // namespace Signal
} // namespace OSAL
