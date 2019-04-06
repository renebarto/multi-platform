/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifdef _MSC_VER
# ifndef _CRT_SECURE_NO_WARNINGS
#  define _CRT_SECURE_NO_WARNINGS 1
# endif
# ifndef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
#  define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
# endif
# ifndef _WIN32_WINNT_WINTHRESHOLD
#  define _WIN32_WINNT_WINTHRESHOLD           0x0601
# endif
# ifndef NTDDI_WIN7SP1
#  define NTDDI_WIN7SP1                       0x0601010
# endif
#endif

#include "sscfts1/eventloop.h"
#include <cassert>
#include <climits>
#include <cstdint>
#ifdef _MSC_VER
# pragma warning (disable : 4265) // Suppress warning from Microsoft headers
#endif
#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#if !defined(_MSC_VER) || (_MSC_VER < 1700) || (_MSC_VER >= 1900)
# include <thread>
#endif
#ifdef _MSC_VER
# pragma warning (default : 4265)
#endif
#include <map>
#include <memory>
#include <queue>
#include <string>

#ifdef _MSC_VER
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
# undef min
# undef max
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#endif

namespace SscfTs1 {

using std::chrono::duration_cast;

struct EventLoop::Item
{
    Item() 
	  :	handle(nullptr), 
		priority(0), 
		function() 
	{
	}

    Item(void *h, Priority p, unsigned c, std::function<void()> f) 
	  :	handle(h), 
		priority(((uint64_t(p) + INT_MAX + 1) << 32) | (UINT_MAX - c)), 
        function(std::make_shared<std::function<void()>>(std::move(f)))
	{
	}

	bool operator<(const Item &c) const { return priority < c.priority; }

	void * handle;
    uint64_t priority;
    std::shared_ptr<std::function<void()>> function;
};

struct EventLoop::Private
{
    std::string threadName;

	std::mutex mutex;
#if !defined(_MSC_VER) || (_MSC_VER >= 1900)
    std::unique_ptr<std::thread> thread;
	std::thread::id threadId;
#else // Circumvent bug in Microsoft C++ runtime library: https://connect.microsoft.com/VisualStudio/feedback/details/845184/std-thread-hides-exception-information-e-x-call-stack-if-exception-is-thrown-from-the-user-thread-entry-point
	HANDLE thread;
	DWORD threadId;
#endif

#ifndef _MSC_VER
	std::condition_variable queueFilledEvent;
#else // Circumvent bug in Microsoft C++ runtime library: https://connect.microsoft.com/VisualStudio/feedback/details/762560/crash-in-runtime-library-msvcr110-dll
	HANDLE queueFilledEvent;
#endif
    std::condition_variable queueEmptyEvent;
    unsigned itemCounter;

    bool running;
    int exitCode;

	struct TimerInfo 
	{ 
		std::chrono::milliseconds interval;
		// Bug in VS2012 and VS2013 implementation of std::chrono::steady_clock.
		// See also: https://connect.microsoft.com/VisualStudio/feedback/details/753115/
#if !defined(_MSC_VER) || ((_MSC_VER < 1700) || (_MSC_VER >= 1900))
		std::chrono::steady_clock::time_point nextEvent;
#else
		uint64_t nextEvent;
#endif
	};

	std::map<Timer *, TimerInfo> timers;

