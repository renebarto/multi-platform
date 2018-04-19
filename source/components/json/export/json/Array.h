#pragma once

#include <map>
#include <vector>
#include <core/Iterator.h>
#include "json/Value.h"

namespace JSON {

using ValueList = std::vector<ValuePtr>;
class Array : public Value
{
public:
    Array();
    Array(const std::initializer_list<ValuePtr> & elements);
    virtual ~Array() {};

    class Iterator
    {
    public:
        Iterator(ValueList & container)
        : _begin(container.begin())
        , _current(_begin)
        , _end(container.end())
        {
        }
        bool AtBegin() const { return _current == _begin; }
        bool AtEnd() const { return _current == _end; }
        void Reset() { _current = _begin; }
        ValuePtr operator++()
        {
            if (!AtEnd())
                ++_current;
            return operator *();
        }
        ValuePtr operator++(int)
        {
            ValuePtr result = operator *();
            if (!AtEnd())
                ++_current;
            return std::move(result);
        }

        ValuePtr operator *()
        {
            if (AtEnd())
                throw OSAL::RuntimeError(__FUNCTION__, __FILE__, __LINE__, "Iterator outside scope");

            return *_current;
        }

    private:
        ValueList::iterator _begin;
        ValueList::iterator _current;
        ValueList::iterator _end;
    };

    class ConstIterator
    {
    public:
        ConstIterator(const ValueList & container)
        : _begin(container.begin())
        , _current(_begin)
        , _end(container.end())
        {
        }
        bool AtBegin() const { return _current == _begin; }
        bool AtEnd() const { return _current == _end; }
        void Reset() { _current = _begin; }
        ValuePtr operator++()
        {
            if (!AtEnd())
                ++_current;
            return operator *();
        }
        ValuePtr operator++(int)
        {
            ValuePtr result = operator *();
            if (!AtEnd())
                ++_current;
            return std::move(result);
        }

        ValuePtr operator *()
        {
            if (AtEnd())
                throw OSAL::RuntimeError(__FUNCTION__, __FILE__, __LINE__, "Iterator outside scope");

            return *_current;
        }

    private:
        ValueList::const_iterator _begin;
        ValueList::const_iterator _current;
        ValueList::const_iterator _end;
    };

    virtual bool Deserialize(std::istream & stream) override;
    virtual void Serialize(std::ostream & stream, int indentDepth = 0, bool indentInitial = true) const override;

    virtual ValueType Type() const override { return ValueType::Array; }

    void AddValue(ValuePtr value);
    size_t Size() const { return _elements.size(); }
    Iterator GetIterator() { return Iterator(_elements); }
    ConstIterator GetIterator() const { return ConstIterator(_elements); }

private:
    ValueList _elements;
};

} // namepsace JSON
