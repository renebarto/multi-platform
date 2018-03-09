#pragma once

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include "InputSet.h"

namespace RE {

template<typename State, typename InputValue = char, typename InputSpecifier = InputSet<InputValue>>
class FSARule
{
public:
    FSARule() = delete;

    static FSARule Create(InputSpecifier input, State state, State nextState)
    {
        return FSARule(input, state, nextState);
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
    FSARule(InputSpecifier input, State state, State nextState)
        : _expectedInput(input)
        , _expectedState(state)
        , _nextState(nextState)
    {}
    FSARule(const FSARule & other)
        : _expectedInput(other._expectedInput)
        , _expectedState(other._expectedState)
        , _nextState(other._nextState)
    {
    }
    FSARule & operator = (const FSARule & other)
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

template<typename State, typename InputValue = char, typename InputSpecifier = InputSet<InputValue>>
using FSARuleSet = std::vector<FSARule<State, InputValue, InputSpecifier>>;

template<typename State, typename InputValue = char, typename InputSpecifier = InputSet<InputValue>>
class FSA
{
public:
    FSA(const FSARuleSet<State, InputValue, InputSpecifier> & rules)
        : _rules(rules)
        , _initialState()
        , _finalState(_initialState)
        , _currentState(_initialState)
    {}
    FSA(const FSARuleSet<State, InputValue, InputSpecifier> & rules, State initialState)
        : _rules(rules)
        , _initialState(initialState)
        , _finalState(_initialState)
        , _currentState(_initialState)
    {}
    FSA(const FSARuleSet<State, InputValue, InputSpecifier> & rules, State initialState, State finalState)
        : _rules(rules)
        , _initialState(initialState)
        , _finalState(finalState)
        , _currentState(_initialState)
    {}
    FSA(const FSA & other)
        : _rules(other._rules)
        , _initialState(other._initialState)
        , _finalState(other._finalState)
        , _currentState(other._currentState)
    {}
    FSA(FSA && other)
        : _rules(std::move(other._rules))
        , _initialState(other._initialState)
        , _finalState(other._finalState)
        , _currentState(other._currentState)
    {}

    FSA & operator = (const FSA & other)
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
    FSA & operator = (FSA && other)
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

protected:
    const FSARuleSet<State, InputValue, InputSpecifier> & _rules;
    State _initialState;
    State _finalState;
    State _currentState;
};

template<typename State, typename InputValue = char, typename InputSpecifier = InputSet<InputValue>>
class FSAValidator
{
public:
    FSAValidator()
        : _map()
    {}

    bool Validate(const FSARuleSet<State, InputValue, InputSpecifier> & rules,
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
    bool Validate(const FSARuleSet<State, InputValue, InputSpecifier> & rules,
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

template<typename State, typename InputValue, typename InputSpecifier>
bool FSA<State, InputValue, InputSpecifier>::Validate() const
{
    FSAValidator<State, InputValue, InputSpecifier> validator;
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


} // namespace RE
