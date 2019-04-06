/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifndef SSCF1_SERVER_H
#define SSCF1_SERVER_H

#include <cstdint>
#include "bulkchannel.h"
#include "ipaddress.h"

namespace Sscf1 {

class Cipher;
class Message;
class TcpSocket;

/*! This class provides the server functionality of a client-server connection.
 */
class Server
{
public:
    struct Callback
    {
		Callback();
		virtual ~Callback();

        /*! This method is invoked when the a client is connected to the
            server.
         */
        virtual void handleConnect(const char *hostId) = 0;

        /*! This method is invoked when the a client is disconnected by the
            server.
         */
        virtual void handleDisconnect(const char *hostId) = 0;

        /*! This method is invoked when the server receives a request from the
            client. The implementation must invoke Server::sendResponse() to
            send the response, otherwise the client will block until the
            connection is closed.
         */
        virtual void handleRequest(const Message &request, const char *hostId) = 0;

        /*! This method is invoked when the client receives a response from the
            server.
         */
        virtual void handleResponse(const Message &response) = 0;

        /*! This method is invoked when the server receives a notification from
            the client.
         */
        virtual void handleNotification(const Message &notification) = 0;
    };

public:
    /*! Creates a new server for the specified port and network. Invoke
        Listen() to start the server listening on the specified port.
        \param callback The callback methods to be invoked.
        \param network The network and port (e.g. 127.0.0.0, 192.168.0.0, or 
					   ::1) to bind, use 0.0.0.0 or :: to bind all interfaces.
		\param cipher The encryption cipher to use.
     */
	Server(Callback *callback, IpAddress network, const Cipher *cipher = 0);

    /*! Creates a new server with the specified UPnP name. The name is used to
        publish the service through SSDP. Invoke Listen() to start the server
        listening on the specified port.
        \param callback The callback methods to be invoked.
        \param network The network (e.g. 127.0.0.0, 192.168.0.0, or ::1) to
					   bind, use 0.0.0.0 or :: to bind all interfaces.
        \param resourceName The UPnP resource name to publish the service with 
                            (e.g. "urn:healthcare-philips-com:service:MyService:1").
		\param cipher The encryption cipher to use.
     */
	Server(Callback *callback, IpAddress network, const char *resourceName, const Cipher *cipher = 0);

    ~Server();

	/*! Adds an interface to use for bulk traffic. If no interfaces are added,
		the interface used by this server is also used for bulk traffic. If one
		or more bulk interfaces are added, the interface used by this server not
		used for bulk traffic (unless it was explicity added). All bulk
		interfaces need to be added before calling listen().
        \param network The network and port (e.g. 127.0.0.0, 192.168.0.0, or 
					   ::1) to bind, use 0.0.0.0 or :: to bind all interfaces.
		\returns true if the interface was added successfully.
	 */
	bool addBulkInterface(IpAddress network);
    
    /*! Attempts to listen on the specified port and interface.
		\returns true if the server has started listening.
	*/
    bool listen(void);

    /*! Returns true if the server is listening.
     */
    bool isListening(void) const;

	/*! Returns the bound interface address and port.
	 */
	IpAddress address(void) const;

	/*! Returns the network mask of the bound interface.
	 */
	IpAddress netmask(void) const;
    
    /*! Returns the Host ID for this server.
     */
    const char * hostId(void) const;

    /*! Returns true if the specified client is connected.
     */
    bool isConnected(const char *clientHostId) const;

    /*! Returns the bulk channel.
     */
	BulkChannel & bulkChannel();

    /*! Creates a new message ID and sets the X-MessageId and X-HostId fields
        in the message.
     */
    void makeMessageId(Message &message);

    /*! Copies the X-MessageId field from the source message to the
        destination message and sets the X-HostId field in the destination
        message.
     */
    void copyMessageId(const Message &source, Message &destination);

    /*! Sends the specified request message to the client. If a response is 
        received, the provided response message is filled. Returns true if the 
        request was properly sent to the client. Note that the request message
        requires a field named X-MessageId with a unique message ID and a
        field named X-HostId with the host ID, use makeMessageId() to add them
        to the message.
		\param clientHostId	The host-ID of the client to send the request to.
		\param request	The request message to send.
		\param timeout	Timeout in milliseconds. The request will fail when it
						exceeds this timeout, resulting in the connection being
						closed. A timeout of 0 means infinite timeout.
     */
	bool sendRequest(const char *clientHostId, const Message &request, unsigned timeout = 0);

    /*! Sends the specified notification to the clients. Returns true if the 
        request was properly sent. When clientHostIds is empty, the notification
        is sent to all clients. Note that the notification message requires a
        field named X-MessageId with a unique message ID and a field named
        X-HostId with the host ID, use makeMessageId() to add them to the
        message.
     */
    bool sendNotification(
		const char * const *, size_t, 
		const BulkChannel::BulkId *, size_t, 
		const Message &notification);
 
	/*! Overload provided for convenience.
	 */
	bool sendNotification(const char *clientHostId, const Message &notification);
   
    /*! Sends the response to the client socket. Returns true if the response
        was properly sent to the client. Note that the response message
        requires a field named X-MessageId with the message ID of the request
        and a field named X-HostId with the host ID, use copyMessageId() to add
        them to the message.
     */
    bool sendResponse(const Message &response, const char *hostId);

private:
	Server(BulkChannel &, const uint8_t(&hostId)[16], IpAddress network, const Cipher &cipher);

	Server(const Server &);
    Server & operator=(const Server &);

private:
    struct Client;
    struct Private;
    Private * const p;
};

} // End of namespaces

#endif
