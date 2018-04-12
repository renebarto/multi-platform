#pragma once

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include "ValueSet.h"

namespace Regex {

template<typename State, typename InputValue = char, typename UnderlyingType = int, typename InputSpecifier = ValueSet<InputValue, UnderlyingType>>
class DFARule
{
public:
    DFARule() = delete;

    static DFARule Create(InputSpecifier input, State state, State nextState)
    {
        return DFARule(input, state, nextState);
    }

    State ExpectedState() const
    {
        return _expectedState;
    }
    State NextState() const
    {
        return _nextState;
    }
    void NextState(State state)
    {
        _nextState = state;
    }
    InputSpecifier ExpectedInput() const
    {
        return _expectedInput;
    }
    bool Match(InputValue input) const
    {
        return _expectedInput.Contains(input);
    }

protected:
    InputSpecifier _expectedInput;
    State _expectedState;
    State _nextState;

public:
    DFARule(InputSpecifier input, State state, State nextState)
        : _expectedInput(input)
        , _expectedState(state)
        , _nextState(nextState)
    {}
    DFARule(const DFARule & other)
        : _expectedInput(other._expectedInput)
        , _expectedState(other._expectedState)
        , _nextState(other._nextState)
    {
    }
    DFARule & operator = (const DFARule & other)
    {
        if (this != &other)
        {
            _expectedInput = other._expectedInput;
            _expectedState = other._expectedState;
            _nextState = other._nextState;
        }
        return *this;
    }
};

template<typename State, typename InputValue = char, typename UnderlyingType = int, typename InputSpecifier = ValueSet<InputValue, UnderlyingType>>
class DFA
{
public:
    using Rule = DFARule<State, InputValue, UnderlyingType, InputSpecifier>;
    using RuleSet = std::vector<Rule>;

    DFA(State initialState, State finalState, State errorState)
        : _rules()
        , _startStateMap()
        , _endStateMap()
        , _initialState(initialState)
        , _finalState(finalState)
        , _errorState(errorState)
        , _currentState(_initialState)
    {}
    DFA(const RuleSet & rules, State initialState, State finalState, State errorState)
        : _rules(rules)
        , _startStateMap()
        , _endStateMap()
        , _initialState(initialState)
        , _finalState(finalState)
        , _errorState(errorState)
        , _currentState(_initialState)
    {
        UpdateStateMap();
    }
    DFA(const DFA & other)
        : _rules(other._rules)
        , _startStateMap(other._startStateMap)
        , _endStateMap(other._endStateMap)
        , _initialState(other._initialState)
        , _finalState(other._finalState)
        , _errorState(other._errorState)
        , _currentState(other._currentState)
    {}
    DFA(DFA && other)
        : _rules(std::move(other._rules))
        , _startStateMap(std::move(other._startStateMap))
        , _endStateMap(std::move(other._endStateMap))
        , _initialState(other._initialState)
        , _finalState(other._finalState)
        , _errorState(other._errorState)
        , _currentState(other._currentState)
    {}

    DFA & operator = (const DFA & other)
    {
        if (this != &other)
        {
            _rules = other._rules;
            _startStateMap = other._startStateMap;
            _endStateMap = other._endStateMap;
            _initialState = other._initialState;
            _finalState = other._finalState;
            _errorState = other._errorState;
            _currentState = other._currentState;
        }
        return *this;
    }
    DFA & operator = (DFA && other)
    {
        if (this != &other)
        {
            _rules = std::move(other._rules);
            _startStateMap = std::move(other._startStateMap);
            _endStateMap = std::move(other._endStateMap);
            _initialState = other._initialState;
            _finalState = other._finalState;
            _errorState = other._errorState;
            _currentState = other._currentState;
        }
        return *this;
    }

    void AddRule(const Rule & rule)
    {
        _rules.push_back(rule);
        UpdateStateMap();
    }

    const RuleSet & GetRules() const { return _rules; }
    bool FindRuleFromStateForChar(State startState, char ch, Rule *& foundRule)
    {
        for (auto rule : _startStateMap[startState])
        {
            if (rule->ExpectedInput().Contains(ch))
            {
                foundRule = rule;
                return true;
            }
        }
        return false;
    }

    bool FindRuleToStateForChar(State endState, char ch, Rule *& foundRule)
    {
        for (auto rule : _endStateMap[endState])
        {
            if (rule->ExpectedInput().Contains(ch))
            {
                foundRule = rule;
                return true;
            }
        }
        return false;
    }

    void MakeComplete();
    void UpdateStateMap();

    bool Validate() const;
    void Reset() { _currentState = _initialState; }
    bool HandleInput(InputValue input)
    {
        if (IsFinalState())
        {
            _currentState = _errorState;
            return false;
        }
        for (auto rule : _startStateMap[_currentState])
        {
            if (rule->Match(input))
            {
                _currentState = rule->NextState();
                return (_currentState != _errorState);
            }
        }
        return false;
    }
    State CurrentState() const { return _currentState; }
    bool IsFinalState() const { return _currentState == _finalState; }
    bool IsErrorState() const { return _currentState == _errorState; }

