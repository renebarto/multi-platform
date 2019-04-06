/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifndef SSCFTS1_CLIENTNODE_H
#define SSCFTS1_CLIENTNODE_H

#include "dispatcher.h"

namespace Sscf1 { class Cipher; }
namespace SscfTs1 {

using Sscf1::Cipher;
class IEventLoop;

/*! This class provides the client functionality of a client-server connection.
    The client sends synchronous calls to the server and receives asynchronous
    notifications from the server.
 */
class SSCF_EXPORT ClientNode : private Dispatcher
{
public:
    /*! Creates a new client for a server with the specified hostname and port.
        Invoke Connect to actually start connecting to the server.
        \param eventLoop The event loop for delegates to be invoked on.
        \param host The hostname or address to connect to.
        \param port The port to connect to.
		\param cipher The encryption cipher to use.
     */
    ClientNode(IEventLoop &eventLoop, const std::string &host, uint16_t port, const Cipher *cipher = 0);

    /*! Creates a new client for a server with the specified UPnP name.
        Invoke Connect to actually start connecting to the server.
        \param eventLoop The event loop for delegates to be invoked on.
        \param network The network (e.g. 127.0.0.0, 192.168.0.0, or ::1) to
					   look on for the service, use 0.0.0.0 or :: to search on
					   all interfaces.
        \param resourceName The UPnP resource name of the service to connect to 
                            (e.g. "urn:healthcare-philips-com:service:MyService:1").
		\param cipher The encryption cipher to use.
     */
    ClientNode(IEventLoop &eventLoop, const std::string &network, const std::string &resourceName, const Cipher *cipher = 0);

    virtual ~ClientNode();

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
	virtual Sscf1::BulkChannel & bulkChannel() override final;

    /*! Returns the Host ID for this client.
     */
    std::string hostId(void) const;

    /*! Sets the call timeout in milliseconds. Synchronous calls will fail when
		they exceed this timeout, resulting in the connection being closed. A
		timeout of 0 means infinite timeout.
     */
	void setTimeout(unsigned);

    /*! This event is invoked when the connection has been established.
     */
    Event0 connected;

    /*! This event is invoked when the connection with the server was
        closed by the server or when the connect has failed.
     */
    Event0 disconnected;

public:
    /*! Returns true if the specified type is registred.
     */
    template <class _interface>
    bool providesType(void)
    {
        return providesType(_interface::_sscf_name);
    }

#if !defined(__INTELLISENSE__) || defined(SSCFTS1_BUILD) // Hide internals from IntelliSense
    /*! Returns an instance ID for the specified type.
     */
    template <class _interface>
    std::string createInstance()
    {
        return createInstance(_interface::_sscf_name);
    }

    /*! Returns an instance ID for the specified type.
     */
    template <class _interface>
    std::string createInstance(const std::string &instance)
    {
        return createInstance(_interface::_sscf_name, instance);
    }

    /*! Initializes an instance.
     */
    template <class _type>
    void initProxy(_type *instance)
    {
        instance->_sscf_init(*this);
    }
#endif // __INTELLISENSE__

    /*! Registers a singleton for usage.
     */
    template <class _type>
    void registerSingleton(_type *singleton)
    {
        Dispatcher::registerInterface<_type>(singleton_instance);
        singleton->_sscf_register(*this, singleton_instance);
    }

    /*! Registers an instance for usage.
     */
    template <class _type>
    void registerInstance(_type *singleton, const std::string &instance)
    {
        Dispatcher::registerInterface<_type>();
        singleton->_sscf_register(*this, instance);
    }

    /*! Registers a multiton for usage.
     */
    template <class _type>
    void registerMultiton(IMultitonFactory<_type> *multitonFactory)
    {
        Dispatcher::registerInterface<_type>(empty, multitonFactory);
    }

private:
    bool providesType(const std::string &name);

#if !defined(__INTELLISENSE__) || defined(SSCFTS1_BUILD) // Hide internals from IntelliSense
    std::string createInstance(const char *name);
    std::string createInstance(const char *name, const std::string &instance);

	virtual std::string clientHostId() override final;

    virtual void sendRequest(const std::string &hostId, const std::string &instance, const char *path, Message &in, Message &out) override final;
    virtual void sendRequestAsync(const std::string &hostId, const std::string &instance, const char *path, Message &in) override final;
    virtual void sendNotification(const std::vector<std::string> &hostIds, const std::vector<Sscf1::BulkChannel::BulkId> &, const std::string &instance, const char *path, Message &in) override final;
	virtual void sendDestroyInstance(const std::string &hostId, const std::string &instance) override final;

    virtual void registerInterface(const char *name, const std::string &instance, IMultitonFactory<void> *, CreateFunc, DestructFunc) override final;
    virtual void unregisterInterface(const std::string &instance, const char *name) override final;

private:
    ClientNode(const ClientNode &);
    ClientNode & operator=(const ClientNode &);

private:
    struct Private;
    Private * const p;
#endif // __INTELLISENSE__
};

} // End of namespaces

#endif
