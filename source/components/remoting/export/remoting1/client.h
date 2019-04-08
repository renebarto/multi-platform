/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifndef REMOTING1_CLIENT_H
#define REMOTING1_CLIENT_H

#include <cstdint>
#include "ipaddress.h"

namespace Remoting1 {

class BulkChannel;
class Cipher;
class Message;
class TcpSocket;

/*! This class provides the client functionality of a client-server connection.
 */
class Client
{
public:
    struct Callback
    {
		Callback();
		virtual ~Callback();

        /*! This method is invoked when the connection has been established.
         */
        virtual void handleConnect(void) = 0;

        /*! This method is invoked when the connection with the server was
            closed by the server or when the connect has failed.
         */
        virtual void handleDisconnect(void) = 0;

        /*! This method is invoked when the client receives a request from the
            server. The implementation must invoke Client::sendResponse() to
            send the response, otherwise the server will block until the
            connection is closed.
         */
        virtual void handleRequest(const Message &request) = 0;

        /*! This method is invoked when the client receives a response from the
            server.
         */
        virtual void handleResponse(const Message &response) = 0;

        /*! This method is invoked when the client receives a notification from
            the server.
         */
        virtual void handleNotification(const Message &notification) = 0;
    };

public:
    /*! Creates a new client for a server with the specified hostname and port.
        Invoke Connect to actually start connecting to the server.
        \param callback The callback methods to be invoked.
        \param address The address and port to connect to.
		\param cipher The encryption cipher to use.
     */
    Client(Callback *callback, IpAddress address, const Cipher *cipher = 0);

    /*! Creates a new client for a server with the specified UPnP name.
        Invoke Connect to actually start connecting to the server.
        \param callback The callback methods to be invoked.
        \param network The network (e.g. 127.0.0.0, 192.168.0.0, or ::1) to
					   look on for the service, use 0.0.0.0 or :: to search on
					   all interfaces.
        \param resourceName The UPnP resource name of the service to connect to 
                            (e.g. "urn:healthcare-philips-com:service:MyService:1").
		\param cipher The encryption cipher to use.
     */
    Client(Callback *callback, IpAddress network, const char *resourceName, const Cipher *cipher = 0);

    ~Client();

    /*! Attempts to connect to the server. Returns true if the connection 
        succeeds.
     */
    bool connect();

    /*! Attempts to connect to the server asynchronously. returns true if the
        asynchronous connect has started.
     */
    bool connectAsync();

    /*! Disconnects from the server.
     */
    void disconnect();

    /*! Returns true if the client is connected.
     */
    bool isConnected(void) const;

    /*! Returns the bulk channel.
     */
	BulkChannel & bulkChannel();

    /*! Returns the Host ID for this client.
     */
    const char * hostId(void) const;

    /*! Creates a new message ID and sets the X-MessageId and X-HostId fields
        in the message.
     */
    void makeMessageId(Message &message);

    /*! Copies the X-MessageId field from the source message to the
        destination message and sets the X-HostId field in the destination
        message.
     */
    void copyMessageId(const Message &source, Message &destination);

    /*! Sends the specified request message to the server, handleResponse() is
        invoked on the callback. Returns true if the request was properly sent
        to the server. Note that the request message requires a field named 
        X-MessageId with a unique message ID and a field named X-HostId with
        the host ID, use makeMessageId() to add them to the message.
		\param request	The request message to send.
		\param timeout	Timeout in milliseconds. The request will fail when it
						exceeds this timeout, resulting in the connection being
						closed. A timeout of 0 means infinite timeout.
	*/
    bool sendRequest(const Message &request, unsigned timeout = 0);
    
    /*! Sends the specified notification to the server. Returns true if the
        notification was properly sent to the server, false otherwise. Note
        that the notification message requires a field named X-MessageId with
        a unique message ID and a field named X-HostId with the host ID, use 
        makeMessageId() to add them to the message.
     */
    bool sendNotification(const Message &notification);
        
    /*! Sends the response to the server. Returns true if the response was
        properly sent to the server. Note that the response message requires a
        field named X-MessageId with the message ID of the request and a field
        named X-HostId with the host ID, use copyMessageId() to add them to the
        message.
     */
    bool sendResponse(const Message &response);

private:
    Client(const Client &);
    Client & operator=(const Client &);

private:
    struct Private;
    Private * const p;
};

} // End of namespaces

#endif
