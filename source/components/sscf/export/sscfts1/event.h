/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifndef SSCFTS1_EVENT_H
#define SSCFTS1_EVENT_H

#include <cstdint>
#ifdef _MSC_VER
# pragma warning (disable : 4265) // Suppress warning from Microsoft headers
#endif
#include <functional>
#ifdef _MSC_VER
# pragma warning (default : 4265)
#endif
#include <map>
#include <set>
#include <vector>
#include "sscf/exports.h"

// This circumvents a compiler bug
#ifdef _MSC_VER
# pragma warning(disable:4505)
#endif

namespace std { class recursive_mutex; }

namespace SscfTs1 {

class EventHandler;

class SSCF_EXPORT EventBase
{
friend class EventHandler;
public:
    typedef unsigned Subscription;

protected:
#if !defined(__INTELLISENSE__) || defined(SSCFTS1_BUILD) // Hide internals from IntelliSense
    EventBase();
    virtual ~EventBase();

    EventBase(EventBase &&);
	EventBase & operator=(EventBase &&);

    virtual void unsubscribe(Subscription subscription) const = 0;

    static std::recursive_mutex & acquireMutex();
    static void releaseMutex();

	struct SSCF_EXPORT Lock { Lock(void *); Lock(const Lock &); ~Lock(); void unlock(); mutable void *m; };
	Lock lock() const;

	void checkThread(Lock &) const;

private:
    EventBase(const EventBase &);
    EventBase & operator=(const EventBase &);

private:
    std::recursive_mutex &mutex;
    mutable std::set<EventHandler *> destroyed;
	mutable void *thread_id;
#endif // __INTELLISENSE__
};

/*! The Event is a wrapper around a list of function objects that can be used
    to multicast events. use an EventHandler to subscribe to an event. The
    EventHandler class manages connections and automatically unsubscribes when
	it is destructed.
    \code
        class MyClass : protected EventHandler
        {
            explicit MyClass(IInterfaceWithEvents &iface)
            {
                // Automatically unsubscribed when this class is destructed.
                subscribe(iface.event1, &MyClass::event1);
                subscribe(iface.event2, [this](const int &v1, const int &v2) 
                { ... });
            }

            void event1(const int &v1) 
            { ... }
        };
    \endcode
 */
template <class _signature>
class Event : public EventBase
{
friend class Dispatcher;
friend class EventHandler;
protected:
#if !defined(__INTELLISENSE__) || defined(SSCFTS1_BUILD) // Hide internals from IntelliSense
    inline Event() : EventBase(), nextId(1) { }

    Event(Event &&);
	Event & operator=(Event &&);

    /*! Subscribes a function to this event. A handle is returned which can be
		used to unsubscribe later.
        \note This method is thread-safe.
     */
	Subscription subscribe(const std::function<_signature> &) const;

    /*! Subscribes a function to this event. A handle is returned which can be
		used to unsubscribe later.
        \note This method is thread-safe.
     */
	Subscription subscribe(std::function<_signature> &&) const;

    /*! Unsubscribes from this event.
        \note This method is thread-safe.
     */
    virtual void unsubscribe(Subscription subscription) const override final;

private:
    Event(const Event &);
    Event & operator=(const Event &);

protected:
    mutable Subscription nextId;
	mutable std::map<unsigned, std::function<_signature>> functions;
#endif // __INTELLISENSE__
};

#if !defined(__INTELLISENSE__) || defined(SSCFTS1_BUILD) // Hide internals from IntelliSense

template <class _signature>
Event<_signature>::Event(Event &&from)
	: EventBase(std::move(static_cast<EventBase &&>(from))),
	  nextId(from.nextId),
	  functions(std::move(from.functions))
{
}

template <class _signature>
Event<_signature> & Event<_signature>::operator=(Event &&from)
{
	EventBase::operator=(std::move(static_cast<EventBase &&>(from)));
	nextId = from.nextId;
	functions = std::move(from.functions);

	return *this;
}

template <class _signature>
EventBase::Subscription Event<_signature>::subscribe(const std::function<_signature> &function) const
{
    Lock l = lock();
	checkThread(l);

    const Subscription subscription = nextId++;
	functions[subscription] = function;
	return subscription;
}

template <class _signature>
EventBase::Subscription Event<_signature>::subscribe(std::function<_signature> &&function) const
{
    Lock l = lock();
	checkThread(l);

    const Subscription subscription = nextId++;
	functions.emplace(subscription, std::move(function));
	return subscription;
}

template <class _signature>
void Event<_signature>::unsubscribe(Subscription subscription) const
{
    Lock l = lock();
	checkThread(l);

	functions.erase(subscription);
}

#endif // __INTELLISENSE__

/*! Creates an Event with 0 arguments.
 */
class SSCF_EXPORT Event0 : public Event<void()>
{
public:
	inline Event0() : Event<void()>() { }

