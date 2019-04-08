/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#include "remotingts1/windowseventloop.h"
#include <cassert>
#include <climits>
#include <cstdint>
#include <memory>
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
# pragma warning (disable : 4265) // Suppress warning from Microsoft headers
#endif
#include <condition_variable>
#include <functional>
#include <mutex>
#if !defined(_MSC_VER) || (_MSC_VER < 1700) || (_MSC_VER >= 1900)
# include <thread>
#endif
#ifdef _MSC_VER
# pragma warning (default : 4265)
#endif
#include <queue>
#include <set>
#include <string>

#ifdef _WIN32
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
# undef min
# undef max
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#endif

namespace RemotingTs1 {

using std::chrono::duration_cast;

#ifdef _WIN32

struct WindowsEventLoop::Item
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

	void *handle;
    uint64_t priority;
    std::shared_ptr<std::function<void()>> function;
};

struct WindowsEventLoop::Private
{
    static const wchar_t className[];
    static volatile long instances;
    static volatile long classRegistred;
    static const unsigned messageId;

    HANDLE queueEmpty;
    unsigned itemCounter;

    CRITICAL_SECTION mutex;
    DWORD threadId;
    HWND window;
	std::set<const Timer *> timers;

	bool running;
	int exitCode;
    std::priority_queue<Item> events;
	size_t eventPostCount;

	inline void lock() { ::EnterCriticalSection(&mutex); }
	inline void unlock() { ::LeaveCriticalSection(&mutex); }
};

const wchar_t WindowsEventLoop::Private::className[] = L"RemotingTs1::WindowsEventLoop";
volatile long WindowsEventLoop::Private::instances = 0;
volatile long WindowsEventLoop::Private::classRegistred = 0;
const unsigned WindowsEventLoop::Private::messageId = WM_USER;

WindowsEventLoop::WindowsEventLoop()
  : p(new Private())
{
    p->queueEmpty = ::CreateEvent(NULL, TRUE, TRUE, NULL);
    p->itemCounter = 0;

    ::InitializeCriticalSection(&p->mutex);

    p->threadId = ::GetCurrentThreadId();
    p->window = nullptr;
	p->running = false;
	p->exitCode = 0;
	p->eventPostCount = 0;

    if (::InterlockedIncrement(&Private::instances) == 1)
    {
        struct T
        {
            static LRESULT WINAPI WndProc(HWND window, UINT msg, WPARAM wp, LPARAM lp)
            {
				if (msg != WM_CREATE)
				{
                	WindowsEventLoop * const me = reinterpret_cast<WindowsEventLoop *>(::GetWindowLongPtr(window, GWLP_USERDATA));
                    if (me)
					{
						if (msg == Private::messageId) 
						{
 							return me->handleEvent();
						}
						else if (msg == WM_TIMER)
						{
							std::unique_lock<Private> l(*me->p);

							Timer * const timer = reinterpret_cast<Timer *>(wp);
							if (me->p->timers.find(timer) != me->p->timers.end())
							{
								l.unlock();

								timer->timeout();
							}
						}
						else if (msg == WM_CLOSE)
							::PostQuitMessage(0);
					}
				}
                else
                    ::SetWindowLongPtr(window, GWLP_USERDATA, LONG_PTR(reinterpret_cast<LPCREATESTRUCT>(lp)->lpCreateParams));

                return ::DefWindowProc(window, msg, wp, lp);
            }
        };

        WNDCLASSEXW wndClass;
        memset(&wndClass, 0, sizeof(wndClass));
        wndClass.cbSize = sizeof(wndClass);
        wndClass.lpfnWndProc = &T::WndProc;
        wndClass.hInstance = reinterpret_cast<HINSTANCE>(&__ImageBase);
        wndClass.lpszClassName = p->className;

        // Wait for any previous unregister to complete first.
        while (Private::classRegistred != 0) ::Sleep(0);

        ::RegisterClassExW(&wndClass);
        ::InterlockedIncrement(&Private::classRegistred);
    }

    // Wait for register to complete first (if performed in another thread).
    while (Private::classRegistred == 0) ::Sleep(0);

    p->window = ::CreateWindowExW(
        0, p->className, NULL, 
        0, 0, 0, 0, 0, 
        HWND_MESSAGE, NULL, 
        reinterpret_cast<HINSTANCE>(&__ImageBase), 
        this);
}

WindowsEventLoop::~WindowsEventLoop()
{
    if (p->window != nullptr)
        ::DestroyWindow(p->window);

    if (InterlockedDecrement(&Private::instances) <= 0)
    {
        if (Private::instances == 0)
        {
            ::UnregisterClassW(p->className, reinterpret_cast<HINSTANCE>(&__ImageBase));
            ::InterlockedDecrement(&Private::classRegistred);
        }
        else
            Private::instances = 0;
    }

    ::DeleteCriticalSection(&p->mutex);

    ::CloseHandle(p->queueEmpty);

	assert(p->timers.empty());

    delete p;
    *const_cast<Private **>(&p) = nullptr;
}

void * WindowsEventLoop::handle()
{
	return p->window;
}

