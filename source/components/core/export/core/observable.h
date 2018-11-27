#pragma once

#include <algorithm>
#include <list>
#include <mutex>
#include <osal/osal.h>
#include "core/IObservable.h"

namespace Core
{

template <class T>
class observable : public IObservable<T>
{
public:
    typedef std::recursive_mutex Mutex;
    typedef std::lock_guard<Mutex> Lock;

    virtual void add(T * observer);
    virtual void remove(T * observer);

    size_t count() const;

    void forall(std::function<void (T *)> action);

protected:
    std::list<T *> _observers;
    typename std::list<T *>::iterator Find(T * observer);
    Mutex _guard;
};

template <class T>
size_t observable<T>::count() const
{
    return static_cast<size_t>(std::count_if(_observers.begin(), _observers.end(), [](const T *) -> bool { return true; }));
}

template <class T>
void observable<T>::add(T * observer)
{
    Lock lock(_guard);
    typename std::list<T *>::iterator it = Find(observer);
    if (it == _observers.end())
    {
        _observers.push_back(observer);
    }
}

template <class T>
void observable<T>::remove(T * observer)
{
    Lock lock(_guard);
    typename std::list<T *>::iterator it = Find(observer);
    if (it != _observers.end())
    {
        _observers.erase(it);
    }
}

template<class T>
void observable<T>::forall(std::function<void (T *)> action)
{
    for (auto const & observer : _observers)
    {
        action(observer);
    }
}

template <class T>
typename std::list<T *>::iterator observable<T>::Find(T * observer)
{
    Lock lock(_guard);
    typename std::list<T *>::iterator it = _observers.begin();
    while (it != _observers.end())
    {
        if (*it == observer)
            break;
        ++it;
    }
    return it;
}

} // namespace Core