    void ExportToDot(std::ostream & stream) const
    {
        stream << "digraph {" << std::endl;
        stream << _initialState << " [penwidth=2.0]" << std::endl;
        stream << _finalState << " [peripheries=2]" << std::endl;
        for (auto const & rule : _rules)
        {
            stream << rule.ExpectedState() << " -> " << rule.NextState() << " [label=\"" << rule.ExpectedInput() << "\"];" << std::endl;
        }
        stream << "}" << std::endl;
    }
    void PrintTo(std::ostream & stream) const
    {
        for (auto state : _startStateMap)
        {
            for (auto rule : state.second)
            {
                stream << state.first << " (" << rule->ExpectedInput() << ")-> " << rule->NextState() << std::endl;
            }
        }
    }

protected:
    using StateRules = std::vector<Rule *>;
    using StateMap = std::map<State, StateRules>;
    RuleSet _rules;
    StateMap _startStateMap;
    StateMap _endStateMap;
    State _initialState;
    State _finalState;
    State _errorState;
    State _currentState;
};

template<typename State, typename InputValue = char, typename UnderlyingType = int, typename InputSpecifier = ValueSet<InputValue, UnderlyingType>>
class DFAValidator
{
public:
    using Rule = DFARule<State, InputValue, UnderlyingType, InputSpecifier>;
    using RuleSet = std::vector<Rule>;

    DFAValidator()
        : _map()
    {}

    bool Validate(const RuleSet & rules,
                  const std::set<State> & states, State finalState, State errorState) const
    {
        _map.clear();
        for (auto state : states)
        {
            if (!Validate(rules, state, finalState, errorState))
                return false;
        }
        return true;
    }
    bool Validate(const RuleSet & rules,
                  State from, State to, State errorState) const
    {
        auto it = _map.find(from);
        if (it != _map.end())
        {
            return it->second;
        }
        _map.insert(Pair(from, false));
        if ((from == to) || (from == errorState))
        {
            _map[from] = true;
            return true;
        }
        bool ok = false;
        for (auto const & rule : rules)
        {
            if (rule.ExpectedState() == from)
            {
                if (Validate(rules, rule.NextState(), to, errorState))
                {
                    _map[from] = true;
                    ok = true;
                }
            }
        }
        if (!ok)
            return false;
        InputSpecifier coveredSet;
        for (auto const & rule : rules)
        {
            if (rule.ExpectedState() == from)
            {
                coveredSet |= rule.ExpectedInput();
            }
        }
        return coveredSet.IsFull();
    }

protected:
    using Pair = std::pair<State, bool>;
    mutable std::map<State, bool> _map;
};

template<typename State, typename InputValue, typename UnderlyingType, typename InputSpecifier>
bool DFA<State, InputValue, UnderlyingType, InputSpecifier>::Validate() const
{
    DFAValidator<State, InputValue, UnderlyingType, InputSpecifier> validator;
    std::set<State> states({_initialState});
    for (auto const & rule : _rules)
    {
        if (states.find(rule.ExpectedState()) == states.end())
            states.insert(rule.ExpectedState());
        if (states.find(rule.NextState()) == states.end())
            states.insert(rule.NextState());
    }

    return validator.Validate(_rules, states, _finalState, _errorState);
}

template<typename State, typename InputValue, typename UnderlyingType, typename InputSpecifier>
void DFA<State, InputValue, UnderlyingType, InputSpecifier>::MakeComplete()
{
    std::set<State> states({_initialState});
    for (auto const & rule : _rules)
    {
        if (states.find(rule.ExpectedState()) == states.end())
            states.insert(rule.ExpectedState());
        if (states.find(rule.NextState()) == states.end())
            states.insert(rule.NextState());
    }

    for (auto state : states)
    {
        if (state == _finalState)
            continue;
        InputSpecifier missingAlphabet = InputSpecifier::All;
        for (auto rule : _startStateMap[state])
        {
            missingAlphabet -= rule->ExpectedInput();
        }
        if (!missingAlphabet.IsEmpty())
        {
            AddRule(Rule::Create(missingAlphabet, state, _errorState));
        }
    }
}

template<typename State, typename InputValue, typename UnderlyingType, typename InputSpecifier>
void DFA<State, InputValue, UnderlyingType, InputSpecifier>::UpdateStateMap()
{
    _startStateMap.clear();
    _endStateMap.clear();
    for (auto & rule : _rules)
    {
        if (_startStateMap.find(rule.ExpectedState()) != _startStateMap.end())
        {
            _startStateMap[rule.ExpectedState()].push_back(&rule);
        }
        else
        {
            _startStateMap.insert(std::pair<State, StateRules>(rule.ExpectedState(), StateRules {&rule}));
        }
        if (_endStateMap.find(rule.NextState()) != _startStateMap.end())
        {
            _endStateMap[rule.NextState()].push_back(&rule);
        }
        else
        {
            _endStateMap.insert(std::pair<State, StateRules>(rule.NextState(), StateRules {&rule}));
        }
    }
}

template<typename State, typename InputValue, typename UnderlyingType, typename InputSpecifier>
inline void PrintTo(const DFA<State, InputValue, UnderlyingType, InputSpecifier> & term, std::ostream & stream)
{
    term.PrintTo(stream);
}

} // namespace Regex

template<typename State, typename InputValue, typename UnderlyingType, typename InputSpecifier>
inline std::ostream & operator << (std::ostream & stream, const Regex::DFA<State, InputValue, UnderlyingType, InputSpecifier> & term)
{
    term.PrintTo(stream);
    return stream;
}