	Event0(Event0 &&from) : Event(std::move(from)) { }
	Event0 & operator=(Event0 &&from) { Event::operator=(std::move(from)); return *this; }

	void operator()() { return invoke(); }

private:
    Event0(const Event0 &);
    Event0 & operator=(const Event0 &);

	void invoke();
};

/*! Creates an Event with 1 arguments.
 */
template <typename _prm1>
class Event1 : public Event<void(const _prm1 &)>
{
public:
    inline Event1() : Event<void(const _prm1 &)>() { }

    Event1(Event1 &&from) : Event<void(const _prm1 &)>(std::move(from)) { }
    Event1 & operator=(Event1 &&from) { Event<void(const _prm1 &)>::operator=(std::move(from)); return *this; }

	void operator()(const _prm1 &prm1);

private:
    Event1(const Event1 &);
    Event1 & operator=(const Event1 &);
};

/*! Creates an Event with 2 arguments.
 */
template <typename _prm1, typename _prm2>
class Event2 : public Event<void(const _prm1 &, const _prm2 &)>
{
public:
    inline Event2() : Event<void(const _prm1 &, const _prm2 &)>() { }
	
    Event2(Event2 &&from) : Event<void(const _prm1 &, const _prm2 &)>(std::move(from)) { }
    Event2 & operator=(Event2 &&from) { Event<void(const _prm1 &, const _prm2 &)>::operator=(std::move(from)); return *this; }

    void operator()(const _prm1 &prm1, const _prm2 &prm2);

private:
    Event2(const Event2 &);
    Event2 & operator=(const Event2 &);
};

/*! Creates an Event with 3 arguments.
 */
template <typename _prm1, typename _prm2, typename _prm3>
class Event3 : public Event<void(const _prm1 &, const _prm2 &, const _prm3 &)>
{
public:
    inline Event3() : Event<void(const _prm1 &, const _prm2 &, const _prm3 &)>() { }
	
    Event3(Event3 &&from) : Event<void(const _prm1 &, const _prm2 &, const _prm3 &)>(std::move(from)) { }
    Event3 & operator=(Event3 &&from) { Event<void(const _prm1 &, const _prm2 &, const _prm3 &)>::operator=(std::move(from)); return *this; }

    void operator()(const _prm1 &prm1, const _prm2 &prm2, const _prm3 &prm3);

private:
    Event3(const Event3 &);
    Event3 & operator=(const Event3 &);
};

/*! Creates an Event with 4 arguments.
 */
template <typename _prm1, typename _prm2, typename _prm3, typename _prm4>
class Event4 : public Event<void(const _prm1 &, const _prm2 &, const _prm3 &, const _prm4 &)>
{
public:
    inline Event4() : Event<void(const _prm1 &, const _prm2 &, const _prm3 &, const _prm4 &)>() { }
	
    Event4(Event4 &&from) : Event<void(const _prm1 &, const _prm2 &, const _prm3 &, const _prm4 &)>(std::move(from)) { }
    Event4 & operator=(Event4 &&from) { Event<void(const _prm1 &, const _prm2 &, const _prm3 &, const _prm4 &)>::operator=(std::move(from)); return *this; }

