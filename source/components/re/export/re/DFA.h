#pragma once

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include "ValueSet.h"

namespace RE {

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
using DFARuleSet = std::vector<DFARule<State, InputValue, UnderlyingType, InputSpecifier>>;

template<typename State, typename InputValue = char, typename UnderlyingType = int, typename InputSpecifier = ValueSet<InputValue, UnderlyingType>>
class DFA
{
public:
    DFA(State initialState, State finalState)
        : _rules()
        , _initialState(initialState)
        , _finalState(finalState)
        , _currentState(_initialState)
    {}
    DFA(const DFARuleSet<State, InputValue, UnderlyingType, InputSpecifier> & rules, State initialState, State finalState)
        : _rules(rules)
        , _initialState(initialState)
        , _finalState(finalState)
        , _currentState(_initialState)
    {}
    DFA(const DFA & other)
        : _rules(other._rules)
        , _initialState(other._initialState)
        , _finalState(other._finalState)
        , _currentState(other._currentState)
    {}
    DFA(DFA && other)
        : _rules(std::move(other._rules))
        , _initialState(other._initialState)
        , _finalState(other._finalState)
        , _currentState(other._currentState)
    {}

    DFA & operator = (const DFA & other)
    {
        if (this != &other)
        {
            _rules = other._rules;
            _initialState = other._initialState;
            _finalState = other._finalState;
            _currentState = other._currentState;
        }
        return *this;
    }
    DFA & operator = (DFA && other)
    {
        if (this != &other)
        {
            _rules = std::move(other._rules);
            _initialState = other._initialState;
            _finalState = other._finalState;
            _currentState = other._currentState;
        }
        return *this;
    }

    void AddRule(const DFARule<State, InputValue, UnderlyingType, InputSpecifier> & rule)
    {
        _rules.push_back(rule);
    }
    bool Validate() const;
    void Reset() { _currentState = _initialState; }
    bool HandleInput(InputValue input)
    {
        for (auto const & rule : _rules)
        {
            if (rule.Match(input) && _currentState == rule.ExpectedState())
            {
                _currentState = rule.NextState();
                return true;
            }
        }
        return false;
    }
    State CurrentState() const { return _currentState; }
    bool IsFinalState() const { return _currentState == _finalState; }

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
        ExportToDot(stream);
    }

protected:
    DFARuleSet<State, InputValue, UnderlyingType, InputSpecifier> _rules;
    State _initialState;
    State _finalState;
    State _currentState;
};

template<typename State, typename InputValue = char, typename UnderlyingType = int, typename InputSpecifier = ValueSet<InputValue, UnderlyingType>>
class DFAValidator
{
public:
    DFAValidator()
        : _map()
    {}

    bool Validate(const DFARuleSet<State, InputValue, UnderlyingType, InputSpecifier> & rules,
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
    bool Validate(const DFARuleSet<State, InputValue, UnderlyingType, InputSpecifier> & rules,
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

    return validator.Validate(_rules, states, _finalState);
}

template<typename State, typename InputValue, typename UnderlyingType, typename InputSpecifier>
inline void PrintTo(const DFA<State, InputValue, UnderlyingType, InputSpecifier> & term, std::ostream & stream)
{
    term.PrintTo(stream);
}

} // namespace RE

template<typename State, typename InputValue, typename UnderlyingType, typename InputSpecifier>
inline std::ostream & operator << (std::ostream & stream, const RE::DFA<State, InputValue, UnderlyingType, InputSpecifier> & term)
{
    term.PrintTo(stream);
    return stream;
}

