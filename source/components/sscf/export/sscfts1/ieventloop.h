/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifndef SSCFTS1_IEVENTLOOP_H
#define SSCFTS1_IEVENTLOOP_H

#include "event.h"
#ifdef _MSC_VER
# pragma warning (disable : 4265) // Suppress warning from Microsoft headers
#endif
#include <functional>
#ifdef _MSC_VER
# pragma warning (default : 4265)
#endif
#include <string>

namespace SscfTs1 {

class EventLoopProxy;

class SSCF_EXPORT IEventLoop
{
friend class EventLoopProxy;
public:
    enum class Priority
    {
        Idle = -15,
        Lowest = -2,
        BelowNormal = -1,
        Normal = 0,
        AboveNormal = 1,
        Highest = 2,
        TimeCritical = 15
    };

	class Timer
	{
	public:
		explicit Timer(IEventLoop &);
		~Timer();

        /*! Starts the timer with the specified interval in milliseconds.
         */
		void start(unsigned milliseconds);

        /*! Stops the timer, pending timeout events shall be discarded. 
         */
		void stop();

        /*! Returns true if the timer is running.
         */
        bool running() const { return started; }

        /*! This event shall be raised periodically when the timer is running.
         */
		Event0 timeout;

	private:
		Timer(const Timer &);
		Timer(Timer &&);
		Timer & operator=(const Timer &);
		Timer & operator=(Timer &&);

	private:
		IEventLoop &eventLoop;
		bool started;
	};

public:
    virtual ~IEventLoop();

    /*! Returns true if the method is called from the event loop thread.
     */
    virtual bool checkThread(void) const = 0;

    /*! Returns true if the event loop is running.
     */
	virtual bool running(void) const = 0;
	
    /*! Posts an event to the thread.
     */
	virtual void post(std::function<void()>, Priority = Priority::Normal) = 0;

    /*! Sends an event to the thread and waits for it to be handled.
     */
    virtual void send(std::function<void()>, Priority = Priority::Normal) = 0;

	/*! Returns true if more events are pending (excluding the currently
		executing event).
	 */
	virtual bool eventsPending() const = 0;
       
    /*! Waits for any posted events to be handled.
     */
    virtual void wait(void) const = 0;

    /*! Stops the eventloop, waits for any posted events to be handled first.
     */
    virtual void stop(int exitCode) = 0;

protected:
    /*! Posts an event to the thread. The handle can be used to cancel the event.
     */
    virtual void post(void *handle, std::function<void()>, Priority = Priority::Normal) = 0;

    /*! Cancels all posted events with the specified handle.
     */
    virtual void cancel(void *handle) = 0;

	/*! Starts a timer with the specified timeout.
	 */
	virtual void startTimer(Timer &, unsigned milliseconds) = 0;

	/*! Stops the specified timer.
	 */
	virtual void stopTimer(const Timer &) = 0;

private:
    /*! Should return the root IEventLoop. The default implementation returns
		*this, only the EventLoopProxy returns parent.
     */
	virtual IEventLoop & root();
};

/*! This class wraps an eventloop and wil automatically ensure all posted
	events are canceled upon destruction.
 */
class EventLoopProxy final : public IEventLoop
{
public:
    explicit EventLoopProxy(IEventLoop &parent);
    virtual ~EventLoopProxy() final;

public: // From IEventLoop
    virtual bool checkThread(void) const override final;
	virtual bool running(void) const override final;
	virtual void post(std::function<void()>, Priority = Priority::Normal) override final;
	virtual void send(std::function<void()>, Priority = Priority::Normal) override final;
	virtual bool eventsPending() const override final;
    virtual void wait(void) const override final;
    virtual void stop(int exitCode = 0) override final;

protected: // From IEventLoop
	virtual void post(void *, std::function<void()>, Priority = Priority::Normal) override final;
	virtual void cancel(void *) override final;
	virtual void startTimer(Timer &, unsigned milliseconds) override final;
	virtual void stopTimer(const Timer &) override final;

private: // From IEventLoop
	virtual IEventLoop & root() override final;

private:
    EventLoopProxy(const EventLoopProxy &);
    EventLoopProxy & operator=(const EventLoopProxy &);

private:
	IEventLoop &parent;
};

} // End of namespaces

#endif
