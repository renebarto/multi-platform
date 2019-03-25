#pragma once

#include <map>
#include <vector>
#include <core/iterator.h>
#include "json/Value.h"

namespace JSON {

class KVPair
{
public:
    KVPair();
    KVPair(const std::string & key, const ValuePtr & value);

    const std::string & GetKey() const { return _key; }
    void SetKey(const std::string & value) { _key = value; }
    const ValuePtr GetValue() const { return _value; }
    void SetValue(const ValuePtr value) { _value = value; }
    bool Deserialize(std::basic_istream<char> & stream);
    void Serialize(std::basic_ostream<char> & stream, int indentDepth = 0) const;

private:
    std::string _key;
    ValuePtr _value;
};

using KVList = std::map<std::string, ValuePtr>;

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

    virtual bool Deserialize(std::basic_istream<char> & stream) override;
    virtual void Serialize(std::basic_ostream<char> & stream, int indentDepth = 0, bool indentInitial = true) const override;

    virtual ValueType Type() const override { return ValueType::Object; }
    virtual void Set(ValuePtr other) override;
    virtual std::shared_ptr<Value> Clone() const;
    bool AddPair(const KVPair & pair);
    ValuePtr Find(const std::string & name);
    const ValuePtr Find(const std::string & name) const;
    size_t Size() const { return _elements.size(); }
    JSON::ValuePtr operator [] (const std::string & key) override;

    Iterator GetIterator() { return Iterator(_elements); }
    ConstIterator GetIterator() const { return ConstIterator(_elements); }
    KVList GetValue() const { return _elements; }

    bool EqualTo(std::shared_ptr<Value> other) const override;

private:
    KVList _elements;
};

} // namepsace JSON
