#include "unittest-cpp/UnitTestC++.h"

#include <fstream>
#include "re/DFA.h"
#include "re/CharSet.h"
#include "re/TestData.h"

using namespace std;

namespace RE {
namespace Test {

class DFATest : public UnitTestCpp::TestFixture
{
};

TEST_SUITE(re) {

static DFARuleSet<int> EmptyRules
{

};
static DFARuleSet<int> Rules
{
    DFARule<int>::Create('a', 0, 1),
};

static DFARuleSet<int> RulesCharSet
{
    DFARule<int>::Create(CharSet::Range('a', 'c'), 0, 1),
    DFARule<int>::Create(CharSet::Range('a', 'c'), 1, 2),
};

TEST_FIXTURE(DFATest, ConstructRules)
{
    DFA<int> finiteStateAutomaton(EmptyRules);
    EXPECT_EQ(0, finiteStateAutomaton.CurrentState());
    EXPECT_TRUE(finiteStateAutomaton.IsFinalState());
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(DFATest, ConstructRulesInitialState)
{
    DFA<int> finiteStateAutomaton(Rules, 0);
    EXPECT_EQ(0, finiteStateAutomaton.CurrentState());
    EXPECT_TRUE(finiteStateAutomaton.IsFinalState());
    // Initial state: 0, Final state: 0, Rules 0->1
    // We cannot get from 1 to final state
    EXPECT_FALSE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(DFATest, ConstructRulesInitialAndFinalState)
{
    DFA<int> finiteStateAutomaton(Rules, 0, 1);
    EXPECT_EQ(0, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    // Initial state: 0, Final state: 1, Rules 0->1
    // We can get to final state from all states
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(DFATest, HandleInputEmptyRules)
{
    DFA<int> finiteStateAutomaton(EmptyRules, 123);
    EXPECT_FALSE(finiteStateAutomaton.HandleInput('a'));
    EXPECT_EQ(123, finiteStateAutomaton.CurrentState());
    EXPECT_TRUE(finiteStateAutomaton.IsFinalState());
    // Initial state: 123, Final state: 123, Rules none
    // We can get from 123 to final state
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(DFATest, HandleInput)
{
    DFA<int> finiteStateAutomaton(Rules, 0, 1);
    EXPECT_TRUE(finiteStateAutomaton.HandleInput('a'));
    EXPECT_EQ(1, finiteStateAutomaton.CurrentState());
    EXPECT_TRUE(finiteStateAutomaton.IsFinalState());
    // Initial state: 0, Final state: 1, Rules 0->1
    // We can get to final state from all states
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(DFATest, Reset)
{
    DFA<int> finiteStateAutomaton(Rules, 0, 1);
    EXPECT_TRUE(finiteStateAutomaton.HandleInput('a'));
    EXPECT_EQ(1, finiteStateAutomaton.CurrentState());
    EXPECT_TRUE(finiteStateAutomaton.IsFinalState());
    EXPECT_TRUE(finiteStateAutomaton.Validate());
    finiteStateAutomaton.Reset();
    EXPECT_EQ(0, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(DFATest, HandleInputInvalidState)
{
    DFA<int> finiteStateAutomaton(Rules, 2, 1);
    EXPECT_FALSE(finiteStateAutomaton.HandleInput('a'));
    EXPECT_EQ(2, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    // Initial state: 2, Final state: 1, Rules 0->1
    // We cannot get from 2 to final state
    EXPECT_FALSE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(DFATest, HandleInputInvalidInput)
{
    DFA<int> finiteStateAutomaton(Rules, 0, 1);
    EXPECT_FALSE(finiteStateAutomaton.HandleInput('b'));
    EXPECT_EQ(0, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(DFATest, HandleInputCharSet)
{
    DFA<int> finiteStateAutomaton(RulesCharSet, 0, 2);
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

static DFARuleSet<int> RulesCharSetComplex
{
    DFARule<int>::Create('a', 0, 1),
    DFARule<int>::Create('b', 0, 3),
    DFARule<int>::Create('c', 1, 2),
    DFARule<int>::Create('d', 3, 2),
    DFARule<int>::Create('e', 3, 0),
};

TEST_FIXTURE(DFATest, HandleInputCharSetComplex)
{
    DFA<int> finiteStateAutomaton(RulesCharSetComplex, 0, 2);
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

static DFARuleSet<int> RulesCharSetComplexIncorrect
{
    DFARule<int>::Create('a', 0, 1),
    DFARule<int>::Create('b', 0, 3),
    DFARule<int>::Create('c', 0, 4),
    DFARule<int>::Create('d', 1, 2),
    DFARule<int>::Create('e', 3, 2),
};

TEST_FIXTURE(DFATest, HandleInputCharSetComplexIncorrect)
{
    DFA<int> finiteStateAutomaton(RulesCharSetComplexIncorrect, 0, 2);
    // Initial state: 0, Final state: 2, Rules 0->1, 0->3, 1->2, 3->2, 3->4
    // We cannot get from 4 to final state
    EXPECT_FALSE(finiteStateAutomaton.Validate());
}

} // TEST_SUITE(re)

} // namespace Test
} // namespace RE
