#include "unittest-cpp/UnitTestC++.h"

#include <fstream>
#include "regex/DFA.h"
#include "regex/CharSet.h"
#include "regex/TestData.h"

using namespace std;

namespace Regex {
namespace Test {

class DFATest : public UnitTestCpp::TestFixture
{
};

TEST_SUITE(regex) {

static DFA<int>::RuleSet EmptyRules
{

};
static DFA<int>::RuleSet Rules
{
    DFARule<int>::Create('a', 0, 1),
};
static DFA<int>::RuleSet RulesComplete
{
    DFARule<int>::Create('a', 0, 1),
    DFARule<int>::Create(~CharSet('a'), 0, -1),
};

static DFA<int>::RuleSet RulesCharSet
{
    DFARule<int>::Create(CharSet::Range('a', 'c'), 0, 1),
    DFARule<int>::Create(CharSet::Range('a', 'c'), 1, 2),
};

TEST_FIXTURE(DFATest, ConstructRulesInitialAndFinalState)
{
    DFA<int> finiteStateAutomaton(Rules, 0, 1, -1);
    EXPECT_EQ(0, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    EXPECT_FALSE(finiteStateAutomaton.IsErrorState());
    // Initial state: 0, Final state: 1, Rules 0->1
    // We can get to final state from all states
    EXPECT_FALSE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(DFATest, ConstructRulesInitialAndFinalStateComplete)
{
    DFA<int> finiteStateAutomaton(RulesComplete, 0, 1, -1);
    EXPECT_EQ(0, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    EXPECT_FALSE(finiteStateAutomaton.IsErrorState());
    // Initial state: 0, Final state: 1, Rules 0->1
    // We can get to final state from all states
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(DFATest, ConstructRulesInitialAndFinalStateMakeComplete)
{
    DFA<int> finiteStateAutomaton(Rules, 0, 1, -1);
    finiteStateAutomaton.MakeComplete();
    EXPECT_EQ(0, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    EXPECT_FALSE(finiteStateAutomaton.IsErrorState());
    // Initial state: 0, Final state: 1, Rules 0->1
    // We can get to final state from all states
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(DFATest, HandleInputEmptyRules)
{
    DFA<int> finiteStateAutomaton(EmptyRules, 123, 123, -1);
    EXPECT_FALSE(finiteStateAutomaton.HandleInput('a'));
    EXPECT_EQ(-1, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    EXPECT_TRUE(finiteStateAutomaton.IsErrorState());
    // Initial state: 123, Final state: 123, Rules none
    // We can get from 123 to final state
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(DFATest, HandleInput)
{
    DFA<int> finiteStateAutomaton(Rules, 0, 1, -1);
    finiteStateAutomaton.MakeComplete();
    EXPECT_TRUE(finiteStateAutomaton.HandleInput('a'));
    EXPECT_EQ(1, finiteStateAutomaton.CurrentState());
    EXPECT_TRUE(finiteStateAutomaton.IsFinalState());
    EXPECT_FALSE(finiteStateAutomaton.IsErrorState());
    // Initial state: 0, Final state: 1, Rules 0->1
    // We can get to final state from all states
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(DFATest, HandleInvalidInput)
{
    DFA<int> finiteStateAutomaton(Rules, 0, 1, -1);
    finiteStateAutomaton.MakeComplete();
    EXPECT_FALSE(finiteStateAutomaton.HandleInput('b'));
    EXPECT_EQ(-1, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    EXPECT_TRUE(finiteStateAutomaton.IsErrorState());
    // Initial state: 0, Final state: 1, Rules 0->1
    // We can get to final state from all states
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(DFATest, Reset)
{
    DFA<int> finiteStateAutomaton(Rules, 0, 1, -1);
    finiteStateAutomaton.MakeComplete();
    EXPECT_TRUE(finiteStateAutomaton.HandleInput('a'));
    EXPECT_EQ(1, finiteStateAutomaton.CurrentState());
    EXPECT_TRUE(finiteStateAutomaton.IsFinalState());
    EXPECT_FALSE(finiteStateAutomaton.IsErrorState());
    EXPECT_TRUE(finiteStateAutomaton.Validate());
    finiteStateAutomaton.Reset();
    EXPECT_EQ(0, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    EXPECT_FALSE(finiteStateAutomaton.IsErrorState());
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(DFATest, HandleInputInvalidState)
{
    DFA<int> finiteStateAutomaton(RulesComplete, 2, 1, -1);
    EXPECT_FALSE(finiteStateAutomaton.HandleInput('a'));
    EXPECT_EQ(2, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    EXPECT_FALSE(finiteStateAutomaton.IsErrorState());
    // Initial state: 2, Final state: 1, Rules 0->1
    // We cannot get from 2 to final state
    EXPECT_FALSE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(DFATest, HandleInputCharSet)
{
    DFA<int> finiteStateAutomaton(RulesCharSet, 0, 2, -1);
    finiteStateAutomaton.MakeComplete();
    EXPECT_TRUE(finiteStateAutomaton.HandleInput('a'));
    EXPECT_EQ(1, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    EXPECT_FALSE(finiteStateAutomaton.IsErrorState());
    EXPECT_TRUE(finiteStateAutomaton.HandleInput('b'));
    EXPECT_EQ(2, finiteStateAutomaton.CurrentState());
    EXPECT_TRUE(finiteStateAutomaton.IsFinalState());
    EXPECT_FALSE(finiteStateAutomaton.IsErrorState());
    EXPECT_FALSE(finiteStateAutomaton.HandleInput('c'));
    EXPECT_EQ(-1, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    EXPECT_TRUE(finiteStateAutomaton.IsErrorState());
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

static DFA<int>::RuleSet RulesCharSetComplex
{
    DFARule<int>::Create('a', 0, 1),
    DFARule<int>::Create('b', 0, 3),
    DFARule<int>::Create('c', 1, 2),
    DFARule<int>::Create('d', 3, 2),
    DFARule<int>::Create('e', 3, 0),
};

TEST_FIXTURE(DFATest, HandleInputCharSetComplex)
{
    DFA<int> finiteStateAutomaton(RulesCharSetComplex, 0, 2, -1);
    finiteStateAutomaton.MakeComplete();
    EXPECT_TRUE(finiteStateAutomaton.HandleInput('a'));
    EXPECT_EQ(1, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    EXPECT_TRUE(finiteStateAutomaton.HandleInput('c'));
    EXPECT_EQ(2, finiteStateAutomaton.CurrentState());
    EXPECT_TRUE(finiteStateAutomaton.IsFinalState());
    finiteStateAutomaton.Reset();
    EXPECT_FALSE(finiteStateAutomaton.HandleInput('c'));
    EXPECT_TRUE(finiteStateAutomaton.IsErrorState());
    EXPECT_FALSE(finiteStateAutomaton.HandleInput('b'));
    EXPECT_TRUE(finiteStateAutomaton.IsErrorState());
    EXPECT_FALSE(finiteStateAutomaton.HandleInput('d'));
    EXPECT_TRUE(finiteStateAutomaton.IsErrorState());
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

static DFA<int>::RuleSet RulesCharSetComplexIncorrect
{
    DFARule<int>::Create('a', 0, 1),
    DFARule<int>::Create('b', 0, 3),
    DFARule<int>::Create('c', 0, 4),
    DFARule<int>::Create('d', 1, 2),
    DFARule<int>::Create('e', 3, 2),
};

TEST_FIXTURE(DFATest, HandleInputCharSetComplexIncorrect)
{
    DFA<int> finiteStateAutomaton(RulesCharSetComplexIncorrect, 0, 2, -1);
    // Initial state: 0, Final state: 2, Rules 0->1, 0->3, 1->2, 3->2, 3->4
    // We cannot get from 4 to final state
    EXPECT_FALSE(finiteStateAutomaton.Validate());
}

static DFA<int>::RuleSet RulesRossCoxExample
    {
        DFARule<int>::Create('a', 0, 1),
        DFARule<int>::Create('b', 1, 2),
        DFARule<int>::Create('b', 2, 3),
        DFARule<int>::Create('b', 3, 2),
        DFARule<int>::Create('a', 3, 4),
    };

TEST_FIXTURE(DFATest, HandleInputRussCoxExample)
{
    DFA<int> finiteStateAutomaton(RulesRossCoxExample, 0, 4, -1);
    finiteStateAutomaton.MakeComplete();
    EXPECT_TRUE(finiteStateAutomaton.HandleInput('a'));
    EXPECT_EQ(1, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    EXPECT_FALSE(finiteStateAutomaton.IsErrorState());

    EXPECT_TRUE(finiteStateAutomaton.HandleInput('b'));
    EXPECT_EQ(2, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    EXPECT_FALSE(finiteStateAutomaton.IsErrorState());

    EXPECT_TRUE(finiteStateAutomaton.HandleInput('b'));
    EXPECT_EQ(3, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    EXPECT_FALSE(finiteStateAutomaton.IsErrorState());

    EXPECT_TRUE(finiteStateAutomaton.HandleInput('b'));
    EXPECT_EQ(2, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    EXPECT_FALSE(finiteStateAutomaton.IsErrorState());

    EXPECT_TRUE(finiteStateAutomaton.HandleInput('b'));
    EXPECT_EQ(3, finiteStateAutomaton.CurrentState());
    EXPECT_FALSE(finiteStateAutomaton.IsFinalState());
    EXPECT_FALSE(finiteStateAutomaton.IsErrorState());

    EXPECT_TRUE(finiteStateAutomaton.HandleInput('a'));
    EXPECT_EQ(4, finiteStateAutomaton.CurrentState());
    EXPECT_TRUE(finiteStateAutomaton.IsFinalState());
    EXPECT_FALSE(finiteStateAutomaton.IsErrorState());

    // We can get to final state from all states
    EXPECT_TRUE(finiteStateAutomaton.Validate());
}

TEST_FIXTURE(DFATest, ExportToDot)
{
    ostringstream stream;
    DFA<int> finiteStateAutomaton(RulesCharSetComplex, 0, 2, -1);
    finiteStateAutomaton.ExportToDot(stream);
    string expected = "digraph {\n0 [penwidth=2.0]\n2 [peripheries=2]\n0 -> 1 [label=\"a\"];\n0 -> 3 [label=\"b\"];\n1 -> 2 [label=\"c\"];\n3 -> 2 [label=\"d\"];\n3 -> 0 [label=\"e\"];\n}\n";
    string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(DFATest, PrintToMethod)
{
    ostringstream stream;
    DFA<int> finiteStateAutomaton(RulesCharSetComplex, 0, 2, -1);
    PrintTo(finiteStateAutomaton, stream);
    string expected = "0 (a)-> 1\n0 (b)-> 3\n1 (c)-> 2\n3 (d)-> 2\n3 (e)-> 0\n";
    string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(DFATest, PrintToNonMethod)
{
    ostringstream stream;
    DFA<int> finiteStateAutomaton(RulesCharSetComplex, 0, 2, -1);
    PrintTo(finiteStateAutomaton, stream);
    string expected = "0 (a)-> 1\n0 (b)-> 3\n1 (c)-> 2\n3 (d)-> 2\n3 (e)-> 0\n";
    string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(DFATest, OutputOperator)
{
    ostringstream stream;
    DFA<int> finiteStateAutomaton(RulesCharSetComplex, 0, 2, -1);
    stream << finiteStateAutomaton;
    string expected = "0 (a)-> 1\n0 (b)-> 3\n1 (c)-> 2\n3 (d)-> 2\n3 (e)-> 0\n";
    string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

class DFAStringMatch : public DFA<int, char, int, CharSet>
{
public:
    static constexpr int StartState = 1;
    static constexpr int FinalState = 0;
    static constexpr int ErrorState = -1;
    DFAStringMatch()
        : DFA(StartState, FinalState, ErrorState)
    {}
    explicit DFAStringMatch(const RuleSet & rules)
        : DFA(rules, StartState, FinalState, ErrorState)
    {}

    bool Match(const std::string & input)
    {
        Reset();
        for (auto ch : input)
        {
            HandleInput(ch);
        }
        return CurrentState() == FinalState;
    }
    bool PartialMatch(const std::string & input)
    {
        Reset();
        for (auto ch : input)
        {
            HandleInput(ch);
        }
        return CurrentState() != ErrorState;
    }
};

static DFAStringMatch::RuleSet RulesRossCoxExampleModified
{
    DFAStringMatch::Rule::Create('a', 1, 2),
    DFAStringMatch::Rule::Create('b', 2, 3),
    DFAStringMatch::Rule::Create('b', 3, 4),
    DFAStringMatch::Rule::Create('b', 4, 3),
    DFAStringMatch::Rule::Create('a', 4, 0),
};

TEST_FIXTURE(DFATest, MatchRussCoxExample)
{
    DFAStringMatch finiteStateAutomaton(RulesRossCoxExampleModified);
    finiteStateAutomaton.MakeComplete();

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

} // TEST_SUITE(regex)

} // namespace Test
} // namespace Regex