    void operator()(const _prm1 &prm1, const _prm2 &prm2, const _prm3 &prm3, const _prm4 &prm4);

private:
    Event4(const Event4 &);
    Event4 & operator=(const Event4 &);
};

/*! Creates an Event with 5 arguments.
 */
template <typename _prm1, typename _prm2, typename _prm3, typename _prm4, typename _prm5>
class Event5 : public Event<void(const _prm1 &, const _prm2 &, const _prm3 &, const _prm4 &, const _prm5 &)>
{
public:
    inline Event5() : Event<void(const _prm1 &, const _prm2 &, const _prm3 &, const _prm4 &, const _prm5 &)>() { }
	
    Event5(Event5 &&from) : Event<void(const _prm1 &, const _prm2 &, const _prm3 &, const _prm4 &, const _prm5 &)>(std::move(from)) { }
    Event5 & operator=(Event5 &&from) { Event<void(const _prm1 &, const _prm2 &, const _prm3 &, const _prm4 &, const _prm5 &)>::operator=(std::move(from)); return *this; }

    void operator()(const _prm1 &prm1, const _prm2 &prm2, const _prm3 &prm3, const _prm4 &prm4, const _prm5 &prm5);

private:
    Event5(const Event5 &);
    Event5 & operator=(const Event5 &);
};

#if !defined(__INTELLISENSE__) || defined(SSCFTS1_BUILD) // Hide internals from IntelliSense

template <typename _prm1>
void Event1<_prm1>::operator()(const _prm1 &prm1)
{
    EventBase::Lock l = EventBase::lock();
    EventBase::checkThread(l);

    const auto tmp = Event<void(const _prm1 &)>::functions;
    
	l.unlock();

    for (const auto &i : tmp)
        i.second(prm1);
}

template <typename _prm1, typename _prm2>
void Event2<_prm1, _prm2>::operator()(const _prm1 &prm1, const _prm2 &prm2)
{
    EventBase::Lock l = EventBase::lock();
    EventBase::checkThread(l);

	const auto tmp = Event<void(const _prm1 &, const _prm2 &)>::functions;
    
	l.unlock();

    for (const auto &i : tmp)
        i.second(prm1, prm2);
}

template <typename _prm1, typename _prm2, typename _prm3>
void Event3<_prm1, _prm2, _prm3>::operator()(const _prm1 &prm1, const _prm2 &prm2, const _prm3 &prm3)
{
    EventBase::Lock l = EventBase::lock();
    EventBase::checkThread(l);

	const auto tmp = Event<void(const _prm1 &, const _prm2 &, const _prm3 &)>::functions;
    
	l.unlock();

    for (const auto &i : tmp)
        i.second(prm1, prm2, prm3);
}

template <typename _prm1, typename _prm2, typename _prm3, typename _prm4>
void Event4<_prm1, _prm2, _prm3, _prm4>::operator()(const _prm1 &prm1, const _prm2 &prm2, const _prm3 &prm3, const _prm4 &prm4)
{
    EventBase::Lock l = EventBase::lock();
    EventBase::checkThread(l);

	const auto tmp = Event<void(const _prm1 &, const _prm2 &, const _prm3 &, const _prm4 &)>::functions;
    
	l.unlock();

    for (const auto &i : tmp)
        i.second(prm1, prm2, prm3, prm4);
}

template <typename _prm1, typename _prm2, typename _prm3, typename _prm4, typename _prm5>
void Event5<_prm1, _prm2, _prm3, _prm4, _prm5>::operator()(const _prm1 &prm1, const _prm2 &prm2, const _prm3 &prm3, const _prm4 &prm4, const _prm5 &prm5)
{
    EventBase::Lock l = EventBase::lock();
    EventBase::checkThread(l);

	const auto tmp = Event<void(const _prm1 &, const _prm2 &, const _prm3 &, const _prm4 &, const _prm5 &)>::functions;
    
	l.unlock();

    for (const auto &i : tmp)
        i.second(prm1, prm2, prm3, prm4, prm5);
}

#endif // __INTELLISENSE__

/*! A helper class that can be used to subscribe to events, it will
    automatically unsubscribe all connected events when it is destructed.
    \code
        class MyClass : protected EventHandler
        {
            explicit MyClass(IInterfaceWithEvents &iface)
            {
                // Automatically unsubscribed when this class is destructed.
                subscribe(iface.event1, &MyClass::event1);
                subscribe(iface.event2, [this](const int &v1, const int &v2) 
                { ... });
            }

            void event1(const int &v1) 
            { ... }
        };
    \endcode
 */
class SSCF_EXPORT EventHandler
{
friend class EventBase;
public:
    EventHandler();
    ~EventHandler();

