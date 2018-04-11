#pragma once

#include <iostream>
#include <map>
#include <set>
#include <memory>
#include <vector>
#include "ValueSet.h"

namespace RE {

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
    NFA(const NFA & other)
        : _rules(other._rules)
        , _startStateMap(other._startStateMap)
        , _endStateMap(other._endStateMap)
        , _initialState(other._initialState)
        , _finalState(other._finalState)
        , _errorState(other._errorState)
        , _currentStates(other._currentStates)
    {}
    NFA(NFA && other)
        : _rules(std::move(other._rules))
        , _startStateMap(std::move(other._startStateMap))
        , _endStateMap(std::move(other._endStateMap))
        , _initialState(other._initialState)
        , _finalState(other._finalState)
        , _errorState(other._errorState)
        , _currentStates(other._currentStates)
    {}

    NFA & operator = (const NFA & other)
    {
        if (this != &other)
        {
            _rules = other._rules;
            _startStateMap = other._startStateMap;
            _endStateMap = other._endStateMap;
            _initialState = other._initialState;
            _finalState = other._finalState;
            _errorState= other._errorState;
            _currentStates = other._currentStates;
        }
        return *this;
    }
    NFA & operator = (NFA && other)
    {
        if (this != &other)
        {
            _rules = std::move(other._rules);
            _startStateMap = std::move(other._startStateMap);
            _endStateMap = std::move(other._endStateMap);
            _initialState = other._initialState;
            _finalState = other._finalState;
            _errorState= other._errorState;
            _currentStates = other._currentStates;
        }
        return *this;
    }

    void AddRule(const NFARule<State, InputValue, UnderlyingType, InputSpecifier> & rule)
    {
        _rules.push_back(rule);
        UpdateStateMap();
    }
    const RuleSet & GetRules() const { return _rules; }
    bool FindRuleFromStateForChar(State initialState, char ch, Rule const *& rule)
    {
        for (auto const & currentRule : GetRules())
        {
            if (currentRule.ExpectedState() == initialState)
            {
                if (currentRule.ExpectedInput().Contains(ch))
                {
                    rule = &currentRule;
                    return true;
                }
                else if (currentRule.ExpectedInput().IsEmpty())
                {
                    return FindRuleFromStateForChar(currentRule.NextState(), ch, rule);
                }
            }
        }

        return false;
    }

    bool FindRuleToStateForChar(State finalState, char ch, Rule const *& rule)
    {
        for (auto const & currentRule : GetRules())
        {
            if (currentRule.NextState() == finalState)
            {
                if (currentRule.ExpectedInput().Contains(ch))
                {
                    rule = &currentRule;
                    return true;
                }
                else if (currentRule.ExpectedInput().IsEmpty())
                {
                    return FindRuleToStateForChar(currentRule.ExpectedState(), ch, rule);
                }
            }
        }

        return false;
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
                    startStates.insert(rule->NextState());
            }
        }
        StateSet endStates;
        for (auto state : startStates)
        {
            for (auto rule : _startStateMap[state])
            {
                if (rule->Match(input))
                    endStates.insert(rule->NextState());
            }
        }
        StateSet reachableStates = endStates;
        for (auto state : endStates)
        {
            for (auto rule : _startStateMap[state])
            {
                if (rule->IsEpsilon())
                    reachableStates.insert(rule->NextState());
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

} // namespace RE

template<typename State, typename InputValue, typename UnderlyingType, typename InputSpecifier>
inline std::ostream & operator << (std::ostream & stream, const RE::NFA<State, InputValue, UnderlyingType, InputSpecifier> & term)
{
    term.PrintTo(stream);
    return stream;
}

