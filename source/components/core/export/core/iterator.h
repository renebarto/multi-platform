#pragma once

#include "core/core.h"
#include "osal/Exception.h"

namespace Core {

template<class Container>
class iterator
{
public:
    using Element = typename Container::value_type;
    iterator(Container & container)
        : _begin(container.begin())
        , _current(_begin)
        , _end(container.end())
    {
    }
    bool AtBegin() const { return _current == _begin; }
    bool AtEnd() const { return _current == _end; }
    void Reset() { _current = _begin; }
    Element & operator++()
    {
        if (!AtEnd())
            ++_current;
        return operator *();
    }
    Element & operator++(int)
    {
        Element & result = operator *();
        if (!AtEnd())
            ++_current;
        return result;
    }

    Element * operator ->()
    {
        if ((_current < _begin) || (_current >= _end))
            throw OSAL::RuntimeError(__FUNCTION__, __FILE__, __LINE__, "iterator outside range");

        return _current;
    }
    Element & operator *()
    {
        if ((_current < _begin) || (_current >= _end))
            throw OSAL::RuntimeError(__FUNCTION__, __FILE__, __LINE__, "iterator outside range");

        return *_current;
    }

private:
    typename Container::iterator _begin;
    typename Container::iterator _current;
    typename Container::iterator _end;
};

template<class Container>
class constiterator
{
public:
    using Element = typename Container::value_type;
    constiterator(const Container & container)
        : _begin(container.begin())
        , _current(_begin)
        , _end(container.end())
    {
    }
    bool AtBegin() const { return _current == _begin; }
    bool AtEnd() const { return _current == _end; }
    void Reset() { _current = _begin; }
    const Element & operator++()
    {
        if (!AtEnd())
            ++_current;
        return operator *();
    }
    const Element & operator++(int)
    {
        const Element & result = operator *();
        if (!AtEnd())
            ++_current;
        return result;
    }

    const Element * operator ->()
    {
        if ((_current < _begin) || (_current >= _end))
            throw OSAL::RuntimeError(__FUNCTION__, __FILE__, __LINE__, "iterator outside range");

        return _current;
    }
    const Element & operator *() const
    {
        if ((_current < _begin) || (_current >= _end))
            throw OSAL::RuntimeError(__FUNCTION__, __FILE__, __LINE__, "iterator outside range");

        return *_current;
    }

private:
    typename Container::const_iterator _begin;
    typename Container::const_iterator _current;
    typename Container::const_iterator _end;
};

} // namespace Core