#pragma once

namespace Core
{

template <class T>
class IObservable
{
public:
    virtual ~IObservable() {}

    virtual void add(T * observer) = 0;
    virtual void remove(T * observer) = 0;
};

} // namespace Core
