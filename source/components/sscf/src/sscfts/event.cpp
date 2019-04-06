/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#define SSCFTS1_EVENT_CPP
#include "sscfts1/event.h"
#include "sscf1/system.h"
#include <cassert>
#ifdef _MSC_VER
# pragma warning (disable : 4265) // Suppress warning from Microsoft headers
#endif
#include <atomic>
#include <mutex>
#include <thread>
#ifdef _MSC_VER
# pragma warning (default : 4265)
#endif

namespace SscfTs1 {

static std::atomic<int> staticMutexSpin;
static volatile int staticMutexRefCount = 0;
static std::recursive_mutex * volatile staticMutex = nullptr;

std::recursive_mutex & EventBase::acquireMutex()
{
    int val;
    while (!staticMutexSpin.compare_exchange_strong(val = 0, 1))
        std::this_thread::yield();

    if (staticMutex == nullptr)
        staticMutex = new std::recursive_mutex();

    staticMutexRefCount++;

    staticMutexSpin = 0;

    return *staticMutex;
}

void EventBase::releaseMutex()
{
    int val;
    while (!staticMutexSpin.compare_exchange_strong(val = 0, 1))
        std::this_thread::yield();

    if (--staticMutexRefCount == 0)
    {
        delete staticMutex;
        staticMutex = nullptr;
    }

    staticMutexSpin = 0;
}

EventBase::EventBase()
  : mutex(acquireMutex()), 
    thread_id(nullptr)
{
}

EventBase::~EventBase()
{
    {
        std::lock_guard<std::recursive_mutex> l(mutex);

        for (const auto &i : destroyed)
		    i->onDestroyed(*this);

		delete reinterpret_cast<std::thread::id *>(thread_id);
		thread_id = nullptr;
    }

    releaseMutex();
}

EventBase::EventBase(EventBase &&from)
  : mutex(acquireMutex()),
    destroyed(std::move(from.destroyed)),
    thread_id(nullptr)
{
    for (const auto &i : destroyed)
		i->onMoved(from, *this);
}

EventBase & EventBase::operator=(EventBase &&from)
{
    std::lock_guard<std::recursive_mutex> l(mutex);

    for (const auto &i : destroyed)
		i->onDestroyed(*this);

	destroyed = std::move(from.destroyed);
    for (const auto &i : destroyed)
		i->onMoved(from, *this);

	return *this;
}

EventBase::Lock::Lock(void *m) : m(m) { }
EventBase::Lock::Lock(const Lock &f) : m(f.m) { f.m = nullptr; }
EventBase::Lock::~Lock() { if (m) reinterpret_cast<std::recursive_mutex *>(m)->unlock(); }
void EventBase::Lock::unlock() { if (m) reinterpret_cast<std::recursive_mutex *>(m)->unlock(); m = nullptr; }

EventBase::Lock EventBase::lock() const
{
	mutex.lock();
	return Lock(&mutex);
}

void EventBase::checkThread(Lock &) const
{
	if (Sscf1::system().threadSafetyChecksEnabled())
	{
		if (thread_id)
		{
			if (std::this_thread::get_id() !=
				*reinterpret_cast<const std::thread::id *>(thread_id))
			{
				Sscf1::system().fail(
					"Make sure to subscribe and unsubscribe events on the same "
					"thread that raises them.");
			}
		}
		else
			thread_id = new std::thread::id(std::this_thread::get_id());
	}
}

void Event0::invoke()
{
    Lock l = lock();
	checkThread(l);

	const auto tmp = functions;
    l.unlock();

    for (const auto &i : tmp)
        i.second();
}

EventHandler::EventHandler()
  : mutex(EventBase::acquireMutex())
{
}

EventHandler::~EventHandler()
{
    {
        std::lock_guard<std::recursive_mutex> l(mutex);

	    for (const auto &i : subscriptions)
	    {
		    auto j = i.first->destroyed.find(this);
		    if (j != i.first->destroyed.end())
			    i.first->destroyed.erase(j);

		    for (const auto &k : i.second)
			    i.first->unsubscribe(k);
	    }
    }

    EventBase::releaseMutex();
}

EventBase::Subscription EventHandler::subscribe(const Event0 &event, Event0 &dest)
{
	return subscribe(event, std::bind(&Event0::operator(), &dest));
}

EventBase::Subscription EventHandler::addSubscription(const EventBase &event, EventBase::Subscription subscription)
{
    std::lock_guard<std::recursive_mutex> l(mutex);

    subscriptions[&event].insert(subscription);

    if (event.destroyed.find(this) == event.destroyed.end())
        event.destroyed.insert(this);

    return subscription;
}

void EventHandler::onDestroyed(EventBase &event)
{
    std::lock_guard<std::recursive_mutex> l(mutex);

    auto i = subscriptions.find(&event);
    if (i != subscriptions.end())
        subscriptions.erase(i);
}

void EventHandler::onMoved(EventBase &from, EventBase &to)
{
    std::lock_guard<std::recursive_mutex> l(mutex);

    auto i = subscriptions.find(&from);
    if (i != subscriptions.end())
	{
		subscriptions[&to] = std::move(i->second);
        subscriptions.erase(&from);
	}
}

} // End of namespaces