    /*! Subscribes a function to an event, use a lambda expression or
        std::bind to create the function object.
        \code
            subscribe(iface.event2, [this](const int &v1, const int &v2) 
            { ... });
        \endcode
        \note This method is thread-safe.
     */
	template <typename _signature, typename _func>
	EventBase::Subscription subscribe(const Event<_signature> &event, const _func &func)
    {
		return addSubscription(event, event.subscribe(func));
    }

    /*! Subscribes a function to an event, use a lambda expression or
        std::bind to create the function object.
        \code
            subscribe(iface.event2, [this](const int &v1, const int &v2) 
            { ... });
        \endcode
        \note This method is thread-safe.
     */
	template <typename _signature, typename _func>
	EventBase::Subscription subscribe(const Event<_signature> &event, _func &&func)
    {
		return addSubscription(event, event.subscribe(std::move(func)));
    }

    EventBase::Subscription subscribe(const Event0 &event, Event0 &dest);

    template <typename _prm1>
    EventBase::Subscription subscribe(const Event1<_prm1> &event, Event1<_prm1> &dest)
    {
		using namespace std::placeholders;

		return subscribe(event, std::bind(&Event1<_prm1>::operator(), &dest, _1));
    }

    template <typename _prm1, typename _prm2>
    EventBase::Subscription subscribe(const Event2<_prm1, _prm2> &event, Event2<_prm1, _prm2> &dest)
    {
		using namespace std::placeholders;

		return subscribe(event, std::bind(&Event2<_prm1, _prm2>::operator(), &dest, _1, _2));
    }

    template <typename _prm1, typename _prm2, typename _prm3>
    EventBase::Subscription subscribe(const Event3<_prm1, _prm2, _prm3> &event, Event3<_prm1, _prm2, _prm3> &dest)
    {
		using namespace std::placeholders;

		return subscribe(event, std::bind(&Event3<_prm1, _prm2, _prm3>::operator(), &dest, _1, _2, _3));
    }

    template <typename _prm1, typename _prm2, typename _prm3, typename _prm4>
    EventBase::Subscription subscribe(const Event4<_prm1, _prm2, _prm3, _prm4> &event, Event4<_prm1, _prm2, _prm3, _prm4> &dest)
    {
		using namespace std::placeholders;

		return subscribe(event, std::bind(&Event4<_prm1, _prm2, _prm3, _prm4>::operator(), &dest, _1, _2, _3, _4));
    }

#if !defined(_MSC_VER) || (_MSC_VER >= 1800) || (_VARIADIC_MAX >= 6)
    template <typename _prm1, typename _prm2, typename _prm3, typename _prm4, typename _prm5>
    EventBase::Subscription subscribe(const Event5<_prm1, _prm2, _prm3, _prm4, _prm5> &event, Event5<_prm1, _prm2, _prm3, _prm4, _prm5> &dest)
    {
		using namespace std::placeholders;

		return subscribe(event, std::bind(&Event5<_prm1, _prm2, _prm3, _prm4, _prm5>::operator(), &dest, _1, _2, _3, _4, _5));
    }
#endif
protected:
    /*! Subscribes a member-function to an event.
        \code
            subscribe(iface.event1, &MyClass::event1);
        \endcode
        \note This method is thread-safe.
     */
    template <class _object>
    EventBase::Subscription subscribe(const Event0 &event, void(_object::* func)())
    {
		return subscribe(event, std::bind(func, static_cast<_object *>(this)));
    }

