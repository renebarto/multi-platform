/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#include "sscfts1/clientnode.h"
#include "sscfts1/eventloop.h"
#include "sscf1/client.h"
#include "sscf1/message.h"
#include "sscf1/system.h"
#include "sscf1/tcpsocket.h"

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
#include <string>
#include <sstream>
#include <string>

namespace SscfTs1 {

using namespace Sscf1;

struct ClientNode::Private 
  : Client::Callback
{
	Private() : me(nullptr), timeout(0) { }
    virtual ~Private() { }

    std::mutex mutex;

    ClientNode * me;
    std::unique_ptr<EventLoopProxy> eventLoop;
	std::unique_ptr<Client> client;

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

    virtual void handleConnect(void) override;
    virtual void handleDisconnect(void) override;
    virtual void handleRequest(const Message &request) override;
    virtual void handleResponse(const Message &response) override;
    virtual void handleNotification(const Message &notification) override;
};

ClientNode::ClientNode(IEventLoop &eventLoop, const std::string &host, uint16_t port, const Cipher *cipher)
  : p(new Private())
{
    p->me = this;
    p->eventLoop.reset(new EventLoopProxy(eventLoop));
	p->client.reset(new Client(p, IpAddress::resolve(host, port), cipher));
}

ClientNode::ClientNode(IEventLoop &eventLoop, const std::string &network, const std::string &resourceName, const Cipher *cipher)
  : p(new Private())
{
    p->me = this;
	p->eventLoop.reset(new EventLoopProxy(eventLoop));
	p->client.reset(new Client(p, network, resourceName.c_str(), cipher));
}

ClientNode::~ClientNode()
{
    p->client->disconnect();

    delete p;
    *const_cast<Private **>(&p) = nullptr;
}

bool ClientNode::connect()
{
	if (Sscf1::system().threadSafetyChecksEnabled() &&
		!p->eventLoop->checkThread())
	{
		Sscf1::system().fail(
			"A ClientNode should be connected on its own thread");
	}

    return p->client->connect();
}

bool ClientNode::connectAsync()
{
    return p->client->connectAsync();
}

void ClientNode::disconnect()
{
    p->client->disconnect();

	if (p->eventLoop->checkThread())
		destroyInstances();
	else
		p->eventLoop->post(std::bind(static_cast<void(Dispatcher::*)()>(&ClientNode::destroyInstances), static_cast<Dispatcher *>(this)));
}

bool ClientNode::isConnected(void) const
{
    return p->client->isConnected();
}

std::string ClientNode::hostId(void) const
{
    return p->client->hostId();
}

void ClientNode::setTimeout(unsigned timeout)
{
	p->timeout = timeout;
}

bool ClientNode::providesType(const std::string &name)
{
    return invoke<bool, const std::string>(empty, "sscf", "/providesType", name);
}

std::string ClientNode::createInstance(const char *name)
{
    const std::string result = invoke<std::string, const std::string, const std::string>(empty, "sscf", "/createInstance", name, std::string());
	if (result.empty())
		throw CommunicationException("Failed to create proxy for: " + std::string(name));

	return result;
}

std::string ClientNode::createInstance(const char *name, const std::string &instance)
{
    const std::string result = invoke<std::string, const std::string, const std::string>(empty, "sscf", "/createInstance", name, instance);
	if (result.empty())
		throw CommunicationException("Failed to create proxy for: " + std::string(name));

	return result;
}

Sscf1::BulkChannel & ClientNode::bulkChannel()
{
    return p->client->bulkChannel();
}

std::string ClientNode::clientHostId()
{
    return p->client->hostId();
}

void ClientNode::sendRequest(const std::string &, const std::string &instance, const char *path, Message &in, Message &out)
{
    std::ostringstream fullPath;
	fullPath.imbue(std::locale::classic());
	fullPath << "/" << instance << path;

    p->client->makeMessageId(in);
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

        if (p->client->sendRequest(in, p->timeout))
        {
            waitForResponse(response.finished, p->mutex, response.event);

            if (out.responseStatus() != 200)
            {
                std::ostringstream msg;
                msg << ((out.responseStatus() == 504) ? "Timeout" : "Failure")
					<< " while sending request: " << fullPath.str();

                const char * const moduleName = out.field("X-ModuleName");
                if (moduleName)
                    msg << " to: " << moduleName;

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
            throw CommunicationException("Failure while sending request: " + fullPath.str());

        p->removeResponse(messageId);
    }
}

void ClientNode::sendRequestAsync(const std::string &, const std::string &instance, const char *path, Message &in)
{
    std::ostringstream fullPath;
	fullPath.imbue(std::locale::classic());
	fullPath << "/" << instance << path;

    p->client->makeMessageId(in);
    in.setRequestMethod("NOTIFY");
    in.setRequestPath(fullPath.str().c_str());

    p->client->sendNotification(in);
}

void ClientNode::sendNotification(const std::vector<std::string> &, const std::vector<Sscf1::BulkChannel::BulkId> &, const std::string &instance, const char *path, Message &in)
{
    std::ostringstream fullPath;
	fullPath.imbue(std::locale::classic());
	fullPath << "/" << hostId() << "/" << instance << path;

    p->client->makeMessageId(in);
    in.setRequestMethod("NOTIFY");
    in.setRequestPath(fullPath.str().c_str());

    p->client->sendNotification(in);
}

void ClientNode::sendDestroyInstance(const std::string &, const std::string &instance)
{
    if (p->client->isConnected())
    {
        try
        {
            invoke<void, const std::string>(empty, "sscf", "/destroyInstance", instance);
        }
        catch (const CommunicationException &)
        {
            // Ignore errors in case connection terminated.
        }
    }
}

void ClientNode::registerInterface(
	const char *name, 
	const std::string &instance, 
	IMultitonFactory<void> *factory, 
	CreateFunc createFunc, 
	DestructFunc destructFunc)
{
	system().traceMsg(
		"ClientNode: Registring interface \"%s\"",
		name);

	return Dispatcher::registerInterface(name, instance, factory, createFunc, destructFunc);
}

void ClientNode::unregisterInterface(const std::string &instance, const char *name)
{
	system().traceMsg(
		"ClientNode: Unregistring interface \"%s\"",
		name);

	return Dispatcher::unregisterInterface(instance, name);
}

void ClientNode::Private::addResponse(const std::string &messageId, Response &response)
{
    std::lock_guard<std::mutex> _(mutex);

    responses[messageId] = &response;
}

void ClientNode::Private::removeResponse(const std::string &messageId)
{
    std::lock_guard<std::mutex> _(mutex);

    auto i = responses.find(messageId);
    if (i != responses.end())
        responses.erase(i);
}

void ClientNode::Private::handleConnect(void)
{
	eventLoop->post(std::bind(&Event0::operator(), &me->connected));
}

void ClientNode::Private::handleDisconnect(void)
{
    eventLoop->post([this]
	{
		me->handleDisconnect(me->empty);
        me->disconnected();
	});
}

void ClientNode::Private::handleRequest(const Message &request)
{
    eventLoop->post([this, request]
	{
        Message response;
        me->handleMessage(request, response);
        client->copyMessageId(request, response);
        client->sendResponse(response);
	});
}

void ClientNode::Private::handleResponse(const Message &response)
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

void ClientNode::Private::handleNotification(const Message &notification)
{
    eventLoop->post([this, notification]
	{
        Message dummy;
        me->handleMessage(notification, dummy);
	});
}

} // End of namespaces
