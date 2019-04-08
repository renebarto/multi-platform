/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifdef _MSC_VER
# pragma warning (disable : 4464) // Suppress warning including relative headers
#endif

#include "remoting1/server.h"
#include "remoting1/bulkchannel.h"
#include "remoting1/cipher.h"
#include "remoting1/system.h"
#include "remoting1/discovery.h"
#include "remoting1/message.h"
#include "remoting1/tcpsocket.h"
#include "../../include/remoting/commthread.h"

#include <cassert>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <locale>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>

namespace Remoting1 {

struct Server::Client 
  : TcpSocket::SendCallback, 
    TcpSocket::ReceiveCallback
{
    Client(Private *, TcpSocket *);
    virtual ~Client();

    Private *p;
    std::unique_ptr<TcpSocket> socket;
    std::string hostIdStr;
    CommThread::HostId hostId;
    std::string moduleName;

    std::vector<char> inbuffer;
    Message message;
        
    std::set<std::string> requests;

    virtual void sendClosed(void);
    virtual void sendSent(const char *buffer, size_t len);
    virtual void receiveClosed(void);
    virtual void receiveReceived(const char *, size_t);
};

struct Server::Private : TcpSocket::BindCallback
{
    Private(void *) 
      :	commThread(CommThread::instance()),
        discovery(), 
        cipher(nullptr),
        callback(nullptr),
        listening(false),
        messageId(0),
        bulkChannel(nullptr)
    {
    }

    Private() 
      :	commThread(CommThread::instance()),
        discovery(Discovery::instance()), 
        cipher(nullptr),
        callback(nullptr),
        listening(false),
        messageId(0),
        bulkChannel(nullptr)
    { 
    }

    virtual ~Private() { }

    std::recursive_mutex mutex;

    const std::shared_ptr<CommThread> commThread;
    const std::shared_ptr<Discovery> discovery;

    std::unique_ptr<CipherNull> myCipher;
    const Cipher *cipher;

    Callback *callback;
    std::string resourceName;
    IpAddress network;
    IpAddress::Interface iface;
    bool listening;

    CommThread::HostId hostId;
    std::string hostIdStr;
    volatile uint64_t messageId;

    std::unique_ptr<TcpSocket> serverSocket;
    std::unique_ptr<BulkChannel> myBulkChannel;
    BulkChannel *bulkChannel;
    std::vector<std::unique_ptr<Server>> bulkServers;

    std::map<std::string, std::unique_ptr<Client>> clients;
    std::vector<std::unique_ptr<Client>> anonymousClients;

