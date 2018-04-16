#pragma once

#include <iostream>
#include <map>
#include <set>
#include <memory>
#include <vector>
#include "ValueSet.h"

namespace Regex {

template<typename State, typename InputValue = char, typename UnderlyingType = int, typename InputSpecifier = ValueSet<InputValue, UnderlyingType>>
class NFARule
{
public:
    NFARule() = delete;

    static NFARule Create(InputSpecifier input, State state, State nextState)
    {
        return NFARule(input, state, nextState);
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
    bool IsEpsilon() const
    {
        return _expectedInput.IsEmpty();
    }

protected:
    InputSpecifier _expectedInput;
    State _expectedState;
    State _nextState;

public:
    NFARule(InputSpecifier input, State state, State nextState)
        : _expectedInput(input)
        , _expectedState(state)
        , _nextState(nextState)
    {}
    NFARule(const NFARule & other)
        : _expectedInput(other._expectedInput)
        , _expectedState(other._expectedState)
        , _nextState(other._nextState)
    {
    }
    NFARule & operator = (const NFARule & other)
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
class NFA
{
public:
    using Rule = NFARule<State, InputValue, UnderlyingType, InputSpecifier>;
    using RuleSet = std::vector<Rule>;
    using RulePtrSet = std::vector<Rule *>;
    using StateSet = std::set<State>;

    NFA(State initialState, State finalState, State errorState)
        : _rules()
        , _startStateMap()
        , _endStateMap()
        , _initialState(initialState)
        , _finalState(finalState)
        , _errorState(errorState)
        , _currentStates({_initialState})
    {}
    NFA(const RuleSet & rules, State initialState, State finalState, State errorState)
        : _rules(rules)
        , _startStateMap()
        , _endStateMap()
        , _initialState(initialState)
        , _finalState(finalState)
        , _errorState(errorState)
        , _currentStates({_initialState})
    {
        UpdateStateMap();
    }

    void AddRule(const NFARule<State, InputValue, UnderlyingType, InputSpecifier> & rule)
    {
        _rules.push_back(rule);
        UpdateStateMap();
    }
    const RuleSet & GetRules() const { return _rules; }
    const RulePtrSet FindRulesToState(State finalState)
    {
        return _endStateMap[finalState];
    }

    void UpdateStateMap();

    bool Validate() const;
    void Reset() { _currentStates = {_initialState}; }
    bool HandleInput(InputValue input)
    {
        StateSet startStates = _currentStates;
        // First add all states reachable by epsilon transition
        for (auto state : _currentStates)
        {
            for (auto rule : _startStateMap[state])
            {
                if (rule->IsEpsilon())
                {
                    if (startStates.find(rule->NextState()) == startStates.end())
                    {
                        startStates.insert(rule->NextState());
                    }
                }
            }
        }
        StateSet endStates;
        for (auto state : startStates)
        {
            for (auto rule : _startStateMap[state])
            {
                if (rule->Match(input))
                {
                    if (endStates.find(rule->NextState()) == endStates.end())
                    {
                        endStates.insert(rule->NextState());
                    }
                }
            }
        }
        StateSet reachableStates = endStates;
        bool changes = true;
        while (changes)
        {
            changes = false;
            endStates = reachableStates;
            for (auto state : endStates)
            {
                for (auto rule : _startStateMap[state])
                {
                    if (rule->IsEpsilon())
                    {
                        if (reachableStates.find(rule->NextState()) == reachableStates.end())
                        {
                            reachableStates.insert(rule->NextState());
                            changes = true;
                        }
                    }
                }
            }
        }
        if (reachableStates.empty())
        {
            _currentStates = { _errorState };
            return false;
        }
        _currentStates = reachableStates;
        return true;
    }
    StateSet CurrentStates() const { return _currentStates; }
    bool CurrentStateContains(State state) const { return _currentStates.find(state) != _currentStates.end(); }
    bool HasFinalState() const { return CurrentStateContains(_finalState); }
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
    StateSet _currentStates;
};

template<typename State, typename InputValue = char, typename UnderlyingType = int, typename InputSpecifier = ValueSet<InputValue, UnderlyingType>>
class NFAValidator
{
public:
    using Rule = NFARule<State, InputValue, UnderlyingType, InputSpecifier>;
    using RuleSet = std::vector<Rule>;

    NFAValidator()
        : _map()
    {}

    bool Validate(const RuleSet & rules,
                  const std::set<State> & states, State finalState) const
    {
        _map.clear();
        for (auto state : states)
        {
            if (!Validate(rules, state, finalState))
                return false;
        }
        return true;
    }
    bool Validate(const RuleSet & rules,
                  State from, State to) const
    {
        auto it = _map.find(from);
        if (it != _map.end())
        {
            return it->second;
        }
        _map.insert(Pair(from, false));
        if (from == to)
        {
            _map[from] = true;
            return true;
        }
        bool ok = false;
        for (auto const & rule : rules)
        {
            if (rule.ExpectedState() == from)
            {
                if (Validate(rules, rule.NextState(), to))
                {
                    _map[from] = true;
                    ok = true;
                }
            }
        }
        return ok;
    }

protected:
    using Pair = std::pair<State, bool>;
    mutable std::map<State, bool> _map;
};

template<typename State, typename InputValue, typename UnderlyingType, typename InputSpecifier>
bool NFA<State, InputValue, UnderlyingType, InputSpecifier>::Validate() const
{
    NFAValidator<State, InputValue, UnderlyingType, InputSpecifier> validator;
    std::set<State> states({_initialState});
    for (auto const & rule : _rules)
    {
        if (states.find(rule.ExpectedState()) == states.end())
            states.insert(rule.ExpectedState());
        if (states.find(rule.NextState()) == states.end())
            states.insert(rule.NextState());
    }

    return validator.Validate(_rules, states, _finalState);
}

template<typename State, typename InputValue, typename UnderlyingType, typename InputSpecifier>
void NFA<State, InputValue, UnderlyingType, InputSpecifier>::UpdateStateMap()
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
inline void PrintTo(const NFA<State, InputValue, UnderlyingType, InputSpecifier> & term, std::ostream & stream)
{
    term.PrintTo(stream);
}

} // namespace Regex

template<typename State, typename InputValue, typename UnderlyingType, typename InputSpecifier>
inline std::ostream & operator << (std::ostream & stream, const Regex::NFA<State, InputValue, UnderlyingType, InputSpecifier> & term)
{
    term.PrintTo(stream);
    return stream;
}