int WindowsEventLoop::run(unsigned milliseconds)
{
    assert(::GetCurrentThreadId() == p->threadId);
    if (::GetCurrentThreadId() == p->threadId)
	{
		p->running = true;
		for (const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();;)
		{
			MSG msg = {0};
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
					return int(msg.wParam);

				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}

			if (!p->running)
				return p->exitCode;

			if (milliseconds != unsigned(-1))
			{
				const int64_t wait = int64_t(milliseconds) - duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
				if ((wait <= 0) || (::MsgWaitForMultipleObjects(0, NULL, FALSE, DWORD(wait), QS_ALLEVENTS) == WAIT_TIMEOUT))
					return -1;
			}
			else
				::MsgWaitForMultipleObjects(0, NULL, FALSE, INFINITE, QS_ALLEVENTS);
		}
	}

    return -1;
}

void WindowsEventLoop::stop(int code)
{
	post(nullptr, [this, code]
	{
		p->exitCode = code;
		p->running = false;
	}, Priority::Idle);
}

void WindowsEventLoop::wait(void) const
{
    if (p->threadId && (p->threadId != ::GetCurrentThreadId()))
        ::WaitForSingleObject(p->queueEmpty, INFINITE);
}

bool WindowsEventLoop::checkThread(void) const
{
    return ::GetCurrentThreadId() == p->threadId;
}

bool WindowsEventLoop::running(void) const
{
	return true;
}

void WindowsEventLoop::post(std::function<void()> function, Priority priority)
{
	WindowsEventLoop::post(this, std::move(function), priority);
}

void WindowsEventLoop::send(std::function<void()> function, Priority priority)
{
    if (p->window != nullptr)
    {
		if (!WindowsEventLoop::checkThread())
		{
			HANDLE event = ::CreateEvent(NULL, TRUE, FALSE, NULL);

			{
				std::lock_guard<Private> l(*p);

				p->events.emplace(this, priority, p->itemCounter++, [this, &function, event]
				{
					function();

					::SetEvent(event);
				});

				::ResetEvent(p->queueEmpty);

				if (::PostMessageW(p->window, p->messageId, 0, 0))
					p->eventPostCount++;
				else if (p->eventPostCount == 0) // This will deadlock, so abort.
					throw std::runtime_error("message queue failure.");
			}

			::WaitForSingleObject(event, INFINITE);
			::CloseHandle(event);
		}
		else
			function();
    }
}

void WindowsEventLoop::post(void *handle, std::function<void()> function, Priority priority)
{
    if (p->window != nullptr)
    {
        std::lock_guard<Private> l(*p);

        p->events.emplace(handle, priority, p->itemCounter++, std::move(function));
        ::ResetEvent(p->queueEmpty);

        if (::PostMessageW(p->window, p->messageId, 0, 0))
			p->eventPostCount++;
    }
}

bool WindowsEventLoop::eventsPending() const
{
    std::lock_guard<Private> l(*p);

	return !p->events.empty();
}

void WindowsEventLoop::cancel(void *handle)
{
	send([this, handle]
	{
		std::lock_guard<Private> l(*p);

		std::priority_queue<Item> newQueue;
		while (!p->events.empty())
		{
            const auto &item = p->events.top();
			if (item.handle != handle)
                newQueue.emplace(item);

			p->events.pop();
		}

		p->events = newQueue;

		if (p->events.empty())
		{
			p->itemCounter = 0;
			::SetEvent(p->queueEmpty);
		}
	}, Priority::TimeCritical);
}

void WindowsEventLoop::startTimer(Timer &timer, unsigned milliseconds)
{
    std::lock_guard<Private> l(*p);

	const UINT elapse = UINT(std::max(unsigned(USER_TIMER_MINIMUM), std::min(milliseconds, unsigned(USER_TIMER_MAXIMUM))));

	if (::SetTimer(p->window, UINT_PTR(&timer), elapse, NULL) != 0)
		p->timers.insert(&timer);
}

void WindowsEventLoop::stopTimer(const Timer &timer)
{
    std::lock_guard<Private> l(*p);

	auto i = p->timers.find(&timer);
	if (i != p->timers.end())
	{
		::KillTimer(p->window, UINT_PTR(&timer));
		p->timers.erase(i);
	}
}

int WindowsEventLoop::handleEvent()
{
    std::unique_lock<Private> l(*p);

	assert(p->eventPostCount > 0);
	if (p->eventPostCount > 0)
		p->eventPostCount--;

	// Handle extra events if needed, this can happen as Windows limits the 
	// maximum number of events in the message queue.
	while (p->events.size() > p->eventPostCount)
    {
		// Extra scope as the destructor of a lambda expression may invoke a destructor.
		bool unlocked = false;
		{
            const auto item = p->events.top();
			p->events.pop();

            if (item.function && *item.function)
			{
				l.unlock();
				unlocked = true;

                (*item.function)();
			}
		}

		if (unlocked)
			l.lock();
    }

    if (p->events.empty())
    {
        p->itemCounter = 0;
        ::SetEvent(p->queueEmpty);
    }

    return 0;
}

#endif

} // End of namespace