    virtual void bindAccepted(TcpSocket *);
};

Server::Server(Callback *callback, IpAddress network, const Cipher *cipher)
  : p(new Private(nullptr))
{
    p->myCipher.reset((cipher == nullptr) ? new CipherNull() : nullptr);
    p->cipher = (cipher == nullptr) ? p->myCipher.get() : cipher;

    p->callback = callback;
    p->listening = false;

    p->network = std::move(network);
    p->iface = IpAddress::findInterface(p->network);
    if (!p->iface)
        p->iface.address = IpAddress::any(p->network.family());

    p->hostId = CommThread::generateHostId();
    p->hostIdStr = p->hostId;
    p->messageId = 1;

    p->serverSocket.reset(new TcpSocket(p->iface.address.family(), *p->cipher));
    p->myBulkChannel.reset(new BulkChannel(*p->cipher, p->hostId.data));
    p->bulkChannel = p->myBulkChannel.get();
}

Server::Server(Callback *callback, IpAddress network, const char *resourceName, const Cipher *cipher)
  : p(new Private())
{
    p->myCipher.reset((cipher == nullptr) ? new CipherNull() : nullptr);
    p->cipher = (cipher == nullptr) ? p->myCipher.get() : cipher;

    p->callback = callback;
    p->resourceName = resourceName;
    p->listening = false;

    p->network = std::move(network);
    p->network.setPort(0);
    p->iface = IpAddress::findInterface(p->network);
    if (!p->iface)
        p->iface.address = IpAddress::any(p->network.family());

    p->hostId = CommThread::generateHostId();
    p->hostIdStr = p->hostId;
    p->messageId = 1;

    p->serverSocket.reset(new TcpSocket(p->iface.address.family(), *p->cipher));
    p->myBulkChannel.reset(new BulkChannel(*p->cipher, p->hostId.data));
    p->bulkChannel = p->myBulkChannel.get();
}

Server::Server(BulkChannel &bulkChannel, const uint8_t(&hostId)[16], IpAddress network, const Cipher &cipher)
  : p(new Private(nullptr))
{
    p->cipher = &cipher;

    p->callback = nullptr;
    p->listening = false;

    p->network = std::move(network);
    p->iface = IpAddress::findInterface(p->network);
    if (!p->iface)
        p->iface.address = IpAddress::any(p->network.family());

    p->hostId = hostId;
    p->hostIdStr = p->hostId;
    p->messageId = 1;

    p->serverSocket.reset(new TcpSocket(p->iface.address.family(), *p->cipher));
    p->bulkChannel = &bulkChannel;
}

Server::~Server()
{
    if (p->discovery)
        p->discovery->unpublishService(p->resourceName.c_str(), p->hostIdStr.c_str());

    p->serverSocket->close();

    std::vector<std::unique_ptr<Client>> clients;

    {
        std::lock_guard<std::recursive_mutex> _(p->mutex);

        for (auto &&i : p->clients)
            clients.emplace_back(std::move(i.second));
        
        p->clients.clear();

        for (auto i = p->anonymousClients.begin();
             i != p->anonymousClients.end();
             i = p->anonymousClients.erase(i))
        {
            clients.emplace_back(std::move(*i));
        }
    }

    // These have to be closed outside the mutex as they may trigger a
    // receiveClosed().
    for (const auto &i : clients)
        i->socket->close();

    clients.clear();

    delete p;
    *const_cast<Private **>(&p) = nullptr;
}

bool Server::addBulkInterface(IpAddress network)
{
    if (!p->listening)
    {
        std::unique_ptr<Server> server(new Server(
            *p->bulkChannel, 
            p->hostId.data, 
            std::move(network), 
            *p->cipher));

        if (server->listen())
        {
            p->bulkServers.emplace_back(std::move(server));
            return true;
        }
    }

    return false;
}

bool Server::listen(void)
{
    if (!p->listening)
    {
        auto iface = p->iface.address;
        iface.setPort(p->network.port());

        if (p->serverSocket->bind(p, iface))
        {
            p->listening = true;

            if (p->discovery)
                p->discovery->publishService(p->resourceName.c_str(), p->hostIdStr.c_str(), address());

            if (!p->resourceName.empty())
            {
                system().traceMsg(
                    "Server: \"%s\" listening on %s",
                    p->resourceName.c_str(),
                    p->serverSocket->address().toString().c_str());
            }
            else
            {
                system().traceMsg(
                    "Server: listening on %s",
                    p->serverSocket->address().toString().c_str());
            }
        }
    }

    return p->listening;
}

bool Server::isListening(void) const
{
    return p->listening;
}

IpAddress Server::address(void) const
{
    IpAddress result = p->iface.address;
    if (p->listening)
        result.setPort(p->serverSocket->address().port());

    return result;
}

IpAddress Server::netmask(void) const
{
    return p->iface.netmask;
}

const char * Server::hostId(void) const
{
    return p->hostIdStr.c_str();
}

bool Server::isConnected(const char *clientHostId) const
{
    std::lock_guard<std::recursive_mutex> _(p->mutex);

    return p->clients.find(clientHostId) != p->clients.end();
}

BulkChannel & Server::bulkChannel()
{
    return *p->bulkChannel;
}

void Server::makeMessageId(Message &message)
{
    uint64_t messageId;
    {
        std::lock_guard<std::recursive_mutex> _(p->mutex);

        messageId = p->messageId++;
    }

    std::ostringstream messageIdStr;
    messageIdStr.imbue(std::locale::classic());
    messageIdStr << std::hex << std::setfill('0') << std::setw(16) << messageId;

    message.setField("X-MessageId", messageIdStr.str().c_str());
    message.setField("X-HostId", p->hostIdStr.c_str());
}

void Server::copyMessageId(const Message &source, Message &destination)
{
    const char * const messageId = source.field("X-MessageId");
    assert(messageId);
    if (messageId)
        destination.setField("X-MessageId", messageId);

    destination.setField("X-HostId", p->hostIdStr.c_str());
#ifndef NDEBUG // Add the request path in debug mode.
    destination.setField("X-Debug-RequestPath", source.requestPath());
#endif
}

bool Server::sendRequest(const char *clientHostId, const Message &request, unsigned timeout)
{
    using namespace std::chrono;

    bool result = false;

    const char * const messageId = request.field("X-MessageId");
    const char * const hostIdStr = request.field("X-HostId");
    assert(messageId && hostIdStr);
    if (messageId && hostIdStr)
    {
        std::lock_guard<std::recursive_mutex> _(p->mutex);

        auto i = p->clients.find(clientHostId);
        if (i != p->clients.end())
        {
            i->second->requests.insert(messageId);

            const std::vector<uint8_t> &content = request.content();
            if (!content.empty())
            {
                const char * const sendbuf[2] = { request.header(), reinterpret_cast<const char *>(&(content[0])) };
                const size_t sendlen[2] = { ::strlen(sendbuf[0]), content.size() };
                i->second->socket->beginSend(i->second.get(), sendbuf, sendlen, 2);
            }
            else
                i->second->socket->beginSend(i->second.get(), request.header());

            if (timeout)
            {
                const std::string chid = clientHostId;
                const std::string id = messageId;
                p->commThread->setTimerEvent(i->second.get(), milliseconds(timeout), [this, chid, id]
                {
                    std::unique_ptr<Client> client;

                    {
                        std::lock_guard<std::recursive_mutex> _(p->mutex);

                        auto i = p->clients.find(chid);
                        if (i != p->clients.end())
                        {
                            auto j = i->second->requests.find(id);
                            if (j != i->second->requests.end())
                            {
                                client = std::move(i->second);
                                client->requests.erase(j);
                                p->clients.erase(i);
                            }
                        }
                    }

                    if (client)
                    {
                        Message response(504);
                        response.setField("X-MessageId", id.c_str());
                        p->callback->handleResponse(response);

                        client->socket->close();
                        client->receiveClosed();
                    }
                });
            }

            result = true;
        }
    }

    return result;
}

bool Server::sendNotification(
    const char * const *clientHostIds, size_t clientHostIdsSize, 
    const BulkChannel::BulkId *bulkIds, size_t bulkIdsSize,
    const Message &notification)
{
    bool result = false;
    
    const char * const messageId = notification.field("X-MessageId");
    const char * const hostIdStr = notification.field("X-HostId");
    assert(messageId && hostIdStr);
    if (messageId && hostIdStr)
    {
        std::lock_guard<std::recursive_mutex> _(p->mutex);

        if (clientHostIdsSize == 0)
        {
            p->bulkChannel->discard(bulkIds, bulkIdsSize);
        }
        else for (size_t i = 0; i < clientHostIdsSize; i++)
        {
            if ((clientHostIds[i] == nullptr) || (strlen(clientHostIds[i]) == 0))
            {
                for (const auto &j : p->clients)
                {
                    p->bulkChannel->addRecipients(bulkIds, bulkIdsSize);

                    const std::vector<uint8_t> &content = notification.content();
                    if (!content.empty())
                    {
                        const char * const sendbuf[2] = { notification.header(), reinterpret_cast<const char *>(&(content[0])) };
                        const size_t sendlen[2] = { ::strlen(sendbuf[0]), content.size() };
                        j.second->socket->beginSend(j.second.get(), sendbuf, sendlen, 2);
                    }
                    else
                        j.second->socket->beginSend(j.second.get(), notification.header());
                }

                result = true;
            }
            else 
            {
                auto j = p->clients.find(clientHostIds[i]);
                if (j != p->clients.end())
                {
                    p->bulkChannel->addRecipients(bulkIds, bulkIdsSize);

                    const std::vector<uint8_t> &content = notification.content();
                    if (!content.empty())
                    {
                        const char * const sendbuf[2] = { notification.header(), reinterpret_cast<const char *>(&(content[0])) };
                        const size_t sendlen[2] = { ::strlen(sendbuf[0]), content.size() };
                        j->second->socket->beginSend(j->second.get(), sendbuf, sendlen, 2);
                    }
                    else
                        j->second->socket->beginSend(j->second.get(), notification.header());

                    result = true;
                }
            }
        }
    }

    return result;
}

bool Server::sendNotification(const char *clientHostId, const Message &notification)
{
    return sendNotification(&clientHostId, 1, nullptr, 0, notification);
}

bool Server::sendResponse(const Message &response, const char *hostId)
{
    {
        std::lock_guard<std::recursive_mutex> _(p->mutex);

        auto i = p->clients.find(hostId);
        if (i != p->clients.end())
        {
            const std::vector<uint8_t> &content = response.content();
            if (!content.empty())
            {
                const char * const sendbuf[2] = { response.header(), reinterpret_cast<const char *>(&(content[0])) };
                const size_t sendlen[2] = { ::strlen(sendbuf[0]), content.size() };
                i->second->socket->beginSend(nullptr, sendbuf, sendlen, 2);
            }
            else
                i->second->socket->beginSend(nullptr, response.header());
        }
    }

    return true;
}

void Server::Private::bindAccepted(TcpSocket *socket)
{
    std::unique_ptr<Client> client(new Client(this, socket));

    std::lock_guard<std::recursive_mutex> _(mutex);

    socket->enableKeepAlive();
    socket->enableNoDelay();
    socket->beginReceive(client.get());
    anonymousClients.emplace_back(std::move(client));
}

Server::Client::Client(Private *p, TcpSocket *socket)
    : p(p),
      socket(socket),
      hostIdStr()
{
}

Server::Client::~Client()
{
    socket.reset();
    p->commThread->removeEvents(this);
    p = nullptr;
}

void Server::Client::sendClosed(void)
{
    assert(p->commThread->onCommThread());

    system().traceMsg(
        "Server[%s]: Connection to \"%s\"[%s] was closed",
        p->hostIdStr.c_str(),
        moduleName.c_str(),
        hostIdStr.c_str());

    {
        std::lock_guard<std::recursive_mutex> _(p->mutex);

        // Abort all pending requests.
        for (const auto &i : requests)
        {
            Message response(500);
            response.setField("X-MessageId", i.c_str());
            p->callback->handleResponse(response);
        }

        requests.clear();

        for (auto i = p->clients.begin(); i != p->clients.end(); i++)
        {
            if (i->second.get() == this)
            {
                const auto obsolete = i->second.release();
                p->commThread->post([obsolete]
                {
                    delete obsolete;
                });

                p->clients.erase(i);
                break;
            }
        }
    }

    if (!hostIdStr.empty())
        p->callback->handleDisconnect(hostIdStr.c_str());
}

void Server::Client::sendSent(const char *, size_t)
{
}

void Server::Client::receiveClosed(void)
{
    sendClosed();
}

void Server::Client::receiveReceived(const char *buffer, size_t len)
{
    assert(p->commThread->onCommThread());

    const size_t pos = inbuffer.size();
    inbuffer.resize(pos + len);
    ::memcpy(&(inbuffer.front()) + pos, buffer, len);

    while (!inbuffer.empty())
    {
        const int msgSize = message.parse(&(inbuffer.front()), int(inbuffer.size()));
        if (msgSize > 0)
        {
            assert(inbuffer.size() >= size_t(msgSize));
            if (inbuffer.size() > size_t(msgSize))
            {
                ::memmove(&(inbuffer.front()), &(inbuffer.front()) + msgSize, inbuffer.size() - msgSize);
                inbuffer.resize(inbuffer.size() - msgSize);
            }
            else
                inbuffer.resize(0);

            // The first HostId will trigger the client to be moved from
            // anonymousClients to clients.
            if (hostIdStr.empty())
            {
                const char * const id = message.field("X-HostId");
                if (id)
                {
                    const CommThread::HostId host = id;

                    const char * const upgrade = message.field("Upgrade");
                    if (upgrade && (::strcmp(upgrade, "REMOTING-Bulk") == 0))
                    {
                        std::lock_guard<std::recursive_mutex> _(p->mutex);

                        std::vector<Server *> bulkServers;
                        if (!p->bulkServers.empty())
                        {
                            // Determine all available networks of the client.
                            std::set<IpAddress> networks;
                            for (unsigned i = 0;; i++)
                            {
                                const char * const network = message.field("X-Available-Network", i);
                                if (network)
                                {
                                    const IpAddress ip(network);
                                    if (ip)
                                        networks.insert(ip);
                                }
                                else
                                    break;
                            }

                            // Detemine which servers are reachable from the client.
                            for (const auto &i : p->bulkServers)
                            {
                                const IpAddress network = i->address() & i->netmask();
                                if (networks.find(network) != networks.end())
                                    bulkServers.push_back(i.get());
                            }
                        }

                        if (bulkServers.empty())
                        {
                            // Use the current connection if none of the bulk servers are reachable.

                            int numSockets = 1;
                            {
                                const char * const num = message.field("X-NumSockets");
                                if (num)
                                    numSockets = ::atoi(num);
                            }

                            p->bulkChannel->addSocket(socket.release(), host.data, static_cast<size_t>(numSockets));
                        }
                        else
                        {
                            Message response(307);
                            response.addField("Connection", "close");

                            for (const auto &i : bulkServers)
                            {
                                const IpAddress address = i->address();
                                assert(address);
                                assert(address.port() != 0);

                                std::ostringstream str;
                                str.imbue(std::locale::classic());
                                str << "http://" << address.toString(true, false) << '/';
                                response.addField("Location", str.str().c_str());
                            }

                            socket->beginSend(nullptr, response.header());
                        }

                        for (auto i = p->anonymousClients.begin(); i != p->anonymousClients.end();)
                        {
                            if (i->get() == this)
                            {
                                const auto obsolete = i->release();
                                p->commThread->post([obsolete]
                                {
                                    delete obsolete;
                                });

                                p->anonymousClients.erase(i);
                                break;
                            }
                            else
                                i++;
                        }

                        break;
                    }
                    else
                    {
                        {
                            std::lock_guard<std::recursive_mutex> _(p->mutex);

                            for (auto i = p->anonymousClients.begin();
                                 i != p->anonymousClients.end();
                                 i++)
                            {
                                if (i->get() == this)
                                {
                                    p->clients[id] = std::move(*i);
                                    p->anonymousClients.erase(i);
                                    hostIdStr = id;
                                    hostId = host;

                                    break;
                                }
                            }
                        }

                        p->callback->handleConnect(hostIdStr.c_str());
                    }
                }
            }

            if (::strcmp(message.requestMethod(), "GET") == 0)
            {
                p->callback->handleRequest(message, hostIdStr.c_str());
            }
            else if (::strcmp(message.requestMethod(), "NOTIFY") == 0)
            {
                p->callback->handleNotification(message);
            }
            else if (::strcmp(message.requestMethod(), "TRACE") == 0)
            {
                const char * const mname = message.field("X-ModuleName");
                moduleName = mname ? mname : "Unknown";

                system().traceMsg(
                    "Server[%s]: Incoming connection from \"%s\"[%s]",
                    p->hostIdStr.c_str(),
                    moduleName.c_str(),
                    hostIdStr.c_str());

                // Echo the received message.
                const std::vector<uint8_t> &content = message.content();
                if (!content.empty())
                {
                    const char * const sendbuf[2] = { message.header(), reinterpret_cast<const char *>(&(content[0])) };
                    const size_t sendlen[2] = { ::strlen(sendbuf[0]), content.size() };
                    socket->beginSend(nullptr, sendbuf, sendlen, 2);
                }
                else
                    socket->beginSend(nullptr, message.header());
            }
            else if (message.responseStatus() != 0) // Response
            {
                const char * const messageId = message.field("X-MessageId");
                assert(messageId);
                if (messageId != nullptr)
                {
                    std::unique_lock<std::recursive_mutex> l(p->mutex);

                    auto i = requests.find(messageId);
                    assert(i != requests.end());
                    if (i != requests.end())
                    {
                        requests.erase(i);
                        l.unlock();

                        p->callback->handleResponse(message);
                    }
                }
            }
            else
            {
                // Unsupported message
                Message response(405);
                socket->beginSend(nullptr, response.header());

                assert(false);
            }
        }
        else
            break;
    }
}


Server::Callback::Callback()
{
}

Server::Callback::~Callback()
{
}

} // End of namespaces
