#pragma once

#include <map>
#include <vector>
#include <core/Iterator.h>
#include "json/Value.h"

namespace JSON {

class KVPair
{
public:
    KVPair();
    KVPair(const OSAL::String & key, const ValuePtr & value);

    const OSAL::String & GetKey() const { return _key; }
    void SetKey(const OSAL::String & value) { _key = value; }
    const ValuePtr GetValue() const { return _value; }
    void SetValue(const ValuePtr value) { _value = value; }
    bool Deserialize(std::basic_istream<OSAL::Char> & stream);
    void Serialize(std::basic_ostream<OSAL::Char> & stream, int indentDepth = 0) const;

private:
    OSAL::String _key;
    ValuePtr _value;
};

using KVList = std::map<OSAL::String, ValuePtr>;

class Object : public Value
{
public:
    Object();
    Object(const std::initializer_list<KVPair> & elements);
    virtual ~Object() {};

    class Iterator
    {
    public:
        Iterator(KVList & container)
            : _begin(container.begin())
            , _current(_begin)
            , _end(container.end())
        {
        }
        bool AtBegin() const { return _current == _begin; }
        bool AtEnd() const { return _current == _end; }
        void Reset() { _current = _begin; }
        KVPair operator++()
        {
            if (!AtEnd())
                ++_current;
            return operator *();
        }
        KVPair operator++(int)
        {
            KVPair result = operator *();
            if (!AtEnd())
                ++_current;
            return std::move(result);
        }

        KVPair operator *()
        {
            if (AtEnd())
                throw OSAL::RuntimeError(__FUNCTION__, __FILE__, __LINE__, "Iterator outside scope");

            return KVPair(_current->first, _current->second);
        }

    private:
        KVList::iterator _begin;
        KVList::iterator _current;
        KVList::iterator _end;
    };

    class ConstIterator
    {
    public:
        ConstIterator(const KVList & container)
            : _begin(container.begin())
            , _current(_begin)
            , _end(container.end())
        {
        }
        bool AtBegin() const { return _current == _begin; }
        bool AtEnd() const { return _current == _end; }
        void Reset() { _current = _begin; }
        KVPair operator++()
        {
            if (!AtEnd())
                ++_current;
            return operator *();
        }
        KVPair operator++(int)
        {
            KVPair result = operator *();
            if (!AtEnd())
                ++_current;
            return std::move(result);
        }

        KVPair operator *()
        {
            if (AtEnd())
                throw OSAL::RuntimeError(__FUNCTION__, __FILE__, __LINE__, "Iterator outside scope");

            return KVPair(_current->first, _current->second);
        }

    private:
        KVList::const_iterator _begin;
        KVList::const_iterator _current;
        KVList::const_iterator _end;
    };

    virtual bool Deserialize(std::basic_istream<OSAL::Char> & stream) override;
    virtual void Serialize(std::basic_ostream<OSAL::Char> & stream, int indentDepth = 0, bool indentInitial = true) const override;

    virtual ValueType Type() const override { return ValueType::Object; }
    bool AddPair(const KVPair & pair);
    ValuePtr Find(const OSAL::String & name);
    const ValuePtr Find(const OSAL::String & name) const;
    size_t Size() const { return _elements.size(); }
    Iterator GetIterator() { return Iterator(_elements); }
    ConstIterator GetIterator() const { return ConstIterator(_elements); }

private:
    KVList _elements;
};

} // namepsace JSON
