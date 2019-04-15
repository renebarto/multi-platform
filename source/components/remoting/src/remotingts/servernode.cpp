/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#include "remotingts1/servernode.h"
#include "remotingts1/eventloop.h"
#include "remoting1/server.h"
#include "remoting1/message.h"
#include "remoting1/system.h"
#include "remoting1/tcpsocket.h"

#include <cassert>
#include <locale>
#include <map>
#include <memory>
#ifdef _MSC_VER
# pragma warning (disable : 4265) // Suppress warning from Microsoft headers
#endif
#include <condition_variable>
#include <mutex>
#ifdef _MSC_VER
# pragma warning (default : 4265)
#endif
#include <sstream>
#include <string>

namespace RemotingTs1 {

using namespace Remoting1;

struct ServerNode::Private 
    : Server::Callback
{
	Private() : me(nullptr), timeout(0) { }
    virtual ~Private() { }

    std::mutex mutex;

    ServerNode * me;
    std::unique_ptr<EventLoopProxy> eventLoop;
	std::unique_ptr<Server> server;

	unsigned timeout;

    struct Response
    {
        Message *message;
        bool finished;
        std::condition_variable event;
    };

    std::map<std::string, Response *> responses;
    void addResponse(const std::string &, Response &);
    void removeResponse(const std::string &);

    virtual void handleConnect(const char *hostId) override;
    virtual void handleDisconnect(const char *hostId) override;
    virtual void handleRequest(const Message &request, const char *hostId) override;
    virtual void handleResponse(const Message &response) override;
    virtual void handleNotification(const Message &notification) override;
};

ServerNode::ServerNode(IEventLoop &eventLoop, uint16_t port, const std::string &network, const Cipher *cipher)
  : p(new Private())
{
    p->me = this;
    p->eventLoop.reset(new EventLoopProxy(eventLoop));
	p->server.reset(new Server(p, !network.empty() ? IpAddress(network, port) : IpAddress(0u, port), cipher));
}

ServerNode::ServerNode(IEventLoop &eventLoop, const std::string &network, const std::string &resourceName, const Cipher *cipher)
  : p(new Private())
{
    p->me = this;
	p->eventLoop.reset(new EventLoopProxy(eventLoop));
	p->server.reset(new Server(p, network, resourceName.c_str(), cipher));
}

ServerNode::~ServerNode()
{
    delete p;
    *const_cast<Private **>(&p) = nullptr;
}

bool ServerNode::addBulkInterface(const std::string &network, uint16_t port)
{
	return p->server->addBulkInterface(IpAddress(network, port));
}

bool ServerNode::listen(void)
{
    return p->server->listen();
}

bool ServerNode::isListening(void) const
{
    return p->server->isListening();
}

IpAddress ServerNode::address(void) const
{
	return p->server->address();
}

IpAddress ServerNode::netmask(void) const
{
	return p->server->netmask();
}

bool ServerNode::isConnected(const std::string &clientHostId) const
{
    return p->server->isConnected(clientHostId.c_str());
}

std::string ServerNode::hostId(void) const
{
	return p->server->hostId();
}

void ServerNode::setTimeout(unsigned timeout)
{
	p->timeout = timeout;
}

bool ServerNode::providesType(const std::string &hostId, const std::string &name)
{
    return invoke<bool, const std::string>(hostId, "remoting", "/providesType", name);
}

std::string ServerNode::createInstance(const std::string &clientHostId, const char *name)
{
    const std::string result = invoke<std::string, const std::string, const std::string>(clientHostId, "remoting", "/createInstance", name, empty);
	if (result.empty())
		throw CommunicationException("Failed to create proxy for: " + std::string(name) + " at [" + clientHostId + ']');

	return result;
}

std::string ServerNode::createInstance(const std::string &clientHostId, const char *name, const std::string &instance)
{
    const std::string result = invoke<std::string, const std::string, const std::string>(clientHostId, "remoting", "/createInstance", name, instance);
	if (result.empty())
		throw CommunicationException("Failed to create proxy for: " + std::string(name) + " at [" + clientHostId + ']');

	return result;
}

Remoting1::BulkChannel & ServerNode::bulkChannel()
{
	return p->server->bulkChannel();
}

std::string ServerNode::clientHostId()
{
    return empty;
}

void ServerNode::sendRequest(const std::string &hostId, const std::string &instance, const char *path, Message &in, Message &out)
{
    std::ostringstream fullPath;
	fullPath.imbue(std::locale::classic());
	fullPath << "/" << instance << path;

    p->server->makeMessageId(in);
    in.setRequestMethod("GET");
    in.setRequestPath(fullPath.str().c_str());

    Private::Response response;
    response.message = &out;
    response.finished = false;

    const char * const messageId = in.field("X-MessageId");
    assert(messageId);
    if (messageId)
    {
        p->addResponse(messageId, response);

        if (p->server->sendRequest(hostId.c_str(), in, p->timeout))
        {
            waitForResponse(response.finished, p->mutex, response.event);

            if (out.responseStatus() != 200)
            {
                std::ostringstream msg;
				msg << ((out.responseStatus() == 504) ? "Timeout" : "Failure")
					<< " while sending request: " << fullPath.str()
					<< " to [" << hostId << ']';

                const char * const moduleName = out.field("X-ModuleName");
                if (moduleName)
                    msg << ' ' << moduleName;

				Formatter::Deserializer deserializer(out.content(), &bulkChannel());
				std::string text;
				deserializer >> text;

                if (!text.empty())
                    msg << std::endl << text;

                p->removeResponse(messageId);
                throw CommunicationException(msg.str());
            }
        }
		else
		{
			throw CommunicationException(
				"Failure while sending request: " + fullPath.str() + 
				" to [" + hostId + ']');
		}

        p->removeResponse(messageId);
    }
}

void ServerNode::sendRequestAsync(const std::string &hostId, const std::string &instance, const char *path, Message &in)
{
    std::ostringstream fullPath;
	fullPath.imbue(std::locale::classic());
	fullPath << "/" << instance << path;

    p->server->makeMessageId(in);
    in.setRequestMethod("NOTIFY");
    in.setRequestPath(fullPath.str().c_str());

    p->server->sendNotification(hostId.c_str(), in);
}

void ServerNode::sendNotification(const std::vector<std::string> &hostIds, const std::vector<Remoting1::BulkChannel::BulkId> &bulkIds, const std::string &instance, const char *path, Message &in)
{
    std::ostringstream fullPath;
	fullPath.imbue(std::locale::classic());
	fullPath << "/" << instance << path;

    p->server->makeMessageId(in);
    in.setRequestMethod("NOTIFY");
    in.setRequestPath(fullPath.str().c_str());

	std::vector<const char *> hostIdPtrs;
	hostIdPtrs.reserve(hostIds.size());
	for (const auto &i : hostIds)
		hostIdPtrs.push_back(i.c_str());

    p->server->sendNotification(
		!hostIdPtrs.empty() ? &hostIdPtrs[0] : nullptr, hostIdPtrs.size(), 
		!bulkIds.empty() ? &bulkIds[0] : nullptr, bulkIds.size(),
		in);
}

void ServerNode::sendDestroyInstance(const std::string &clientHostId, const std::string &instance)
{
    if (p->server->isConnected(clientHostId.c_str()))
    {
        try
        {
            invoke<void, const std::string>(clientHostId, "remoting", "/destroyInstance", instance);
        }
        catch (const CommunicationException &)
        {
            // Ignore errors in case connection terminated.
        }
    }
}

void ServerNode::registerInterface(
	const char *name, 
	const std::string &instance, 
	IMultitonFactory<void> *factory, 
	CreateFunc createFunc, 
	DestructFunc destructFunc)
{
	system().traceMsg(
		"ServerNode: Registring interface \"%s\" on %s",
		name,
        address().toString().c_str());

	return Dispatcher::registerInterface(name, instance, factory, createFunc, destructFunc);
}

void ServerNode::unregisterInterface(const std::string &instance, const char *name)
{
	system().traceMsg(
		"ServerNode: Unregistring interface \"%s\" from %s",
		name,
        address().toString().c_str());

	return Dispatcher::unregisterInterface(instance, name);
}

void ServerNode::Private::addResponse(const std::string &messageId, Response &response)
{
    std::lock_guard<std::mutex> _(mutex);

    responses[messageId] = &response;
}

void ServerNode::Private::removeResponse(const std::string &messageId)
{
    std::lock_guard<std::mutex> _(mutex);

    auto i = responses.find(messageId);
    if (i != responses.end())
        responses.erase(i);
}

void ServerNode::Private::handleConnect(const char *hostId_)
{
	const std::string hostId = hostId_;

    eventLoop->post([this, hostId]
	{
        me->connected(hostId);
	});
}

void ServerNode::Private::handleDisconnect(const char *hostId_)
{
	const std::string hostId = hostId_;

    eventLoop->post([this, hostId]
	{
		me->handleDisconnect(hostId);
        me->disconnected(hostId);
	});
}

void ServerNode::Private::handleRequest(const Message &request, const char *hostId_)
{
	const std::string hostId = hostId_;

    eventLoop->post([this, request, hostId]
	{
        Message response;
        me->handleMessage(request, response);
        server->copyMessageId(request, response);
        server->sendResponse(response, hostId.c_str());
	});
}

void ServerNode::Private::handleResponse(const Message &response)
{
    const char * const messageId = response.field("X-MessageId");
    if (messageId)
    {
        std::lock_guard<std::mutex> _(mutex);

        auto i = responses.find(messageId);
        if (i != responses.end())
        {
            *(i->second->message) = response;
            i->second->finished = true;
            i->second->event.notify_all();
            responses.erase(i);
        }
    }
}

void ServerNode::Private::handleNotification(const Message &notification)
{
    eventLoop->post([this, notification]
	{
        Message dummy;
        me->handleMessage(notification, dummy);
	});
}

} // End of namespaces
