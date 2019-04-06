/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifndef SSCFTS1_SERVERNODE_H
#define SSCFTS1_SERVERNODE_H

#include "dispatcher.h"
#include "sscf1/ipaddress.h"

namespace Sscf1 { class Cipher; }
namespace SscfTs1 {

using Sscf1::Cipher;
class IEventLoop;

/*! This class provides the client functionality of a client-server connection.
    The client sends synchronous calls to the server and receives asynchronous
    notifications from the server.
 */
class SSCF_EXPORT ServerNode : private Dispatcher
{
public:
    /*! Creates a new server for the specified port and network. Invoke
        Listen() to start the server listening on the specified port.
        \param eventLoop The event loop for delegates to be invoked on.
        \param port The port to bind to, use 0 to bind a random free port.
        \param network The network and port (e.g. 127.0.0.0, 192.168.0.0, or 
					   ::1) to bind, use 0.0.0.0 or :: to bind all interfaces.
		\param cipher The encryption cipher to use.
     */
    ServerNode(IEventLoop &eventLoop, uint16_t port = 0, const std::string &network = empty, const Cipher *cipher = 0);

    /*! Creates a new server with the specified UPnP name. The name is used to
        publish the service through SSDP. Invoke Listen() to start the server
        listening on the specified port.
        \param eventLoop The event loop for delegates to be invoked on.
        \param network The network and port (e.g. 127.0.0.0, 192.168.0.0, or 
					   ::1) to bind, use 0.0.0.0 or :: to bind all interfaces.
        \param resourceName The UPnP resource name to publish the service with 
                            (e.g. "urn:healthcare-philips-com:service:MyService:1").
		\param cipher The encryption cipher to use.
     */
    ServerNode(IEventLoop &eventLoop, const std::string &network, const std::string &resourceName, const Cipher *cipher = 0);

    virtual ~ServerNode();

	/*! Adds an interface to use for bulk traffic. If no interfaces are added,
		the interface used by this server is also used for bulk traffic. If one
		or more bulk interfaces are added, the interface used by this server not
		used for bulk traffic (unless it was explicity added). All bulk
		interfaces need to be added before calling listen().
        \param network The network and port (e.g. 127.0.0.0, 192.168.0.0, or 
					   ::1) to bind, use 0.0.0.0 or :: to bind all interfaces.
        \param port The port to bind to, use 0 to bind a random free port.
		\returns true if the interface was added successfully.
	 */
	bool addBulkInterface(const std::string &network, uint16_t port = 0);

    /*! Attempts to listen on the specified port and interface.
		\returns true if the server has started listening.
	*/
    bool listen(void);

    /*! Returns true if the server is listening.
     */
    bool isListening(void) const;

	/*! Returns the bound interface address.
	 */
	Sscf1::IpAddress address(void) const;

	/*! Returns the network mask of the bound interface.
	 */
	Sscf1::IpAddress netmask(void) const;
    
    /*! Returns true if the specified client is connected.
     */
    bool isConnected(const std::string &clientHostId) const;

	/*! Returns the bulk channel.
	*/
	virtual Sscf1::BulkChannel & bulkChannel() override final;

	/*! Returns the Host ID for this server.
	*/
	std::string hostId(void) const;

    /*! Sets the call timeout in milliseconds. Synchronous calls will fail when
		they exceed this timeout, resulting in the connection being closed. A
		timeout of 0 means infinite timeout.
     */
	void setTimeout(unsigned);

    /*! This event is invoked when the connection has been established.
     */
    Event1<std::string> connected;

    /*! This event is invoked when the connection with the server was
        closed by the server or when the connect has failed.
     */
    Event1<std::string> disconnected;

public:
    /*! Returns true if the specified type is registred.
     */
    template <class _interface>
    bool providesType(const std::string &hostId)
    {
        return providesType(hostId, _interface::_sscf_name);
    }

#if !defined(__INTELLISENSE__) || defined(SSCFTS1_BUILD) // Hide internals from IntelliSense
    /*! Returns an instance ID for the specified type.
     */
    template <class _interface>
    std::string createInstance(const std::string &hostId)
    {
        return createInstance(hostId, _interface::_sscf_name);
    }

    /*! Returns an instance ID for the specified type.
     */
    template <class _interface>
    std::string createInstance(const std::string &hostId, const std::string &instance)
    {
        return createInstance(hostId, _interface::_sscf_name, instance);
    }

    /*! Initializes a proxy.
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
        Dispatcher::registerInterface<_type>(instance);
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
    bool providesType(const std::string &hostId, const std::string &name);

#if !defined(__INTELLISENSE__) || defined(SSCFTS1_BUILD) // Hide internals from IntelliSense
    std::string createInstance(const std::string &hostId, const char *name);
    std::string createInstance(const std::string &hostId, const char *name, const std::string &instance);

	virtual std::string clientHostId() override final;

    virtual void sendRequest(const std::string &hostId, const std::string &instance, const char *path, Message &in, Message &out) override final;
    virtual void sendRequestAsync(const std::string &hostId, const std::string &instance, const char *path, Message &in) override final;
    virtual void sendNotification(const std::vector<std::string> &hostIds, const std::vector<Sscf1::BulkChannel::BulkId> &, const std::string &instance, const char *path, Message &in) override final;
	virtual void sendDestroyInstance(const std::string &hostId, const std::string &instance) override final;

    virtual void registerInterface(const char *name, const std::string &instance, IMultitonFactory<void> *, CreateFunc, DestructFunc) override final;
    virtual void unregisterInterface(const std::string &instance, const char *name) override final;

private:
    ServerNode(const ServerNode &);
    ServerNode & operator=(const ServerNode &);

private:
    struct Private;
    Private * const p;
#endif // __INTELLISENSE__
};

} // End of namespaces

#endif
