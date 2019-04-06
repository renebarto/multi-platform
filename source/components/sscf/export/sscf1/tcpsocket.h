/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifndef SSCF1_TCPSOCKET_H
#define SSCF1_TCPSOCKET_H

#include <cstdint>
#include <cstdlib>
#include "ipaddress.h"

namespace Sscf1 {

class Cipher;

/*! This class provide asynchronous tcp socket functionality. All asynchronous
    socket I/O is handled on the communication thread.
 */
class TcpSocket
{
public:
    struct BindCallback
    {
		BindCallback();
		virtual ~BindCallback();
	
        /*! Is invoked whe the socket is bound and a new connection has been
            accepted.
         */
        virtual void bindAccepted(TcpSocket *) = 0;
    };

    struct ConnectCallback
    {
		ConnectCallback();
		virtual ~ConnectCallback();

        /*! Is invoked whe the connect has finished.
         */
        virtual void connectFinished(bool success) = 0;
    };

    struct SendCallback
    {
		SendCallback();
		virtual ~SendCallback();

        /*! Is invoked when the sending channel has been closed before the data
            was sent.
         */
        virtual void sendClosed(void) = 0;

        /*! Is invoked when the specified buffer has been sent successfully.
            Note the beginSend copies the provided buffer, therefore the
            provided pointer my be different.
         */
        virtual void sendSent(const char *buffer, size_t len) = 0;
    };

    struct ReceiveCallback
    {
		ReceiveCallback();
		virtual ~ReceiveCallback();

        /*! Is invoked when the receiving channel is closed by the remote peer.
         */
        virtual void receiveClosed(void) = 0;

        /*! Is invoked when a buffer has been received.
         */
        virtual void receiveReceived(const char *buffer, size_t len) = 0;
    };

    struct ReceiveBulkCallback : ReceiveCallback
    {
        /*! Is invoked when a new buffer is needed. The buffer can be freed or
			reused when it is passed to receiveReceived().
         */
        virtual void acquireBuffer(char *&buffer, size_t &len) = 0;
    };

public:
	TcpSocket(IpAddress::Family family, const Cipher &);
    ~TcpSocket();

    /*! Closes the socket and unblocks any blocked calls on it.
     */
    void close(void);

    /*! Enables the TCP keepalive mechanism with the specified timeout.
     */
    bool enableKeepAlive(int timeout = 1000);

    /*! Disabled the NAGLE algorithm.
     */
    bool enableNoDelay(void);

	/*! Sets the size of the send and receive buffer of the socket.
	 */
	bool setBufferSize(uint32_t send, uint32_t receive);

    /*! Binds the requested port, or a random port if the requested port is 0.
        If bind fails, invoke close before invoking bind again.
     */
	bool bind(BindCallback *, const IpAddress &address);

	/*! Returns the bound interface address.
	 */
	const IpAddress & address(void) const;

    /*! Connects to the specified address. If connect fails, invoke close
		before invoking connect again.
     */
	bool connect(ConnectCallback *, IpAddress address);

    /*! Changes the encryption cipher for this socket.
     */
    void setCipher(const Cipher &);

    /*! Atomically sends all data in the specified buffer.
     */
    void beginSend(SendCallback *, const char *buffer, size_t len = 0);

    /*! Atomically sends all data in the specified buffers.
     */
    void beginSend(SendCallback *, const char * const *buffer, const size_t *len, int count);

    /*! Sends all bulk data in the specified buffer, optimized for transferring
		large chunks of data.
		\note The buffer needs to remain valid until sendSent is invoked.
		\note Bulk data is not encrypted.
     */
    void beginSendBulk(SendCallback *, const char *buffer, size_t len);

    /*! Starts the receive loop, the specified callback is invoked when a
        chunk of data has been received. May only be invoked once.
     */
    void beginReceive(ReceiveCallback *);

    /*! Starts the receive loop, the specified callback is invoked when a
        chunk of data has been received. May only be invoked once.
		\note Bulk data is not encrypted.
     */
    void beginReceiveBulk(ReceiveBulkCallback *);

private:
#if defined(__unix__) || defined(__VXWORKS__)
	TcpSocket(const Cipher &, int, IpAddress);
#elif defined(_WIN64)
	TcpSocket(const Cipher &, unsigned __int64, IpAddress);
#elif defined(_WIN32)
    TcpSocket(const Cipher &, unsigned int, IpAddress);
#endif

    TcpSocket(const TcpSocket &);
    TcpSocket & operator=(const TcpSocket &);

private:
    struct Private;
    Private * const p;
};

} // End of namespaces

#endif