    std::priority_queue<Item> events;

#if !defined(_MSC_VER) || ((_MSC_VER < 1700) || (_MSC_VER >= 1900)) // Bug in VS2012 and VS2013 implementation of std::chrono::steady_clock.
    std::chrono::steady_clock::time_point lastEvent;
#else
    uint64_t lastEvent;
#endif
};

EventLoop::EventLoop(const std::string &name)
  : p(new Private())
{
    p->threadName = !name.empty() ? name : "SSCF EventLoop thread";

#if defined(_MSC_VER) && (_MSC_VER < 1900)
	p->thread = INVALID_HANDLE_VALUE;
	p->threadId = 0;
#endif
    p->itemCounter = 0;
    p->running = false;
    p->exitCode = 0;

#ifdef _MSC_VER // Circumvent bug in Microsoft C++ runtime library
	p->queueFilledEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
#endif
}

EventLoop::~EventLoop()
{
    EventLoop::stop();

#if !defined(_MSC_VER) || (_MSC_VER >= 1900)
	if (p->thread)
    {
        p->thread->join();
        p->thread = nullptr;
    }
#else
	if (p->thread != INVALID_HANDLE_VALUE)
    {
		::WaitForSingleObject(p->thread, INFINITE);
		::CloseHandle(p->thread);
		p->thread = INVALID_HANDLE_VALUE;
		p->threadId = 0;
    }
#endif

#ifdef _MSC_VER // Circumvent bug in Microsoft C++ runtime library
	::CloseHandle(p->queueFilledEvent);
#endif

	assert(p->timers.empty());

    delete p;
    *const_cast<Private **>(&p) = nullptr;
}

void EventLoop::start(void)
{
    std::lock_guard<std::mutex> _(p->mutex);

#if !defined(_MSC_VER) || (_MSC_VER >= 1900)
    if (p->thread == nullptr)
    {
        p->running = true;

        struct T
        {
            static void threadProc(EventLoop *me)
            { 
#ifdef _MSC_VER
                // Give the thread a name.
                {
#pragma pack(push, 8)
					const struct { DWORD type; LPCSTR name; DWORD threadId; DWORD flags; } info = { 0x1000, me->p->threadName.c_str(), DWORD(-1), 0 };
#pragma pack(pop)
					__try { ::RaiseException(0x406D1388, 0, sizeof(info) / sizeof(DWORD), (ULONG_PTR *)&info); } __except(EXCEPTION_CONTINUE_EXECUTION) { }
				}
#endif

                me->eventThread(unsigned(-1));
            }
        };

        p->thread.reset(new std::thread(&T::threadProc, this));
    }
#else
	if (p->thread == INVALID_HANDLE_VALUE)
    {
        p->running = true;

        struct T
        {
            static DWORD WINAPI threadProc(LPVOID arg)
            { 
				EventLoop * const me = reinterpret_cast<EventLoop *>(arg);

                // Give the thread a name.
                {
#pragma pack(push, 8)
					const struct { DWORD type; LPCSTR name; DWORD threadId; DWORD flags; } info = { 0x1000, me->p->threadName.c_str(), DWORD(-1), 0 };
#pragma pack(pop)
					__try { ::RaiseException(0x406D1388, 0, sizeof(info) / sizeof(DWORD), (ULONG_PTR *)&info); } __except(EXCEPTION_CONTINUE_EXECUTION) { }
				}

                me->eventThread(unsigned(-1));
				return 0;
            }
        };

		p->thread = ::CreateThread(NULL, 0, &T::threadProc, this, 0, &p->threadId);
    }
#endif
}

const std::string & EventLoop::name(void) const 
{ 
	return p->threadName; 
}

void EventLoop::start(
#ifdef _MSC_VER
        Priority priority)
#else
        Priority)
#endif
{
    start();
#if defined(_MSC_VER) && (_MSC_VER >= 1900)
    if (p->thread != nullptr)
        ::SetThreadPriority(p->thread->native_handle(), int(priority));
#elif defined(_MSC_VER)
    if (p->thread != INVALID_HANDLE_VALUE)
        ::SetThreadPriority(p->thread, int(priority));
#endif
}

int EventLoop::run(unsigned milliseconds)
{
    p->running = true;
#if !defined(_MSC_VER) || (_MSC_VER >= 1900)
	p->threadId = std::this_thread::get_id();
#else
	p->threadId = ::GetCurrentThreadId();
#endif

    eventThread(milliseconds);

    return p->exitCode;
}

void EventLoop::stop(int code)
{
	std::unique_lock<std::mutex> l(p->mutex);

	if (p->running)
    {
		p->exitCode = code;
		p->running = false;
#ifndef _MSC_VER
		p->queueFilledEvent.notify_one();
#else // Circumvent bug in Microsoft C++ runtime library
		::SetEvent(p->queueFilledEvent);
#endif

		l.unlock();

#if !defined(_MSC_VER) || (_MSC_VER >= 1900)
        if (p->thread && (p->thread->get_id() != std::this_thread::get_id()))
        {
            p->thread->join();
            p->thread = nullptr;
        }
#else
		if ((p->thread != INVALID_HANDLE_VALUE) && (p->threadId != ::GetCurrentThreadId()))
        {
			::WaitForSingleObject(p->thread, INFINITE);
			::CloseHandle(p->thread);
			p->thread = INVALID_HANDLE_VALUE;
			p->threadId = 0;
		}
#endif
    }
}

void EventLoop::wait(void) const
{
	std::unique_lock<std::mutex> l(p->mutex);

#if !defined(_MSC_VER) || (_MSC_VER >= 1900)
    if (p->running && p->thread)
    {
        if (p->thread->get_id() != std::this_thread::get_id())
        {
            while (p->itemCounter > 0)
                p->queueEmptyEvent.wait(l);
        }
    }
#else
	if (p->running && (p->thread != INVALID_HANDLE_VALUE))
    {
        if (p->threadId != ::GetCurrentThreadId())
        {
            while (p->itemCounter > 0)
                p->queueEmptyEvent.wait(l);
        }
    }
#endif
}

