/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifdef _MSC_VER
# pragma warning (disable : 4464) // Suppress warning including relative headers
#endif

#include "sscf1/client.h"
#include "sscf1/bulkchannel.h"
#include "sscf1/cipher.h"
#include "sscf1/system.h"
#include "sscf1/discovery.h"
#include "sscf1/message.h"
#include "sscf1/tcpsocket.h"
#include "../../include/sscf/commthread.h"

#include <atomic>
#include <cassert>
#include <cstring>
#include <iomanip>
#include <locale>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>

namespace Sscf1 {

struct Client::Private 
  : Discovery::SearchCallback,
    TcpSocket::ConnectCallback, 
    TcpSocket::SendCallback, 
    TcpSocket::ReceiveCallback
{
    Private(void *) 
      :	commThread(CommThread::instance()), 
        discovery(), 
        cipher(nullptr), 
        callback(nullptr),
        blockHandleDisconnect(false),
        connectFinishedStatus(false),
        messageId(0)
    {
    }

    Private()
      :	commThread(CommThread::instance()), 
        discovery(Discovery::instance()), 
        cipher(nullptr), 
        callback(nullptr),
        blockHandleDisconnect(false),
        connectFinishedStatus(false),
        messageId(0)
    {
    }

    virtual ~Private() { }

    std::mutex mutex;

    const std::shared_ptr<CommThread> commThread;
    const std::shared_ptr<Discovery> discovery;

    std::unique_ptr<CipherNull> myCipher;
    const Cipher *cipher;

    Callback *callback;
    IpAddress network, address;
    std::string resourceName;
    IpAddress::Interface iface;

    std::atomic<bool> connected;
    volatile bool blockHandleDisconnect;
    volatile bool connectFinishedStatus;
    std::condition_variable connectFinishedEvent;

    CommThread::HostId hostId;
    std::string hostIdStr;
    volatile uint64_t messageId;
    std::set<std::string> requests;

    std::unique_ptr<TcpSocket> socket;
    std::vector<char> inbuffer;

    std::unique_ptr<BulkChannel> bulkChannel;

    void resetConnection();

    virtual void searchFound(const IpAddress &);
    virtual void connectFinished(bool success);
    virtual void sendClosed(void);
    virtual void sendSent(const char *, size_t);
    virtual void receiveClosed(void);
    virtual void receiveReceived(const char *, size_t);
};

Client::Client(Callback *callback, IpAddress address, const Cipher *cipher)
  : p(new Private(nullptr))
{
    p->myCipher.reset((cipher == nullptr) ? new CipherNull() : nullptr);
    p->cipher = (cipher == nullptr) ? p->myCipher.get() : cipher;

    p->callback = callback;
    p->network = IpAddress::any(address.family());
    p->address = std::move(address);

    p->iface = IpAddress::findInterface(p->address);
    if (!p->iface)
        p->iface.address = IpAddress::any(p->address.family());

    p->connected = false;
    p->blockHandleDisconnect = false;
    p->connectFinishedStatus = true;

    p->hostId = CommThread::generateHostId();
    p->hostIdStr = p->hostId;
    p->messageId = 1;

    p->bulkChannel.reset(new BulkChannel(*p->cipher, p->hostId.data));
}

Client::Client(Callback *callback, IpAddress network, const char *resourceName, const Cipher *cipher)
  : p(new Private())
{
    p->myCipher.reset((cipher == nullptr) ? new CipherNull() : nullptr);
    p->cipher = (cipher == nullptr) ? p->myCipher.get() : cipher;

    p->callback = callback;
    p->network = std::move(network);
    p->resourceName = resourceName;

    p->iface = IpAddress::findInterface(p->network);
    if (!p->iface)
        p->iface.address = IpAddress::any(p->network.family());

    p->connected = false;
    p->blockHandleDisconnect = false;
    p->connectFinishedStatus = true;

    p->hostId = CommThread::generateHostId();
    p->hostIdStr = p->hostId;
    p->messageId = 1;

    p->socket = nullptr;
    p->bulkChannel.reset(new BulkChannel(*p->cipher, p->hostId.data));
}

Client::~Client()
{
    p->resetConnection();

    if (p->discovery)
        p->discovery->stopSearches(p);

    p->commThread->removeEvents(this);

    delete p;
    *const_cast<Private **>(&p) = nullptr;
}

bool Client::connect()
{
    p->blockHandleDisconnect = true;

    if (connectAsync())
    {
        std::unique_lock<std::mutex> l(p->mutex);
        while (!p->connectFinishedStatus)
            p->connectFinishedEvent.wait(l);

        if (p->connected)
        {
            p->blockHandleDisconnect = false;
            return true;
        }
    }
    else // Not a proper server, abort.
        p->resetConnection();

    p->blockHandleDisconnect = false;
    return false;
}

bool Client::connectAsync()
{
    if (!p->connectFinishedStatus)
    {
        system().fail(
            "connectAsync() already pending; wait for previous connectAsync() "
            "to finish first.");
    }

    p->resetConnection();
    p->connectFinishedStatus = false;

    if (p->iface)
    {
        p->socket.reset(new TcpSocket(p->iface.address.family(), *p->cipher));
        p->socket->enableKeepAlive();
        p->socket->enableNoDelay();
        p->socket->bind(nullptr, p->iface.address);

        if (!p->resourceName.empty() && p->discovery)
        {
            system().traceMsg(
                "Client[%s]: Starting connect to %s",
                p->hostIdStr.c_str(),
                p->resourceName.c_str());

            if (p->discovery->searchService(p, p->resourceName.c_str(), p->iface.address))
                return true;
        }
        else
        {
            system().traceMsg(
                "Client[%s]: Starting connect to %s",
                p->hostIdStr.c_str(),
                p->address.toString().c_str());

            if (p->socket->connect(p, p->address))
                return true;
        }
    }

    return false;
}

void Client::disconnect()
{
    p->resetConnection();
}

bool Client::isConnected(void) const
{
    return p->connected;
}

BulkChannel & Client::bulkChannel()
{
    return *p->bulkChannel;
}

const char * Client::hostId(void) const
{
    return p->hostIdStr.c_str();
}

void Client::makeMessageId(Message &message)
{
    uint64_t messageId;
    {
        std::lock_guard<std::mutex> _(p->mutex);

        messageId = p->messageId++;
    }

    std::ostringstream messageIdStr;
    messageIdStr.imbue(std::locale::classic());
    messageIdStr << std::hex << std::setfill('0') << std::setw(16) << messageId;

    message.setField("X-MessageId", messageIdStr.str().c_str());
    message.setField("X-HostId", p->hostIdStr.c_str());
}

void Client::copyMessageId(const Message &source, Message &destination)
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

bool Client::sendRequest(const Message &request, unsigned timeout)
{
    using namespace std::chrono;

    bool result = false;

    const char * const messageId = request.field("X-MessageId");
    const char * const hostIdStr = request.field("X-HostId");
    assert(messageId && hostIdStr);
    if (messageId && hostIdStr)
    {
        std::lock_guard<std::mutex> _(p->mutex);

        if (p->connected)
        {
            p->requests.insert(messageId);

            const std::vector<uint8_t> &content = request.content();
            if (!content.empty())
            {
                const char * const sendbuf[2] = { request.header(), reinterpret_cast<const char *>(&(content[0])) };
                const size_t sendlen[2] = { ::strlen(sendbuf[0]), content.size() };
                p->socket->beginSend(p, sendbuf, sendlen, 2);
            }
            else
                p->socket->beginSend(p, request.header());

            if (timeout)
            {
                const std::string id = messageId;
                p->commThread->setTimerEvent(this, milliseconds(timeout), [this, id]
                {
                    std::unique_lock<std::mutex> l(p->mutex);

                    auto i = p->requests.find(id);
                    if (i != p->requests.end())
                    {
                        p->requests.erase(i);
                        l.unlock();

                        if (p->connected)
                        {
                            p->resetConnection();
                            p->callback->handleDisconnect();
                        }

                        Message response(504);
                        response.setField("X-MessageId", id.c_str());
                        p->callback->handleResponse(response);
                    }
                });
            }

            result = true;
        }
    }

    return result;
}

bool Client::sendNotification(const Message &notification)
{
    bool result = false;

    const char * const messageId = notification.field("X-MessageId");
    const char * const hostIdStr = notification.field("X-HostId");
    assert(messageId && hostIdStr);
    if (messageId && hostIdStr)
    {
        std::lock_guard<std::mutex> _(p->mutex);

        if (p->connected)
        {
            const std::vector<uint8_t> &content = notification.content();
            if (!content.empty())
            {
                const char * const sendbuf[2] = { notification.header(), reinterpret_cast<const char *>(&(content[0])) };
                const size_t sendlen[2] = { ::strlen(sendbuf[0]), content.size() };
                p->socket->beginSend(p, sendbuf, sendlen, 2);
            }
            else
                p->socket->beginSend(p, notification.header());

            result = true;
        }
    }

    return result;
}

bool Client::sendResponse(const Message &response)
{
    bool result = false;

    std::lock_guard<std::mutex> _(p->mutex);

    if (p->connected)
    {
        const std::vector<uint8_t> &content = response.content();
        if (!content.empty())
        {
            const char * const sendbuf[2] = { response.header(), reinterpret_cast<const char *>(&(content[0])) };
            const size_t sendlen[2] = { ::strlen(sendbuf[0]), content.size() };
            p->socket->beginSend(p, sendbuf, sendlen, 2);
        }
        else
            p->socket->beginSend(p, response.header());

        result = true;
    }

    return result;
}


void Client::Private::resetConnection()
{
    if (socket)
    {
        system().traceMsg(
            "Client[%s]: Connection closed",
            hostIdStr.c_str());

        std::unique_ptr<TcpSocket> sock;
        std::vector<std::string> pending;
        {
            std::lock_guard<std::mutex> _(mutex);

            connected = false;

            sock = std::move(socket);

            for (const auto &i : requests)
                pending.push_back(i);

            requests.clear();
        }

        // Abort all pending requests.
        for (const auto &i : pending)
        {
            Message response(500);
            response.setField("X-MessageId", i.c_str());
            callback->handleResponse(response);
        }

        sock = nullptr;

        std::lock_guard<std::mutex> _(mutex);
        if (!connectFinishedStatus)
        {
            connectFinishedStatus = true;
            connectFinishedEvent.notify_all();
        }
    }
}

void Client::Private::searchFound(const IpAddress &service)
{
    if (!connectFinishedStatus)
    {
        std::unique_lock<std::mutex> l(mutex);

        if (socket)
        {
            if (service)
            {
                address = service;
                if (socket->connect(this, address))
                    return;
            }

            connectFinishedStatus = true;
            connectFinishedEvent.notify_all();

            if (!blockHandleDisconnect)
            {
                l.unlock();
                callback->handleDisconnect();
                l.lock();
            }
        }
    }
}

void Client::Private::connectFinished(bool success)
{
    if (!connectFinishedStatus)
    {
        std::unique_lock<std::mutex> l(mutex);

        if (success)
        {
            bulkChannel->setServerAddress(address, network);

            socket->beginReceive(this);

            // Send a TRACE to check if the HTTP connection is Ok.
            Message request("TRACE", "*");
            request.setField("X-HostId", hostIdStr.c_str());
            request.setField("X-ModuleName", commThread->moduleName().c_str());

            const std::vector<uint8_t> &content = request.content();
            if (!content.empty())
            {
                const char * const sendbuf[2] = { request.header(), reinterpret_cast<const char *>(&(content[0])) };
                const size_t sendlen[2] = { ::strlen(sendbuf[0]), content.size() };
                socket->beginSend(this, sendbuf, sendlen, 2);
            }
            else
                socket->beginSend(this, request.header());

            // recvReceived TRACE will handle the rest.
        }
        else
        {
            connectFinishedStatus = true;
            connectFinishedEvent.notify_all();

            if (!blockHandleDisconnect)
            {
                l.unlock();
                callback->handleDisconnect();
                l.lock();
            }
        }
    }
}

void Client::Private::sendClosed(void)
{
    system().traceMsg(
        "Client[%s]: Connection terminated by server",
        hostIdStr.c_str());

    std::vector<std::string> pending;
    {
        std::lock_guard<std::mutex> _(mutex);

        // Abort all pending requests.
        for (const auto &i : requests)
            pending.push_back(i);

        requests.clear();
    }

    // Abort all pending requests.
    for (const auto &i : pending)
    {
        Message response(500);
        response.setField("X-MessageId", i.c_str());
        callback->handleResponse(response);
    }

    connected = false;

    if (!connectFinishedStatus)
    {
        connectFinishedStatus = true;
        connectFinishedEvent.notify_all();
    }

    if (!blockHandleDisconnect)
        callback->handleDisconnect();
}

void Client::Private::sendSent(const char *, size_t)
{
}

void Client::Private::receiveClosed(void)
{
    sendClosed();
}

void Client::Private::receiveReceived(const char *buffer, size_t len)
{
    const size_t pos = inbuffer.size();
    inbuffer.resize(pos + len);
    ::memcpy(&(inbuffer.front()) + pos, buffer, len);

    while (!inbuffer.empty())
    {
        Message message;
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

            if (::strcmp(message.requestMethod(), "GET") == 0)
            {
                callback->handleRequest(message);
            }
            else if (::strcmp(message.requestMethod(), "NOTIFY") == 0)
            {
                callback->handleNotification(message);
            }
            else if (::strcmp(message.requestMethod(), "TRACE") == 0)
            {
                std::unique_lock<std::mutex> l(mutex);

                if (!connected && socket)
                {
                    if (!connectFinishedStatus)
                    {
                        connectFinishedStatus = true;
                        connectFinishedEvent.notify_all();
                    }
                    
                    const char * const str = message.field("X-HostId");
                    if (str && (hostIdStr == str))
                    {
                        connected = true;

                        l.unlock();
                        callback->handleConnect();
                        l.lock();
                    }
                    else if (!blockHandleDisconnect)
                    {
                        l.unlock();
                        callback->handleDisconnect();
                        l.lock();
                    }
                }
            }
            else if (message.responseStatus() != 0)
            {
                const char * const id = message.field("X-MessageId");
                assert(id);
                if (id != nullptr)
                {
                    std::unique_lock<std::mutex> l(mutex);

                    auto i = requests.find(id);
                    if (i != requests.end())
                    {
                        requests.erase(i);
                        l.unlock();

                        callback->handleResponse(message);
                    }
                }
            }
            else
            {
                // Unsupported message
                assert(false);
            }
        }
        else
            break;
    }
}


Client::Callback::Callback()
{
}

Client::Callback::~Callback()
{
}

} // End of namespaces