    /*! Subscribes a member-function to an event.
        \code
            subscribe(iface.event1, &MyClass::event1);
        \endcode
        \note This method is thread-safe.
     */
    template <class _object, typename _prm1>
    EventBase::Subscription subscribe(const Event1<_prm1> &event, void(_object::* func)(const _prm1 &))
    {
		using namespace std::placeholders;

		return subscribe(event, std::bind(func, static_cast<_object *>(this), _1));
    }

    /*! Subscribes a member-function to an event.
        \code
            subscribe(iface.event1, &MyClass::event1);
        \endcode
        \note This method is thread-safe.
     */
    template <class _object, typename _prm1, typename _prm2>
    EventBase::Subscription subscribe(const Event2<_prm1, _prm2> &event, void(_object::* func)(const _prm1 &, const _prm2 &))
    {
		using namespace std::placeholders;

		return subscribe(event, std::bind(func, static_cast<_object *>(this), _1, _2));
    }

    /*! Subscribes a member-function to an event.
        \code
            subscribe(iface.event1, &MyClass::event1);
        \endcode
        \note This method is thread-safe.
     */
    template <class _object, typename _prm1, typename _prm2, typename _prm3>
    EventBase::Subscription subscribe(const Event3<_prm1, _prm2, _prm3> &event, void(_object::* func)(const _prm1 &, const _prm2 &, const _prm3 &))
    {
		using namespace std::placeholders;

		return subscribe(event, std::bind(func, static_cast<_object *>(this), _1, _2, _3));
    }

    /*! Subscribes a member-function to an event.
        \code
            subscribe(iface.event1, &MyClass::event1);
        \endcode
        \note This method is thread-safe.
     */
    template <class _object, typename _prm1, typename _prm2, typename _prm3, typename _prm4>
    EventBase::Subscription subscribe(const Event4<_prm1, _prm2, _prm3, _prm4> &event, void(_object::* func)(const _prm1 &, const _prm2 &, const _prm3 &, const _prm4 &))
    {
		using namespace std::placeholders;

		return subscribe(event, std::bind(func, static_cast<_object *>(this), _1, _2, _3, _4));
    }

#if !defined(_MSC_VER) || (_MSC_VER >= 1800) || (_VARIADIC_MAX >= 6)
    /*! Subscribes a member-function to an event.
        \code
            subscribe(iface.event1, &MyClass::event1);
        \endcode
        \note This method is thread-safe.
     */
    template <class _object, typename _prm1, typename _prm2, typename _prm3, typename _prm4, typename _prm5>
    EventBase::Subscription subscribe(const Event5<_prm1, _prm2, _prm3, _prm4, _prm5> &event, void(_object::* func)(const _prm1 &, const _prm2 &, const _prm3 &, const _prm4 &, const _prm5 &))
    {
		using namespace std::placeholders;

		return subscribe(event, std::bind(func, static_cast<_object *>(this), _1, _2, _3, _4, _5));
    }
#endif
private:
#if !defined(__INTELLISENSE__) || defined(SSCFTS1_BUILD) // Hide internals from IntelliSense
    EventHandler(const EventHandler &);
    EventHandler & operator=(const EventHandler &);

    EventBase::Subscription addSubscription(const EventBase &, EventBase::Subscription);
    void onDestroyed(EventBase &);
    void onMoved(EventBase &, EventBase &);

private:
    std::recursive_mutex &mutex;
    typedef std::map<const EventBase *, std::set<EventBase::Subscription>> Subscriptions;
    Subscriptions subscriptions;
#endif // __INTELLISENSE__
};

} // End of namespaces

#endif
