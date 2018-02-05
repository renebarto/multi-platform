#include <unittest-cpp/UnitTestC++.h>
#include <bits/sigset.h>

#include "osal/Signal.h"

namespace OSAL {
namespace Test {

class SignalTest : public UnitTestCpp::TestFixture
{
public:
    static void AssertionHandler(bool expression, const char * expressionText, const char * file, int line, const char * func)
    {
        _expression = expression;
        _expressionText = expressionText;
        _file = file;
        _line = line;
        _func = func;
    }
    static bool _expression;
    static const char * _expressionText;
    static const char * _file;
    static int _line;
    static const char * _func;

    void SetUp() override
    {
        _expression = false;
        _expressionText = nullptr;
        _file = nullptr;
        _line = 0;
        _func = nullptr;
    }
};

bool SignalTest::_expression = false;
const char * SignalTest::_expressionText = nullptr;
const char * SignalTest::_file = nullptr;
int SignalTest::_line = 0;
const char * SignalTest::_func = nullptr;

TEST_SUITE(osal)
{

TEST_FIXTURE(SignalTest, Constructor)
{
    Signals::SignalSet set;
    for (int i = static_cast<int>(Signals::Signal::SigHangup); i < static_cast<int>(Signals::Signal::MaxSignal); ++i)
    {
        EXPECT_EQ(0, set.Contains(static_cast<Signals::Signal>(i)));
    }
    EXPECT_TRUE(set.IsEmpty());
}

TEST_FIXTURE(SignalTest, Fill)
{
    Signals::SignalSet set;
    set.Fill();
    for (int i = static_cast<int>(Signals::Signal::SigHangup); i < static_cast<int>(Signals::Signal::MaxSignal); ++i)
    {
        EXPECT_EQ(1, set.Contains(static_cast<Signals::Signal>(i)));
    }
    EXPECT_FALSE(set.IsEmpty());
}

TEST_FIXTURE(SignalTest, Add)
{
    Signals::SignalSet set;
    Signals::Signal signalToSet = Signals::Signal::SigUser1;
    set.Add(signalToSet);
    for (int i = static_cast<int>(Signals::Signal::SigHangup); i < static_cast<int>(Signals::Signal::MaxSignal); ++i)
    {
        if (i == static_cast<int>(signalToSet))
            EXPECT_EQ(1, set.Contains(static_cast<Signals::Signal>(i)));
        else
            EXPECT_EQ(0, set.Contains(static_cast<Signals::Signal>(i)));
    }
    EXPECT_FALSE(set.IsEmpty());
}

TEST_FIXTURE(SignalTest, Remove)
{
    Signals::SignalSet set;
    Signals::Signal signalToClear = Signals::Signal::SigUser1;
    set.Fill();
    set.Remove(signalToClear);
    for (int i = static_cast<int>(Signals::Signal::SigHangup); i < static_cast<int>(Signals::Signal::MaxSignal); ++i)
    {
        if (i == static_cast<int>(signalToClear))
            EXPECT_EQ(0, set.Contains(static_cast<Signals::Signal>(i)));
        else
            EXPECT_EQ(1, set.Contains(static_cast<Signals::Signal>(i)));
    }
    EXPECT_FALSE(set.IsEmpty());
}

TEST_FIXTURE(SignalTest, Clear)
{
    Signals::SignalSet set;
    set.Fill();
    set.Clear();
    for (int i = static_cast<int>(Signals::Signal::SigHangup); i < static_cast<int>(Signals::Signal::MaxSignal); ++i)
    {
        EXPECT_EQ(0, set.Contains(static_cast<Signals::Signal>(i)));
    }
    EXPECT_TRUE(set.IsEmpty());
}

TEST_FIXTURE(SignalTest, And)
{
    Signals::SignalSet result;
    Signals::SignalSet set1;
    Signals::SignalSet set2;
    Signals::Signal signalToSet1 = Signals::Signal::SigUser1;
    Signals::Signal signalToSet2 = Signals::Signal::SigUser2;
    set1.Add(signalToSet1);
    set2.Add(signalToSet1);
    set2.Add(signalToSet2);
    result.And(set1, set2);
    for (int i = static_cast<int>(Signals::Signal::SigHangup); i < static_cast<int>(Signals::Signal::MaxSignal); ++i)
    {
        if (i == static_cast<int>(signalToSet1))
            EXPECT_EQ(1, result.Contains(static_cast<Signals::Signal>(i)));
        else
            EXPECT_EQ(0, result.Contains(static_cast<Signals::Signal>(i)));
    }
}

TEST_FIXTURE(SignalTest, Or)
{
    Signals::SignalSet result;
    Signals::SignalSet set1;
    Signals::SignalSet set2;
    Signals::Signal signalToSet1 = Signals::Signal::SigUser1;
    Signals::Signal signalToSet2 = Signals::Signal::SigUser2;
    set1.Add(signalToSet1);
    set2.Add(signalToSet2);
    result.Or(set1, set2);
    for (int i = static_cast<int>(Signals::Signal::SigHangup); i < static_cast<int>(Signals::Signal::MaxSignal); ++i)
    {
        if ((i == static_cast<int>(signalToSet1)) || (i == static_cast<int>(signalToSet2)))
            EXPECT_EQ(1, result.Contains(static_cast<Signals::Signal>(i)));
        else
            EXPECT_EQ(0, result.Contains(static_cast<Signals::Signal>(i)));
    }
}

TEST_FIXTURE(SignalTest, GetSet)
{
    Signals::SignalSet set;
    Signals::Signal signalToSet1 = Signals::Signal::SigUser1;
    Signals::Signal signalToSet2 = Signals::Signal::SigUser2;
    set.Add(signalToSet1);
    set.Add(signalToSet2);
    auto nativeSet = set.GetSet();
    for (int i = static_cast<int>(Signals::Signal::SigHangup); i < static_cast<int>(Signals::Signal::MaxSignal); ++i)
    {
        if ((i == static_cast<int>(signalToSet1)) || (i == static_cast<int>(signalToSet2)))
            EXPECT_EQ(1, sigismember(&nativeSet, i));
        else
            EXPECT_EQ(0, sigismember(&nativeSet, i));
    }
}

} // TEST_SUITE(osal)

} // namespace Test
} // namespace OSAL