bool EventLoop::checkThread(void) const
{
	std::lock_guard<std::mutex> _(p->mutex);

    if (p->running)
	{
#if !defined(_MSC_VER) || (_MSC_VER >= 1900)
		if (p->thread)
			return p->thread->get_id() == std::this_thread::get_id();
		else
			return p->threadId == std::this_thread::get_id();
#else
		return p->threadId == ::GetCurrentThreadId();
#endif
	}

    return true;
}

bool EventLoop::running(void) const
{
	std::lock_guard<std::mutex> _(p->mutex);

	return p->running;
}

void EventLoop::post(std::function<void()> function, Priority priority)
{
    EventLoop::post(this, std::move(function), priority);
}

void EventLoop::send(std::function<void()> function, Priority priority)
{
	std::unique_lock<std::mutex> l(p->mutex);

#if !defined(_MSC_VER) || (_MSC_VER >= 1900)
	const bool onThread = p->thread
		? (p->thread->get_id() == std::this_thread::get_id())
		: (p->threadId == std::this_thread::get_id());
#else
	const bool onThread = p->threadId == ::GetCurrentThreadId();
#endif

	if (p->running && !onThread)
	{
		// Not on the EventLoop thread, a send is required.
		std::condition_variable event;
		bool finished = false;

		p->events.emplace(
			this, 
			priority, 
			p->itemCounter++, 
			[this, &function, &finished, &event]
		{
			function();

			std::lock_guard<std::mutex> _(p->mutex);

			finished = true;
			event.notify_one();
		});

#ifndef _MSC_VER
		p->queueFilledEvent.notify_one();
#else // Circumvent bug in Microsoft C++ runtime library
		::SetEvent(p->queueFilledEvent);
#endif

		while (!finished)
			event.wait(l);
	}
	else
	{
		// From the same thread or not running, simply invoke the delegate.
		l.unlock();

		function();
	}
}

void EventLoop::post(void *handle, std::function<void()> function, Priority priority)
{
    std::lock_guard<std::mutex> _(p->mutex);

    p->events.emplace(handle, priority, p->itemCounter++, std::move(function));
#ifndef _MSC_VER
	p->queueFilledEvent.notify_one();
#else // Circumvent bug in Microsoft C++ runtime library
	::SetEvent(p->queueFilledEvent);
#endif
}

bool EventLoop::eventsPending() const
{
    std::lock_guard<std::mutex> _(p->mutex);

	return !p->events.empty();
}

void EventLoop::cancel(void *handle)
{
    send([this, handle]
	{
		// Make sure these items are destructed outside the lock.
		std::vector<Item> obsoleteItems;
		{
			std::lock_guard<std::mutex> _(p->mutex);

			std::priority_queue<Item> newQueue;
			while (!p->events.empty())
			{
                const auto &item = p->events.top();
                if (item.handle != handle)
                    newQueue.emplace(item);
				else
                    obsoleteItems.emplace_back(item);

				p->events.pop();
			}

			p->events = newQueue;
		}
	}, Priority::TimeCritical);
}

void EventLoop::startTimer(Timer &timer, unsigned milliseconds)
{
	std::lock_guard<std::mutex> _(p->mutex);

	Private::TimerInfo &timerInfo = p->timers[&timer];
	timerInfo.interval = std::chrono::milliseconds(milliseconds);
#if !defined(_MSC_VER) || ((_MSC_VER < 1700) || (_MSC_VER >= 1900)) // Bug in VS2012 and VS2013 implementation of std::chrono::steady_clock.
	timerInfo.nextEvent = std::chrono::steady_clock::now() + timerInfo.interval;
#else
	timerInfo.nextEvent = ::GetTickCount64() + timerInfo.interval.count();
#endif
}

void EventLoop::stopTimer(const Timer &timer)
{
	std::lock_guard<std::mutex> _(p->mutex);

	p->timers.erase(const_cast<Timer *>(&timer));
}

