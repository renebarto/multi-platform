/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifndef SSCFTS1_EVENTLOOP_H
#define SSCFTS1_EVENTLOOP_H

#include "ieventloop.h"
#include "sscf/exports.h"

namespace SscfTs1 {

/*! This class creates a thread that handles events asynchronously.
 */
class SSCF_EXPORT EventLoop : public IEventLoop
{
public:
    explicit EventLoop(const std::string &name = std::string());
    virtual ~EventLoop();

    /*! Returns the name of the event loop.
     */
    const std::string & name(void) const;

    /*! Starts the thread.
     */
    void start(void);

    /*! Starts the thread with a specific priority.
     */
    void start(Priority);
    
    /*! Runs the eventloop on the current thread.
		\param	milliseconds	The timeout in ms after which run() will exit with -1.
     */
    int run(unsigned milliseconds = -1);

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
    EventLoop(const EventLoop &);
    EventLoop & operator=(const EventLoop &);

    void eventThread(unsigned);
    void handleEvents(unsigned);

private:
	struct Item;
	struct Private;
	Private * const p;
};

} // End of namespaces

#endif
