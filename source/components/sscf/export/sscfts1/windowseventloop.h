/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifndef SSCFTS1_WINDOWSEVENTLOOP_H
#define SSCFTS1_WINDOWSEVENTLOOP_H

#include "ieventloop.h"

namespace SscfTs1 {

#ifdef _WIN32

/*! This class wraps the Windows message queue.
 */
class WindowsEventLoop : public IEventLoop
{
public:
    WindowsEventLoop();
    virtual ~WindowsEventLoop();
    
	/*! Returns the native window handle used by this WindowsEventLoop.
	 */
	void * handle();
    
    /*! Runs the eventloop on the current thread, returns when the WM_QUIT
        message is received. This will simply run a 
        GetMessage/TranslateMessage/DispatchMessage loop, the WindowsEventLoop
        will also function properly if this is done manually.
		\param	milliseconds	The timeout in ms after which run() will exit with -1.
     */
    int run(unsigned milliseconds = unsigned(-1));

public: // From IEventLoop
    virtual bool checkThread(void) const override;
	virtual bool running(void) const override;
    virtual void post(std::function<void()>, Priority = Priority::Normal) override;
	virtual void send(std::function<void()>, Priority = Priority::Normal) override;
	virtual bool eventsPending() const override final;
    virtual void wait(void) const override;
    virtual void stop(int exitCode = 0) override;

protected: // From IEventLoop
    virtual void post(void *, std::function<void()>, Priority = Priority::Normal) override;
    virtual void cancel(void *) override;
	virtual void startTimer(Timer &, unsigned milliseconds) override;
	virtual void stopTimer(const Timer &) override;

private:
    WindowsEventLoop(const WindowsEventLoop &);
    WindowsEventLoop & operator=(const WindowsEventLoop &);

    int handleEvent();

private:
	struct Item;
	struct Private;
	Private * const p;
};

#endif

} // End of namespaces

#endif
