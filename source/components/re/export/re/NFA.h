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
using NFARuleSet = std::vector<NFARule<State, InputValue, UnderlyingType, InputSpecifier>>;

template<typename State, typename InputValue = char, typename UnderlyingType = int, typename InputSpecifier = ValueSet<InputValue, UnderlyingType>>
class NFA
{
public:
    NFA(State initialState, State finalState)
        : _rules()
        , _initialState(initialState)
        , _finalState(finalState)
        , _currentState(_initialState)
    {}
    NFA(const NFA & other)
        : _rules(other._rules)
        , _initialState(other._initialState)
        , _finalState(other._finalState)
        , _currentState(other._currentState)
    {}
    NFA(NFA && other)
        : _rules(std::move(other._rules))
        , _initialState(other._initialState)
        , _finalState(other._finalState)
        , _currentState(other._currentState)
    {}

    NFA & operator = (const NFA & other)
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
    NFA & operator = (NFA && other)
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

    void AddRule(const NFARule<State, InputValue, UnderlyingType, InputSpecifier> & rule)
    {
        _rules.push_back(rule);
    }
    bool Validate() const;
    void Reset() { _currentState = _initialState; }
    bool HandleInput(InputValue input)
    {
        std::vector<NFARule<int>> alternatives;
        for (auto const & rule : _rules)
        {
            if (rule.Match(input) && _currentState == rule.ExpectedState())
            {
                alternatives.push_back(rule);
            }
        }
        if (alternatives.empty())
            return false;
        srand(time(nullptr));
        int randomNumber = rand();
        int option = randomNumber / (RAND_MAX / alternatives.size());
        _currentState = alternatives[option].NextState();
        return true;
    }
    bool HandleEpsilon()
    {
        std::vector<NFARule<int>> alternatives;
        for (auto const & rule : _rules)
        {
            if (rule.IsEpsilon() && _currentState == rule.ExpectedState())
            {
                alternatives.push_back(rule);
            }
        }
        if (alternatives.empty())
            return false;
        srand(time(nullptr));
        int option = rand() / (RAND_MAX / alternatives.size());
        _currentState = alternatives[option].NextState();
        return true;
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
    NFARuleSet<State, InputValue, UnderlyingType, InputSpecifier> _rules;
    State _initialState;
    State _finalState;
    State _currentState;
};

template<typename State, typename InputValue = char, typename UnderlyingType = int, typename InputSpecifier = ValueSet<InputValue, UnderlyingType>>
class NFAValidator
{
public:
    NFAValidator()
        : _map()
    {}

    bool Validate(const NFARuleSet<State, InputValue, UnderlyingType, InputSpecifier> & rules,
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
    bool Validate(const NFARuleSet<State, InputValue, UnderlyingType, InputSpecifier> & rules,
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

