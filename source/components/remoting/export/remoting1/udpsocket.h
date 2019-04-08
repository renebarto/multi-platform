/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifndef REMOTING1_UDPSOCKET_H
#define REMOTING1_UDPSOCKET_H

#include <cstdint>
#include "tcpsocket.h"

namespace Remoting1 {

/*! This class provide asynchronous udp socket functionality. All asynchronous
    socket I/O is handled on the communication thread.
 */
class UdpSocket
{
public:
    struct SendCallback
    {
		SendCallback();
		virtual ~SendCallback();

        /*! Is invoked when the specified buffer has been sent successfully.
            Note the beginSend copies the provided buffer, therefore the
            provided pointer my be different.
         */
        virtual void sendSent(const char *buffer, int len) = 0;
    };

    struct ReceiveCallback
    {
		ReceiveCallback();
		virtual ~ReceiveCallback();

        /*! Is invoked when a buffer has been received.
         */
		virtual void receiveReceived(const char *buffer, int len, IpAddress address) = 0;
    };

public:
	explicit UdpSocket(IpAddress iface);
    ~UdpSocket();

    /*! Closes the socket and unblocks any blocked calls on it.
     */
    void close(void);

	/*! Enables multicast loopback.
	 */
	bool enableMulticastLoopback();

    /*! Binds the requested port, or a random port if the requested port is 0.
        If bind fails, invoke close before invoking bind again.
     */
    bool bind(ReceiveCallback *, uint16_t port, bool multicast = false);

    /*! Binds the requested multicast group, port, and interface. If bind
        fails, invoke close before invoking bind again.
     */
	bool bind(ReceiveCallback *, IpAddress group, uint16_t port);

    /*! Returns the bound address and port.
     */
	const IpAddress & address(void) const;

    /*! Atomically sends all data in the specified buffer.
     */
	void beginSendTo(SendCallback *, IpAddress address, const char *buffer, int len = 0);

    /*! Atomically sends all data in the specified buffers.
     */
	void beginSendTo(SendCallback *, IpAddress address, const char * const *buffer, const int *len, int count);

private:
    UdpSocket(const UdpSocket &);
    UdpSocket & operator=(const UdpSocket &);

private:
    struct Private;
    Private * const p;
};

} // End of namespaces

#endif
