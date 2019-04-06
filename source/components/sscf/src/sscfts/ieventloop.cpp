/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#include "sscfts1/ieventloop.h"
#include "sscf1/system.h"
#include <cassert>
#include <climits>
#include <cstdint>
#ifdef _MSC_VER
# pragma warning (disable : 4265) // Suppress warning from Microsoft headers
#endif
#include <functional>
#ifdef _MSC_VER
# pragma warning (default : 4265)
#endif

namespace SscfTs1 {

IEventLoop::~IEventLoop()
{
}

IEventLoop & IEventLoop::root()
{
	return *this;
}

IEventLoop::Timer::Timer(IEventLoop &eventLoop)
	: eventLoop(eventLoop),
	  started(false)
{
}

IEventLoop::Timer::~Timer()
{
	stop();
}

void IEventLoop::Timer::start(unsigned milliseconds)
{
	if (Sscf1::system().threadSafetyChecksEnabled() &&
		!eventLoop.checkThread())
	{
		Sscf1::system().fail("A timer should be started on its own thread");
	}

	stop();

	eventLoop.startTimer(*this, milliseconds);
	started = true;
}

void IEventLoop::Timer::stop()
{
	if (Sscf1::system().threadSafetyChecksEnabled() &&
		!eventLoop.checkThread())
	{
		Sscf1::system().fail("A timer should be started on its own thread");
	}

	if (started)
	{
		eventLoop.stopTimer(*this);
		started = false;
	}
}

EventLoopProxy::EventLoopProxy(IEventLoop &parent)
	: parent(parent.root())
{
}

EventLoopProxy::~EventLoopProxy()
{
	if (Sscf1::system().threadSafetyChecksEnabled() &&
		!parent.checkThread())
	{
		Sscf1::system().fail(
			"Make sure to destroy the EventLoopProxy on its own thread, "
			"otherwise events may still be running while destructing it.");
	}

	cancel(this);
}

bool EventLoopProxy::checkThread(void) const
{
	return parent.checkThread();
}

bool EventLoopProxy::running(void) const
{
	return parent.running();
}

void EventLoopProxy::post(std::function<void()> function, Priority priority)
{
	EventLoopProxy::post(this, std::move(function), priority);
}

void EventLoopProxy::send(std::function<void()> function, Priority priority)
{
	parent.send(std::move(function), priority); 
}

bool EventLoopProxy::eventsPending() const
{
	return parent.eventsPending();
}

void EventLoopProxy::wait(void) const
{
	parent.wait(); 
}

void EventLoopProxy::stop(int exitCode)
{
	parent.stop(exitCode); 
}

void EventLoopProxy::post(void *handle, std::function<void()> function, Priority priority)
{
	parent.post(handle, std::move(function), priority); 
}

void EventLoopProxy::cancel(void *handle)
{
	parent.cancel(handle); 
}

void EventLoopProxy::startTimer(Timer &timer, unsigned milliseconds)
{
	parent.startTimer(timer, milliseconds);
}

void EventLoopProxy::stopTimer(const Timer &timer)
{
	parent.stopTimer(timer);
}

IEventLoop & EventLoopProxy::root()
{
	return parent;
}

} // End of namespace
