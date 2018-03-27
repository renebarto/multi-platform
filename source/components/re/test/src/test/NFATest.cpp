#include "unittest-cpp/UnitTestC++.h"

#include <fstream>
#include "re/NFA.h"
#include "re/CharSet.h"
#include "re/TestData.h"

using namespace std;

namespace RE {
namespace Test {

class NFATest : public UnitTestCpp::TestFixture
{
};

TEST_SUITE(re) {

static NFARule<int> RuleEpsilon(CharSet(), 0, 1);
static NFARule<int> Rule1('a', 0, 1);
static NFARule<int> Rule2('b', 1, 2);

static NFARule<int> CharSetRule1(CharSet::Range('a', 'c'), 0, 1);
static NFARule<int> CharSetRule2(CharSet::Range('a', 'c'), 1, 2);

TEST_FIXTURE(NFATest, ConstructRules)
{
    NFA<int> finiteStateAutomaton(0, 1);
    EXPECT_EQ(0, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    EXPECT_FALSE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(NFATest, ConstructRulesSameStartAndFinal)
{
    NFA<int> finiteStateAutomaton(0, 0);
    EXPECT_EQ(0, finiteStateAutomaton.CurrentState());
    EXPECT_TRUE(finiteStateAutomaton.IsFinalState());
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(NFATest, AddRuleEpsilon)
{
    NFA<int> finiteStateAutomaton(0, 1);
    EXPECT_EQ(0, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    finiteStateAutomaton.AddRule(RuleEpsilon);
    // Initial state: 0, Final state: 1, Rules 0->1
    // We can get from 0 to final state
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(NFATest, AddRule)
{
    NFA<int> finiteStateAutomaton(0, 2);
    EXPECT_EQ(0, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    finiteStateAutomaton.AddRule(Rule1);
    finiteStateAutomaton.AddRule(Rule2);
    // Initial state: 0, Final state: 1, Rules 0->1
    // We can get from 0 to final state
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(NFATest, HandleInput)
{
    NFA<int> finiteStateAutomaton(0, 1);
    finiteStateAutomaton.AddRule(Rule1);
    EXPECT_TRUE(finiteStateAutomaton.HandleInput('a'));
    EXPECT_EQ(1, finiteStateAutomaton.CurrentState());
    EXPECT_TRUE(finiteStateAutomaton.IsFinalState());
    // Initial state: 0, Final state: 1, Rules 0->1
    // We can get to final state from all states
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(NFATest, Reset)
{
    NFA<int> finiteStateAutomaton(0, 1);
    finiteStateAutomaton.AddRule(Rule1);
    EXPECT_TRUE(finiteStateAutomaton.HandleInput('a'));
    EXPECT_EQ(1, finiteStateAutomaton.CurrentState());
    EXPECT_TRUE(finiteStateAutomaton.IsFinalState());
    EXPECT_TRUE(finiteStateAutomaton.Validate());
    finiteStateAutomaton.Reset();
    EXPECT_EQ(0, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(NFATest, HandleInputInvalidState)
{
    NFA<int> finiteStateAutomaton(2, 1);
    finiteStateAutomaton.AddRule(Rule1);
    EXPECT_FALSE(finiteStateAutomaton.HandleInput('a'));
    EXPECT_EQ(2, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    // Initial state: 2, Final state: 1, Rules 0->1
    // We cannot get from 2 to final state
    EXPECT_FALSE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(NFATest, HandleInputInvalidInput)
{
    NFA<int> finiteStateAutomaton(0, 1);
    finiteStateAutomaton.AddRule(Rule1);
    EXPECT_FALSE(finiteStateAutomaton.HandleInput('b'));
    EXPECT_EQ(0, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(NFATest, HandleInputCharSet)
{
    NFA<int> finiteStateAutomaton(0, 2);
    finiteStateAutomaton.AddRule(CharSetRule1);
    finiteStateAutomaton.AddRule(CharSetRule2);
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

static NFARuleSet<int> RulesCharSetComplex
{
    NFARule<int>::Create('a', 0, 1),
    NFARule<int>::Create('b', 0, 3),
    NFARule<int>::Create('c', 1, 2),
    NFARule<int>::Create('d', 3, 2),
    NFARule<int>::Create('e', 3, 0),
};

TEST_FIXTURE(NFATest, HandleInputCharSetComplex)
{
    NFA<int> finiteStateAutomaton(0, 2);
    for (auto const & rule : RulesCharSetComplex)
    {
        finiteStateAutomaton.AddRule(rule);
    }
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

static NFARuleSet<int> RulesCharSetComplexIncorrect
{
    NFARule<int>::Create('a', 0, 1),
    NFARule<int>::Create('b', 0, 3),
    NFARule<int>::Create('c', 0, 4),
    NFARule<int>::Create('d', 1, 2),
    NFARule<int>::Create('e', 3, 2),
};

TEST_FIXTURE(NFATest, HandleInputCharSetComplexIncorrect)
{
    NFA<int> finiteStateAutomaton(0, 2);
    for (auto const & rule : RulesCharSetComplexIncorrect)
    {
        finiteStateAutomaton.AddRule(rule);
    }
    // Initial state: 0, Final state: 2, Rules 0->1, 0->3, 1->2, 3->2, 3->4
    // We cannot get from 4 to final state
    EXPECT_FALSE(finiteStateAutomaton.Validate());
}

static NFARuleSet<int> RulesNonDeterministic
{
    NFARule<int>::Create('a', 0, 1),
    NFARule<int>::Create('a', 0, 2),
    NFARule<int>::Create(CharSet(), 2, 3),
    NFARule<int>::Create(CharSet(), 1, 3),
};

TEST_FIXTURE(NFATest, HandleInputCharSetNonDeterministic)
{
    NFA<int> finiteStateAutomaton(0, 3);
    for (auto const & rule : RulesNonDeterministic)
    {
        finiteStateAutomaton.AddRule(rule);
    }
    EXPECT_TRUE(finiteStateAutomaton.HandleInput('a'));
    auto currentState = finiteStateAutomaton.CurrentState();
    EXPECT_TRUE(currentState == 1 || currentState == 2);
    EXPECT_TRUE(finiteStateAutomaton.HandleEpsilon());
    EXPECT_TRUE(finiteStateAutomaton.IsFinalState());
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(NFATest, PrintToMethod)
{
    ostringstream stream;
    NFA<int> finiteStateAutomaton(0, 3);
    for (auto const & rule : RulesNonDeterministic)
    {
        finiteStateAutomaton.AddRule(rule);
    }
    finiteStateAutomaton.PrintTo(stream);
    string expected = "digraph {\n0 [penwidth=2.0]\n3 [peripheries=2]\n0 -> 1 [label=\"a\"];\n0 -> 2 [label=\"a\"];\n2 -> 3 [label=\"\"];\n1 -> 3 [label=\"\"];\n}\n";
    string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(NFATest, PrintToNonMethod)
{
    ostringstream stream;
    NFA<int> finiteStateAutomaton(0, 3);
    for (auto const & rule : RulesNonDeterministic)
    {
        finiteStateAutomaton.AddRule(rule);
    }
    PrintTo(finiteStateAutomaton, stream);
    string expected = "digraph {\n0 [penwidth=2.0]\n3 [peripheries=2]\n0 -> 1 [label=\"a\"];\n0 -> 2 [label=\"a\"];\n2 -> 3 [label=\"\"];\n1 -> 3 [label=\"\"];\n}\n";
    string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(NFATest, OutputOperator)
{
    ostringstream stream;
    NFA<int> finiteStateAutomaton(0, 3);
    for (auto const & rule : RulesNonDeterministic)
    {
        finiteStateAutomaton.AddRule(rule);
    }
    stream << finiteStateAutomaton;
    string expected = "digraph {\n0 [penwidth=2.0]\n3 [peripheries=2]\n0 -> 1 [label=\"a\"];\n0 -> 2 [label=\"a\"];\n2 -> 3 [label=\"\"];\n1 -> 3 [label=\"\"];\n}\n";
    string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

} // TEST_SUITE(re)

} // namespace Test
} // namespace RE
