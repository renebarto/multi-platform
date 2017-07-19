#pragma once

#include <list>
#include <mutex>
#include "core/IObservable.h"

namespace Core
{

template <class T>
class Observable : public IObservable<T>
{
public:
    typedef std::recursive_mutex Mutex;
    typedef std::lock_guard<Mutex> Lock;

    virtual void AddObserver(T * observer);
    virtual void RemoveObserver(T * observer);

    std::recursive_mutex & GetMutex()
    {
        return _guard;
    }
    typename std::list<T *>::iterator begin()
    {
        return _observers.begin();
    }
    typename std::list<T *>::iterator end()
    {
        return _observers.end();
    }

protected:
    std::list<T *> _observers;
    typename std::list<T *>::iterator Find(T * observer);
    Mutex _guard;
};

template <class T>
void Observable<T>::AddObserver(T * observer)
{
    Lock lock(_guard);
    typename std::list<T *>::iterator it = Find(observer);
    if (it == _observers.end())
    {
        _observers.push_back(observer);
    }
}

template <class T>
void Observable<T>::RemoveObserver(T * observer)
{
    Lock lock(_guard);
    typename std::list<T *>::iterator it = Find(observer);
    if (it != _observers.end())
    {
        _observers.erase(it);
    }
}

template <class T>
typename std::list<T *>::iterator Observable<T>::Find(T * observer)
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
