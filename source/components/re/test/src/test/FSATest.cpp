#include "unittest-cpp/UnitTestC++.h"

#include <fstream>
#include "re/FSA.h"
#include "re/CharSet.h"
#include "re/TestData.h"

using namespace std;

namespace RE {
namespace Test {

class FSATest : public UnitTestCpp::TestFixture
{
};

TEST_SUITE(re) {

static FSARuleSet<int> EmptyRules
{

};
static FSARuleSet<int> Rules
{
    FSARule<int>::Create('a', 0, 1),
};

static FSARuleSet<int> RulesCharSet
{
    FSARule<int>::Create(CharSet::Range('a', 'c'), 0, 1),
    FSARule<int>::Create(CharSet::Range('a', 'c'), 1, 2),
};

TEST_FIXTURE(FSATest, ConstructRules)
{
    FSA<int> finiteStateAutomaton(EmptyRules);
    EXPECT_EQ(0, finiteStateAutomaton.CurrentState());
    EXPECT_TRUE(finiteStateAutomaton.IsFinalState());
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(FSATest, ConstructRulesInitialState)
{
    FSA<int> finiteStateAutomaton(Rules, 0);
    EXPECT_EQ(0, finiteStateAutomaton.CurrentState());
    EXPECT_TRUE(finiteStateAutomaton.IsFinalState());
    // Initial state: 0, Final state: 0, Rules 0->1
    // We cannot get from 1 to final state
    EXPECT_FALSE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(FSATest, ConstructRulesInitialAndFinalState)
{
    FSA<int> finiteStateAutomaton(Rules, 0, 1);
    EXPECT_EQ(0, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    // Initial state: 0, Final state: 1, Rules 0->1
    // We can get to final state from all states
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(FSATest, HandleInputEmptyRules)
{
    FSA<int> finiteStateAutomaton(EmptyRules, 123);
    EXPECT_FALSE(finiteStateAutomaton.HandleInput('a'));
    EXPECT_EQ(123, finiteStateAutomaton.CurrentState());
    EXPECT_TRUE(finiteStateAutomaton.IsFinalState());
    // Initial state: 123, Final state: 123, Rules none
    // We can get from 123 to final state
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(FSATest, HandleInput)
{
    FSA<int> finiteStateAutomaton(Rules, 0, 1);
    EXPECT_TRUE(finiteStateAutomaton.HandleInput('a'));
    EXPECT_EQ(1, finiteStateAutomaton.CurrentState());
    EXPECT_TRUE(finiteStateAutomaton.IsFinalState());
    // Initial state: 0, Final state: 1, Rules 0->1
    // We can get to final state from all states
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(FSATest, Reset)
{
    FSA<int> finiteStateAutomaton(Rules, 0, 1);
    EXPECT_TRUE(finiteStateAutomaton.HandleInput('a'));
    EXPECT_EQ(1, finiteStateAutomaton.CurrentState());
    EXPECT_TRUE(finiteStateAutomaton.IsFinalState());
    EXPECT_TRUE(finiteStateAutomaton.Validate());
    finiteStateAutomaton.Reset();
    EXPECT_EQ(0, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(FSATest, HandleInputInvalidState)
{
    FSA<int> finiteStateAutomaton(Rules, 2, 1);
    EXPECT_FALSE(finiteStateAutomaton.HandleInput('a'));
    EXPECT_EQ(2, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    // Initial state: 2, Final state: 1, Rules 0->1
    // We cannot get from 2 to final state
    EXPECT_FALSE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(FSATest, HandleInputInvalidInput)
{
    FSA<int> finiteStateAutomaton(Rules, 0, 1);
    EXPECT_FALSE(finiteStateAutomaton.HandleInput('b'));
    EXPECT_EQ(0, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(FSATest, HandleInputCharSet)
{
    FSA<int> finiteStateAutomaton(RulesCharSet, 0, 2);
    EXPECT_TRUE(finiteStateAutomaton.HandleInput('a'));
    EXPECT_EQ(1, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    EXPECT_TRUE(finiteStateAutomaton.HandleInput('b'));
    EXPECT_EQ(2, finiteStateAutomaton.CurrentState());
    EXPECT_TRUE(finiteStateAutomaton.IsFinalState());
    EXPECT_FALSE(finiteStateAutomaton.HandleInput('c'));
    EXPECT_EQ(2, finiteStateAutomaton.CurrentState());
    EXPECT_TRUE(finiteStateAutomaton.IsFinalState());
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

static FSARuleSet<int> RulesCharSetComplex
{
    FSARule<int>::Create('a', 0, 1),
    FSARule<int>::Create('b', 0, 3),
    FSARule<int>::Create('c', 1, 2),
    FSARule<int>::Create('d', 3, 2),
    FSARule<int>::Create('e', 3, 0),
};

TEST_FIXTURE(FSATest, HandleInputCharSetComplex)
{
    FSA<int> finiteStateAutomaton(RulesCharSetComplex, 0, 2);
    EXPECT_TRUE(finiteStateAutomaton.HandleInput('a'));
    EXPECT_EQ(1, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    EXPECT_TRUE(finiteStateAutomaton.HandleInput('c'));
    EXPECT_EQ(2, finiteStateAutomaton.CurrentState());
    EXPECT_TRUE(finiteStateAutomaton.IsFinalState());
    finiteStateAutomaton.Reset();
    EXPECT_FALSE(finiteStateAutomaton.HandleInput('c'));
    EXPECT_EQ(0, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    EXPECT_TRUE(finiteStateAutomaton.HandleInput('b'));
    EXPECT_EQ(3, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    EXPECT_TRUE(finiteStateAutomaton.HandleInput('d'));
    EXPECT_EQ(2, finiteStateAutomaton.CurrentState());
    EXPECT_TRUE(finiteStateAutomaton.IsFinalState());
    finiteStateAutomaton.Reset();
    EXPECT_TRUE(finiteStateAutomaton.HandleInput('b'));
    EXPECT_EQ(3, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    EXPECT_TRUE(finiteStateAutomaton.HandleInput('e'));
    EXPECT_EQ(0, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    // Initial state: 0, Final state: 2, Rules 0->1, 0->3, 1->2, 3->2, 3->0
    // We can get to final state from all states
    // The loop does not hang the check
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

static FSARuleSet<int> RulesCharSetComplexIncorrect
{
    FSARule<int>::Create('a', 0, 1),
    FSARule<int>::Create('b', 0, 3),
    FSARule<int>::Create('c', 0, 4),
    FSARule<int>::Create('d', 1, 2),
    FSARule<int>::Create('e', 3, 2),
};

TEST_FIXTURE(FSATest, HandleInputCharSetComplexIncorrect)
{
    FSA<int> finiteStateAutomaton(RulesCharSetComplexIncorrect, 0, 2);
    // Initial state: 0, Final state: 2, Rules 0->1, 0->3, 1->2, 3->2, 3->4
    // We cannot get from 4 to final state
    EXPECT_FALSE(finiteStateAutomaton.Validate());
}

} // TEST_SUITE(re)

} // namespace Test
} // namespace RE
