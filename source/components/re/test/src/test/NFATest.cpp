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
    NFA<int> finiteStateAutomaton(0, 1, -1);
    EXPECT_TRUE(finiteStateAutomaton.CurrentStateContains(0));
    EXPECT_FALSE(finiteStateAutomaton.HasFinalState());
    EXPECT_FALSE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(NFATest, ConstructRulesSameStartAndFinal)
{
    NFA<int> finiteStateAutomaton(0, 0, -1);
    EXPECT_TRUE(finiteStateAutomaton.CurrentStateContains(0));
    EXPECT_TRUE(finiteStateAutomaton.HasFinalState());
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(NFATest, AddRuleEpsilon)
{
    NFA<int> finiteStateAutomaton(0, 1, -1);
    EXPECT_TRUE(finiteStateAutomaton.CurrentStateContains(0));
    EXPECT_FALSE(finiteStateAutomaton.HasFinalState());
    finiteStateAutomaton.AddRule(RuleEpsilon);
    // Initial state: 0, Final state: 1, Rules 0->1
    // We can get from 0 to final state
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(NFATest, AddRule)
{
    NFA<int> finiteStateAutomaton(0, 2, -1);
    EXPECT_TRUE(finiteStateAutomaton.CurrentStateContains(0));
    EXPECT_FALSE(finiteStateAutomaton.HasFinalState());
    finiteStateAutomaton.AddRule(Rule1);
    finiteStateAutomaton.AddRule(Rule2);
    // Initial state: 0, Final state: 1, Rules 0->1
    // We can get from 0 to final state
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(NFATest, HandleInput)
{
    NFA<int> finiteStateAutomaton(0, 1, -1);
    finiteStateAutomaton.AddRule(Rule1);
    EXPECT_TRUE(finiteStateAutomaton.HandleInput('a'));
    EXPECT_TRUE(finiteStateAutomaton.CurrentStateContains(1));
    EXPECT_TRUE(finiteStateAutomaton.HasFinalState());
    // Initial state: 0, Final state: 1, Rules 0->1
    // We can get to final state from all states
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(NFATest, Reset)
{
    NFA<int> finiteStateAutomaton(0, 1, -1);
    finiteStateAutomaton.AddRule(Rule1);
    EXPECT_TRUE(finiteStateAutomaton.HandleInput('a'));
    EXPECT_TRUE(finiteStateAutomaton.CurrentStateContains(1));
    EXPECT_TRUE(finiteStateAutomaton.HasFinalState());
    EXPECT_TRUE(finiteStateAutomaton.Validate());
    finiteStateAutomaton.Reset();
    EXPECT_TRUE(finiteStateAutomaton.CurrentStateContains(0));
    EXPECT_FALSE(finiteStateAutomaton.HasFinalState());
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(NFATest, HandleInputInvalidState)
{
    NFA<int> finiteStateAutomaton(2, 1, -1);
    finiteStateAutomaton.AddRule(Rule1);
    EXPECT_FALSE(finiteStateAutomaton.HandleInput('a'));
    auto currentState = finiteStateAutomaton.CurrentStates();
    NFA<int>::StateSet expected{-1};
    EXPECT_EQ(expected, currentState);
    EXPECT_FALSE(finiteStateAutomaton.HasFinalState());
    // Initial state: 2, Final state: 1, Rules 0->1
    // We cannot get from 2 to final state
    EXPECT_FALSE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(NFATest, HandleInputInvalidInput)
{
    NFA<int> finiteStateAutomaton(0, 1, -1);
    finiteStateAutomaton.AddRule(Rule1);
    EXPECT_FALSE(finiteStateAutomaton.HandleInput('b'));
    auto currentState = finiteStateAutomaton.CurrentStates();
    NFA<int>::StateSet expected{-1};
    EXPECT_EQ(expected, currentState);
    EXPECT_FALSE(finiteStateAutomaton.HasFinalState());
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(NFATest, HandleInputCharSet)
{
    NFA<int> finiteStateAutomaton(0, 2, -1);
    finiteStateAutomaton.AddRule(CharSetRule1);
    finiteStateAutomaton.AddRule(CharSetRule2);
    EXPECT_TRUE(finiteStateAutomaton.HandleInput('a'));
    NFA<int>::StateSet expected{1};
    EXPECT_EQ(expected, finiteStateAutomaton.CurrentStates());
    EXPECT_FALSE(finiteStateAutomaton.HasFinalState());

    EXPECT_TRUE(finiteStateAutomaton.HandleInput('b'));
    expected = {2};
    EXPECT_EQ(expected, finiteStateAutomaton.CurrentStates());
    EXPECT_TRUE(finiteStateAutomaton.HasFinalState());

    EXPECT_FALSE(finiteStateAutomaton.HandleInput('c'));
    expected = {-1};
    EXPECT_EQ(expected, finiteStateAutomaton.CurrentStates());
    EXPECT_FALSE(finiteStateAutomaton.HasFinalState());
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

static NFA<int>::RuleSet RulesCharSetComplex
{
    NFARule<int>::Create('a', 0, 1),
    NFARule<int>::Create('b', 0, 3),
    NFARule<int>::Create('c', 1, 2),
    NFARule<int>::Create('d', 3, 2),
    NFARule<int>::Create('e', 3, 0),
};

TEST_FIXTURE(NFATest, HandleInputCharSetComplex)
{
    NFA<int> finiteStateAutomaton(0, 2, -1);
    for (auto const & rule : RulesCharSetComplex)
    {
        finiteStateAutomaton.AddRule(rule);
    }
    EXPECT_TRUE(finiteStateAutomaton.HandleInput('a'));
    NFA<int>::StateSet expected{1};
    EXPECT_EQ(expected, finiteStateAutomaton.CurrentStates());
    EXPECT_FALSE(finiteStateAutomaton.HasFinalState());

    EXPECT_TRUE(finiteStateAutomaton.HandleInput('c'));
    expected = {2};
    EXPECT_EQ(expected, finiteStateAutomaton.CurrentStates());
    EXPECT_TRUE(finiteStateAutomaton.HasFinalState());

    finiteStateAutomaton.Reset();

    EXPECT_FALSE(finiteStateAutomaton.HandleInput('c'));
    expected = {-1};
    EXPECT_EQ(expected, finiteStateAutomaton.CurrentStates());
    EXPECT_FALSE(finiteStateAutomaton.HasFinalState());
    finiteStateAutomaton.Reset();

    EXPECT_TRUE(finiteStateAutomaton.HandleInput('b'));
    expected = {3};
    EXPECT_EQ(expected, finiteStateAutomaton.CurrentStates());
    EXPECT_FALSE(finiteStateAutomaton.HasFinalState());

    EXPECT_TRUE(finiteStateAutomaton.HandleInput('d'));
    expected = {2};
    EXPECT_EQ(expected, finiteStateAutomaton.CurrentStates());
    EXPECT_TRUE(finiteStateAutomaton.HasFinalState());

    finiteStateAutomaton.Reset();

    EXPECT_TRUE(finiteStateAutomaton.HandleInput('b'));
    expected = {3};
    EXPECT_EQ(expected, finiteStateAutomaton.CurrentStates());
    EXPECT_FALSE(finiteStateAutomaton.HasFinalState());

    EXPECT_TRUE(finiteStateAutomaton.HandleInput('e'));
    expected = {0};
    EXPECT_EQ(expected, finiteStateAutomaton.CurrentStates());
    EXPECT_FALSE(finiteStateAutomaton.HasFinalState());
    // Initial state: 0, Final state: 2, Rules 0->1, 0->3, 1->2, 3->2, 3->0
    // We can get to final state from all states
    // The loop does not hang the check
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

static NFA<int>::RuleSet RulesCharSetComplexIncorrect
{
    NFARule<int>::Create('a', 0, 1),
    NFARule<int>::Create('b', 0, 3),
    NFARule<int>::Create('c', 0, 4),
    NFARule<int>::Create('d', 1, 2),
    NFARule<int>::Create('e', 3, 2),
};

TEST_FIXTURE(NFATest, HandleInputCharSetComplexIncorrect)
{
    NFA<int> finiteStateAutomaton(0, 2, -1);
    for (auto const & rule : RulesCharSetComplexIncorrect)
    {
        finiteStateAutomaton.AddRule(rule);
    }
    // Initial state: 0, Final state: 2, Rules 0->1, 0->3, 1->2, 3->2, 3->4
    // We cannot get from 4 to final state
    EXPECT_FALSE(finiteStateAutomaton.Validate());
}

static NFA<int>::RuleSet RulesNonDeterministic
{
    NFARule<int>::Create('a', 0, 1),
    NFARule<int>::Create('a', 0, 2),
    NFARule<int>::Create(CharSet(), 2, 3),
    NFARule<int>::Create('b', 1, 3),
};

TEST_FIXTURE(NFATest, HandleInputCharSetNonDeterministic)
{
    NFA<int> finiteStateAutomaton(0, 3, -1);
    for (auto const & rule : RulesNonDeterministic)
    {
        finiteStateAutomaton.AddRule(rule);
    }
    EXPECT_TRUE(finiteStateAutomaton.HandleInput('a'));
    NFA<int>::StateSet expected{1, 2, 3};
    EXPECT_EQ(expected, finiteStateAutomaton.CurrentStates());
    EXPECT_TRUE(finiteStateAutomaton.HasFinalState());

    EXPECT_TRUE(finiteStateAutomaton.HandleInput('b'));
    expected = {3};
    EXPECT_EQ(expected, finiteStateAutomaton.CurrentStates());
    EXPECT_TRUE(finiteStateAutomaton.HasFinalState());

    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(NFATest, ExportToDot)
{
    ostringstream stream;
    NFA<int> finiteStateAutomaton(0, 3, -1);
    for (auto const & rule : RulesNonDeterministic)
    {
        finiteStateAutomaton.AddRule(rule);
    }
    finiteStateAutomaton.ExportToDot(stream);
    string expected = "digraph {\n0 [penwidth=2.0]\n3 [peripheries=2]\n0 -> 1 [label=\"a\"];\n0 -> 2 [label=\"a\"];\n2 -> 3 [label=\"\"];\n1 -> 3 [label=\"b\"];\n}\n";
    string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(NFATest, PrintToMethod)
{
    ostringstream stream;
    NFA<int> finiteStateAutomaton(0, 3, -1);
    for (auto const & rule : RulesNonDeterministic)
    {
        finiteStateAutomaton.AddRule(rule);
    }
    finiteStateAutomaton.PrintTo(stream);
    string expected = "0 (a)-> 1\n0 (a)-> 2\n1 (b)-> 3\n2 ()-> 3\n";
    string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(NFATest, PrintToNonMethod)
{
    ostringstream stream;
    NFA<int> finiteStateAutomaton(0, 3, -1);
    for (auto const & rule : RulesNonDeterministic)
    {
        finiteStateAutomaton.AddRule(rule);
    }
    PrintTo(finiteStateAutomaton, stream);
    string expected = "0 (a)-> 1\n0 (a)-> 2\n1 (b)-> 3\n2 ()-> 3\n";
    string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(NFATest, OutputOperator)
{
    ostringstream stream;
    NFA<int> finiteStateAutomaton(0, 3, -1);
    for (auto const & rule : RulesNonDeterministic)
    {
        finiteStateAutomaton.AddRule(rule);
    }
    stream << finiteStateAutomaton;
    string expected = "0 (a)-> 1\n0 (a)-> 2\n1 (b)-> 3\n2 ()-> 3\n";
    string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

class NFAStringMatch : public NFA<int, char, int, CharSet>
{
public:
    static constexpr int StartState = 1;
    static constexpr int FinalState = 0;
    static constexpr int ErrorState = -1;
    NFAStringMatch()
        : NFA(StartState, FinalState, ErrorState)
    {}
    NFAStringMatch(const RuleSet & rules)
        : NFA(rules, StartState, FinalState, ErrorState)
    {}

    bool Match(const std::string & input)
    {
        Reset();
        for (auto ch : input)
        {
            HandleInput(ch);
        }
        return HasFinalState();
    }
    bool PartialMatch(const std::string & input)
    {
        Reset();
        for (auto ch : input)
        {
            HandleInput(ch);
        }
        return CurrentStates() != NFAStringMatch::StateSet {ErrorState};
    }
};

static NFAStringMatch::RuleSet RulesRossCoxExampleModified
{
    NFAStringMatch::Rule::Create('a', 1, 2),
    NFAStringMatch::Rule::Create('b', 2, 3),
    NFAStringMatch::Rule::Create('b', 3, 4),
    NFAStringMatch::Rule::Create(CharSet(), 4, 2),
    NFAStringMatch::Rule::Create('a', 4, 0),
};

TEST_FIXTURE(NFATest, MatchRussCoxExample)
{
    NFAStringMatch finiteStateAutomaton(RulesRossCoxExampleModified);

    finiteStateAutomaton.PrintTo(cout);

    EXPECT_TRUE(finiteStateAutomaton.Match("abba"));
    EXPECT_TRUE(finiteStateAutomaton.Match("abbbba"));
    EXPECT_TRUE(finiteStateAutomaton.Match("abbbbbba"));
    EXPECT_FALSE(finiteStateAutomaton.Match("a"));
    EXPECT_FALSE(finiteStateAutomaton.Match("b"));
    EXPECT_FALSE(finiteStateAutomaton.Match("aa"));
    EXPECT_FALSE(finiteStateAutomaton.Match("aba"));
    EXPECT_FALSE(finiteStateAutomaton.Match("abbba"));
    EXPECT_FALSE(finiteStateAutomaton.Match("abbbbba"));
    EXPECT_FALSE(finiteStateAutomaton.Match("abbbbbbba"));
    EXPECT_TRUE(finiteStateAutomaton.PartialMatch("abba"));
    EXPECT_TRUE(finiteStateAutomaton.PartialMatch(""));
    EXPECT_TRUE(finiteStateAutomaton.PartialMatch("a"));
    EXPECT_TRUE(finiteStateAutomaton.PartialMatch("ab"));
    EXPECT_TRUE(finiteStateAutomaton.PartialMatch("abb"));
    EXPECT_TRUE(finiteStateAutomaton.PartialMatch("abbb"));
}

} // TEST_SUITE(re)

} // namespace Test
} // namespace RE