void EventLoop::eventThread(unsigned milliseconds)
{
	std::unique_lock<std::mutex> l(p->mutex);

#if !defined(_MSC_VER) || ((_MSC_VER < 1700) || (_MSC_VER >= 1900)) // Bug in VS2012 and VS2013 implementation of std::chrono::steady_clock.
	const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
#else
	const uint64_t start = ::GetTickCount64();
#endif

    do
	{
#if !defined(_MSC_VER) || ((_MSC_VER < 1700) || (_MSC_VER >= 1900)) // Bug in VS2012 and VS2013 implementation of std::chrono::steady_clock.
		const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
		std::chrono::steady_clock::time_point nextEvent = now;
#else
		const uint64_t now = ::GetTickCount64();
		uint64_t nextEvent = now;
#endif

		// Get the next elapsing timer and all elapsed timers.
		const Timer *nextTimer = nullptr;
		std::vector<Timer *> elapsed;
		for (auto &i : p->timers)
		{
			if (i.second.nextEvent <= now)
			{
				do
				{
#if !defined(_MSC_VER) || ((_MSC_VER < 1700) || (_MSC_VER >= 1900)) // Bug in VS2012 and VS2013 implementation of std::chrono::steady_clock.
					i.second.nextEvent += i.second.interval;
#else
					i.second.nextEvent += i.second.interval.count();
#endif
				} while (i.second.nextEvent <= now);

				elapsed.push_back(i.first);
			}

            if ((nextTimer == nullptr) || (i.second.nextEvent < nextEvent))
			{
				nextEvent = i.second.nextEvent;
				nextTimer = i.first;
			}
		}

		l.unlock();

		// Invoke elapsed timers.
		for (const auto &i : elapsed)
			i->timeout();

		// Handle events.
		if (milliseconds != unsigned(-1))
		{
            std::chrono::milliseconds wait = 
#if !defined(_MSC_VER) || ((_MSC_VER < 1700) || (_MSC_VER >= 1900)) // Bug in VS2012 and VS2013 implementation of std::chrono::steady_clock.
				std::chrono::milliseconds(milliseconds) - 
				duration_cast<std::chrono::milliseconds>(now - start);
#else
				std::chrono::milliseconds(milliseconds - (now - start));
#endif

			if (wait.count() <= 0)
			{
				l.lock();
				p->exitCode = -1;
				p->running = false;
				break;
			}
			else
			{
				if (nextTimer)
				{
#if !defined(_MSC_VER) || ((_MSC_VER < 1700) || (_MSC_VER >= 1900)) // Bug in VS2012 and VS2013 implementation of std::chrono::steady_clock.
					wait = std::min(wait, duration_cast<std::chrono::milliseconds>(nextEvent - now));
#else
					wait = std::min(wait, std::chrono::milliseconds(nextEvent - now));
#endif
				}

				handleEvents(unsigned(wait.count())); 
			}
		}
		else if (nextTimer)
		{
#if !defined(_MSC_VER) || ((_MSC_VER < 1700) || (_MSC_VER >= 1900)) // Bug in VS2012 and VS2013 implementation of std::chrono::steady_clock.
			handleEvents(unsigned(duration_cast<std::chrono::milliseconds>(nextEvent - now).count()));
#else
			handleEvents(unsigned(nextEvent - now));
#endif
		}
		else
			handleEvents(milliseconds); 

		l.lock();
	} while (p->running);
}

void EventLoop::handleEvents(unsigned milliseconds)
{
#ifndef _MSC_VER
    std::unique_lock<std::mutex> l(p->mutex);
	if (milliseconds == unsigned(-1))
	{
		while (p->running && p->events.empty())
			p->queueFilledEvent.wait(l);
	}
	else for (const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now(); p->running && p->events.empty();)
	{
        const std::chrono::milliseconds wait = 
			std::chrono::milliseconds(milliseconds) - 
			duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);

        if ((wait.count() <= 0) || (p->queueFilledEvent.wait_for(l, wait) == std::cv_status::timeout))
			return;
	}
#else // Circumvent bug in Microsoft C++ runtime library
	if (::WaitForSingleObject(p->queueFilledEvent, (milliseconds == unsigned(-1)) ? INFINITE : milliseconds) == WAIT_TIMEOUT)
		return;

    std::unique_lock<std::mutex> l(p->mutex);
#endif

    while (!p->events.empty())
    {
		// Extra scope as the destructor of a lambda expression may invoke a destructor.
		bool unlocked = false;
		{
			auto function = p->events.top().function;
			p->events.pop();

            if (function && *function)
			{
                l.unlock();
				unlocked = true;

                (*function)();
			}
		}

		if (unlocked)
            l.lock();

#if !defined(_MSC_VER) || ((_MSC_VER < 1700) || (_MSC_VER >= 1900)) // Bug in VS2012 and VS2013 implementation of std::chrono::steady_clock.
        p->lastEvent = std::chrono::steady_clock::now();
#else
		p->lastEvent = ::GetTickCount64();
#endif
    }

    p->itemCounter = 0;
    p->queueEmptyEvent.notify_all();
}

} // End of namespace
