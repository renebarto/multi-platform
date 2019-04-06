/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifdef _MSC_VER
# ifndef _CRT_SECURE_NO_WARNINGS
#  define _CRT_SECURE_NO_WARNINGS 1
# endif
# ifndef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
#  define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
# endif
# ifndef _WIN32_WINNT_WINTHRESHOLD
#  define _WIN32_WINNT_WINTHRESHOLD           0x0601
# endif
# ifndef NTDDI_WIN7SP1
#  define NTDDI_WIN7SP1                       0x0601010
# endif
#endif

#define SSCF1_BULKCHANNEL_CPP
#include "sscf1/bulkchannel.h"
#include "sscf1/cipher.h"
#include "sscf1/ipaddress.h"
#include "sscf1/message.h"
#include "sscf1/system.h"
#include "sscf1/tcpsocket.h"
#include "sscf/byteorder.h"
#include "sscf/commthread.h"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <locale>
#include <map>
#include <set>
#include <sstream>
#include <vector>
#include <atomic>
#include <condition_variable>
#include <mutex>
#ifdef _MSC_VER
# pragma warning (default : 4265)
#endif

#if defined(_MSC_VER)
# define noexcept throw()
#endif

// define ANSI/ISO names
#ifdef _MSC_VER
# define snprintf _snprintf
#endif

namespace Sscf1 {

// Keep in sync with BulkId declaration in bulkchannel.h
struct BulkChannel::BulkId
{
    union
    {
#ifdef _MSC_VER
# pragma pack(1)
#endif
        struct
        {
            uint8_t  hostId[16];
            uint32_t dataId;
            uint64_t size;
        }
#ifdef __GNUC__
        __attribute__((packed))
#endif
        bigEndian;
#ifdef _MSC_VER
# pragma pack()
#endif

        char data[(sizeof(uint8_t) * 16) + sizeof(uint32_t) + sizeof(uint64_t)];
    };
};

struct BulkChannel::Buffer::Private
{
    std::atomic<int> refcount;

    const uint8_t * data;
    size_t size;
    void(*released)(void *cookie);
    void *cookie;
};

struct BulkChannel::Client
{
    typedef char Opcode;
    static const Opcode opActivate    = 'A';
    static const Opcode opRequestData = 'Q';
    static const Opcode opRespondData = 'R';
    static const Opcode opForgetData  = 'F';

    struct OpCodeBuffer
    {
        union
        {
#ifdef _MSC_VER
# pragma pack(1)
#endif
            struct
            {
                Opcode opcode;
                uint32_t dataId;
                uint64_t offset;
                uint64_t size;
            }
#ifdef __GNUC__
            __attribute__((packed))
#endif
            bigEndian;
#ifdef _MSC_VER
# pragma pack()
#endif

            char data[sizeof(Opcode) + sizeof(uint32_t) + sizeof(uint64_t) + sizeof(uint64_t)];
        };
    };

    struct Socket
      : TcpSocket::SendCallback,
        TcpSocket::ReceiveBulkCallback
    {
        Socket(Client *parent, TcpSocket *socket);
        virtual ~Socket();

        Client *parent;
        std::unique_ptr<TcpSocket> socket;

        uint32_t nextDataId;
        uint64_t nextOffset;
        uint64_t nextSize;

        OpCodeBuffer opcodeBuffer;
        size_t receiveSize;

        virtual void sendClosed(void);
        virtual void sendSent(const char *buffer, size_t len);
        virtual void receiveClosed(void);
        virtual void acquireBuffer(char *&buffer, size_t &len);
        virtual void receiveReceived(const char *, size_t);
    };

    Client(Private *, const CommThread::HostId &, size_t requiredSockets);
    virtual ~Client();

    int remainingSockets() const;
    void addSocket(TcpSocket *);
    void removeSocket(Socket *);

    Private *p;

    CommThread::HostId hostId;
    size_t requiredSockets;
    std::vector<std::unique_ptr<Socket>> pendingSockets;
    std::vector<std::unique_ptr<Socket>> sockets;

    std::map<const void *, Buffer> sendQueue;
    std::map<uint32_t, std::pair<Buffer, uint64_t>> receiveQueue;
};

struct BulkChannel::Private
{
    struct ConnectingSocket
      : TcpSocket::ConnectCallback,
        TcpSocket::ReceiveCallback
    {
        ConnectingSocket(Private *p, IpAddress iface);
        virtual ~ConnectingSocket();

        Private *p;
        IpAddress iface;
        std::unique_ptr<TcpSocket> socket;

        std::vector<char> connectingBuffer;

        virtual void connectFinished(bool success);
        virtual void receiveClosed(void);
        virtual void receiveReceived(const char *, size_t);
    };

    struct SendQueueItem
    {
        SendQueueItem() : retrieved(0), recipients(0) { }

        Buffer buffer;
        uint64_t retrieved;
        unsigned recipients;
    };

    Private() : cipher(nullptr), nextDataId(0), commThread(CommThread::instance()) { }

    const Cipher *cipher;
    const CipherNull cipherNull;
    CommThread::HostId hostId;
    struct ServerAddress { IpAddress address, network; };
    std::vector<ServerAddress> serverAddress;

    uint32_t nextDataId;

    const std::shared_ptr<CommThread> commThread;

    std::vector<Allocator *> allocators;
    std::vector<std::unique_ptr<ConnectingSocket>> connectingSockets;
    std::map<uint32_t, SendQueueItem> sendQueue;

    // This mutex to protect clients.
    std::mutex mutex;
    std::condition_variable condition;
    std::map<CommThread::HostId, std::unique_ptr<Client>> clients;

    void checkConnection();
    void removeConnectingSocket(ConnectingSocket *);
};

BulkChannel::BulkChannel(const Cipher &cipher, const uint8_t(&hostId)[16])
  : p(new Private())
{
    static_assert(sizeof(BulkId) == 28, "BulkId is not properly packed.");
    static_assert(sizeof(Client::OpCodeBuffer) == 21, "Client::OpCodeBuffer is not properly packed.");

    p->cipher = &cipher;
    p->hostId = hostId;

    p->nextDataId = 1;
}

BulkChannel::~BulkChannel()
{
    assert(!p->commThread->onCommThread());

    p->commThread->send([this]
    {
        {
            std::lock_guard<std::mutex> _(p->mutex);

            p->clients.clear();
        }

        p->connectingSockets.clear();
    });

    // When this assert fails there were bulk data objects queued but not
    // retrieved or forgotten. This may result in a, possibly large, memory
    // leak.
    assert(p->sendQueue.empty());

    delete p;
    *const_cast<Private **>(&p) = nullptr;
}

void BulkChannel::setServerAddress(IpAddress address, IpAddress network)
{
    p->commThread->post([this, address, network]
    {
        Private::ServerAddress addr = { address, network };

        p->serverAddress.clear();
        p->serverAddress.push_back(addr);
    });
}

void BulkChannel::addSocket(TcpSocket *socket, const uint8_t(&hostId_)[16], size_t numSockets)
{
    Message response(101);
    response.setField("Upgrade", "SSCF-Bulk");
    response.setField("Connection", "Upgrade");
    response.setField("X-HostId", std::string(p->hostId).c_str());
    std::ostringstream str; str.imbue(std::locale::classic()); str << numSockets;
    response.setField("X-NumSockets", str.str().c_str());
    socket->beginSend(nullptr, response.header());

    const CommThread::HostId hostId(hostId_);
    p->commThread->send([this, socket, hostId, numSockets]
    {
        std::lock_guard<std::mutex> _(p->mutex);

        auto client = p->clients.find(hostId);
        if (client == p->clients.end())
        {
            client = p->clients.emplace(
                hostId,
                std::unique_ptr<Client>(new Client(p, hostId, numSockets))).first;

            p->condition.notify_all();
        }

        client->second->addSocket(socket);
    });
}

BulkChannel::BulkId BulkChannel::enqueue(const Buffer &buffer)
{
    uint32_t dataId = 0;
    p->commThread->send([this, buffer, &dataId]
    {
        dataId = p->nextDataId++;
        if (dataId == 0)
            dataId = p->nextDataId++;

        p->sendQueue[dataId].buffer = buffer;
    });

    p->commThread->post([this]
    {
        p->checkConnection();
    });

    const uint32_t dataIdBigEndian = eswap(dataId);
    const uint64_t sizeBigEndian = eswap(uint64_t(buffer.size()));
    BulkChannel::BulkId id;
    memcpy(id.data, p->hostId.data, sizeof(p->hostId.data));
    memcpy(id.data + sizeof(p->hostId.data), &dataIdBigEndian, sizeof(dataIdBigEndian));
    memcpy(id.data + sizeof(p->hostId.data) + sizeof(dataIdBigEndian), &sizeBigEndian, sizeof(sizeBigEndian));
    return id;
}

void BulkChannel::addRecipient(const BulkId &id)
{
    uint32_t dataIdBigEndian;
    memcpy(&dataIdBigEndian, id.data + sizeof(p->hostId.data), sizeof(dataIdBigEndian));

    const uint32_t dataId = eswap(dataIdBigEndian);
    p->commThread->post([this, dataId]
    {
        auto i = p->sendQueue.find(dataId);
        if (i != p->sendQueue.end())
            i->second.recipients++;
    });
}

void BulkChannel::addRecipients(const BulkId *ids, size_t size)
{
    for (size_t i = 0; i < size; i++)
        addRecipient(ids[i]);
}

BulkChannel::Buffer BulkChannel::retrieve(const BulkId &id)
{
    p->commThread->post([this]
    {
        p->checkConnection();
    });

    const CommThread::HostId hostId(id.bigEndian.hostId);

    // Wait for the connection to be established.
    {
        std::unique_lock<std::mutex> l(p->mutex);

        auto client = p->clients.find(hostId);
        while ((client == p->clients.end()) || (client->second->sockets.size() < client->second->requiredSockets))
        {
            system().traceMsg(
                "BulkChannel: Waiting for connection [%s]->[%s]",
                std::string(p->hostId).c_str(),
                std::string(hostId).c_str());

            p->condition.wait(l);

            client = p->clients.find(hostId);
        }
    }

    const uint32_t dataId = eswap(id.bigEndian.dataId);
    const uint64_t size = eswap(id.bigEndian.size);

    bool success = false;
    p->commThread->send([this, hostId, dataId, size, &success]
    {
        std::unique_lock<std::mutex> l(p->mutex);

        auto client = p->clients.find(hostId);
        if (client != p->clients.end())
        {
            // Send the requests.
            if ((size > 0) && (size < (SIZE_MAX / 32)))
            {
                for (size_t i = 0, n = client->second->sockets.size(); i < n; i++)
                {
                    const uint64_t begin = (size * (i + 0)) / n;
                    const uint64_t end = (size * (i + 1)) / n;

                    Client::OpCodeBuffer request;
                    request.bigEndian.opcode = eswap(Client::opRequestData);
                    request.bigEndian.dataId = eswap(dataId);
                    request.bigEndian.offset = eswap(begin);
                    request.bigEndian.size   = eswap(end - begin);
                    client->second->sockets[i]->socket->beginSend(nullptr, request.data, sizeof(request.data));
                }

                Buffer buffer;
                for (auto i = p->allocators.rbegin();
                     (i != p->allocators.rend()) && (buffer.size() < size);
                     i++)
                {
                    buffer = (*i)->allocate(size_t(size));
                }

                if (buffer.size() < size)
                {
                    uint8_t * const mem = new uint8_t[size_t(size) + 64];
                    uint8_t * const alignedMem = (uint8_t *)((uintptr_t(mem) + 63) & ~uintptr_t(63));
                    buffer = Buffer(alignedMem, size_t(size), operator delete [], mem);
                }

                client->second->receiveQueue[dataId] = std::make_pair(buffer, uint64_t(0));

                success = true;
            }
            else
            {
                // Notify the other end to drop the buffer.
                Client::OpCodeBuffer request;
                request.bigEndian.opcode = eswap(Client::opForgetData);
                request.bigEndian.dataId = eswap(dataId);
                request.bigEndian.offset = 0;
                request.bigEndian.size   = 0;
                client->second->sockets.front()->socket->beginSend(nullptr, request.data, sizeof(request.data));
            }
        }
    });

    // Wait for the replies.
    if (success)
    {
        std::unique_lock<std::mutex> l(p->mutex);

        auto client = p->clients.find(hostId);
        std::map<uint32_t, std::pair<Buffer, uint64_t>>::iterator reply;
        if (client != p->clients.end())
        {
            reply = client->second->receiveQueue.find(dataId);
            while ((client != p->clients.end()) &&
                   (reply != client->second->receiveQueue.end()) &&
                   (reply->second.second < size))
            {
                p->condition.wait(l);

                client = p->clients.find(hostId);
                if (client != p->clients.end())
                    reply = client->second->receiveQueue.find(dataId);
            }
        }

        if ((client != p->clients.end()) &&
            (reply != client->second->receiveQueue.end()))
        {
            const Buffer result = reply->second.first;
            client->second->receiveQueue.erase(reply);
            return result;
        }
    }

    return Buffer();
}

void BulkChannel::discard(const BulkId &id)
{
    uint32_t dataIdBigEndian;
    memcpy(&dataIdBigEndian, id.data + sizeof(p->hostId.data), sizeof(dataIdBigEndian));

    const uint32_t dataId = eswap(dataIdBigEndian);
    p->commThread->post([this, dataId]
    {
        auto i = p->sendQueue.find(dataId);
        if ((i != p->sendQueue.end()) &&
            ((i->second.recipients == 0) || (--(i->second.recipients) == 0)))
        {
            p->sendQueue.erase(i);
        }
    });
}

void BulkChannel::discard(const BulkId *ids, size_t size)
{
    for (size_t i = 0; i < size; i++)
        discard(ids[i]);
}

void BulkChannel::Private::checkConnection()
{
    assert(commThread->onCommThread());

    for (auto &i : serverAddress)
    {
        bool needSocket = false;
        {
            std::lock_guard<std::mutex> _(mutex);

            if (clients.empty())
                needSocket = connectingSockets.size() < serverAddress.size();
            else
                needSocket = clients.begin()->second->remainingSockets() > int(connectingSockets.size());
        }

        if (needSocket)
        {
            ConnectingSocket *socket = new ConnectingSocket(this, i.address);

            if (auto iface = IpAddress::findInterface(i.network))
                socket->socket->bind(nullptr, iface.address);

            connectingSockets.emplace_back(socket);
            socket->socket->connect(socket, i.address);

            system().traceMsg(
                "BulkChannel: Connecting [%s] to address=%s",
                std::string(hostId).c_str(),
                i.address.toString().c_str());
        }
    }
}

void BulkChannel::Private::removeConnectingSocket(ConnectingSocket *socket)
{
    assert(commThread->onCommThread());

    for (auto i = connectingSockets.begin(); i != connectingSockets.end(); )
    if (i->get() == socket)
    {
        const auto obsolete = i->release();
        commThread->post([obsolete]
        {
            delete obsolete;
        });

        i = connectingSockets.erase(i);
    }
    else
        i++;
}

BulkChannel::Private::ConnectingSocket::ConnectingSocket(Private *p, IpAddress iface_)
  : p(p),
    iface(std::move(iface_)),
    socket(new TcpSocket(iface.family(), *p->cipher))
{
    socket->enableKeepAlive();
    socket->enableNoDelay();
}

BulkChannel::Private::ConnectingSocket::~ConnectingSocket()
{
}

void BulkChannel::Private::ConnectingSocket::connectFinished(bool success)
{
    assert(p->commThread->onCommThread());

    if (success)
    {
        connectingBuffer.clear();
        socket->beginReceive(this);

        Message request("GET", "/sscf/bulk");
        request.setField("Upgrade", "SSCF-Bulk");
        request.setField("Connection", "Upgrade");
        request.setField("X-HostId", std::string(p->hostId).c_str());

        {
            std::ostringstream str; str.imbue(std::locale::classic()); str << p->serverAddress.size();
            request.setField("X-NumSockets", str.str().c_str());
        }

        const IpAddress &loopback = IpAddress::loopback(iface.family());
        for (auto &i : IpAddress::listInterfaces())
        {
            if (i.address.family() == iface.family())
            {
                const IpAddress network = i.address & i.netmask;
                if (network != (loopback & i.netmask))
                    request.addField("X-Available-Network", network.toString().c_str());
            }
        }

        socket->beginSend(nullptr, request.header());
    }
    else
        p->removeConnectingSocket(this);

    p->checkConnection();
}

void BulkChannel::Private::ConnectingSocket::receiveClosed(void)
{
    assert(p->commThread->onCommThread());

    p->removeConnectingSocket(this);
    p->checkConnection();
}

void BulkChannel::Private::ConnectingSocket::receiveReceived(const char *buffer, size_t len)
{
    assert(p->commThread->onCommThread());

    const size_t pos = connectingBuffer.size();
    connectingBuffer.resize(pos + len);
    ::memcpy(&(connectingBuffer.front()) + pos, buffer, len);

    while (!connectingBuffer.empty())
    {
        Message message;
        const int msgSize = message.parse(&(connectingBuffer.front()), int(connectingBuffer.size()));
        if (msgSize > 0)
        {
            assert(connectingBuffer.size() >= size_t(msgSize));
            if (connectingBuffer.size() > size_t(msgSize))
            {
                ::memmove(&(connectingBuffer.front()), &(connectingBuffer.front()) + msgSize, connectingBuffer.size() - msgSize);
                connectingBuffer.resize(connectingBuffer.size() - msgSize);
            }
            else
                connectingBuffer.resize(0);

            if (message.responseStatus() == 101) // Upgrade connection
            {
                size_t numSockets = 0;
                const char * const num = message.field("X-NumSockets");
                if (num)
                    numSockets = static_cast<size_t>(::atoi(num));

                const char * const id = message.field("X-HostId");
                if (id)
                {
                    system().traceMsg(
                        "BulkChannel: Connection established [%s]->[%s]",
                        id, std::string(p->hostId).c_str());

                    const CommThread::HostId host(id);

                    const auto sock = socket.release();
                    {
                        std::lock_guard<std::mutex> _(p->mutex);

                        auto client = p->clients.find(host);
                        if (client == p->clients.end())
                        {
                            client = p->clients.emplace(
                                host,
                                std::unique_ptr<Client>(new Client(p, host, numSockets))).first;

                            p->condition.notify_all();
                        }

                        client->second->addSocket(sock);
                    }

                    Client::OpCodeBuffer activate;
                    activate.bigEndian.opcode = eswap(Client::opActivate);
                    activate.bigEndian.dataId = 0;
                    activate.bigEndian.offset = 0;
                    activate.bigEndian.size   = 0;
                    sock->beginSend(nullptr, activate.data, sizeof(activate.data));

                    p->removeConnectingSocket(this);
                    p->checkConnection();
                    break;
                }
            }
            else if (message.responseStatus() == 307) // Temporary Redirect
            {
                p->serverAddress.clear();
                for (unsigned i = 0;; i++)
                {
                    const char * const location = message.field("Location", i);
                    if (location)
                    {
                        std::string buf = location;
                        while (!buf.empty() && (buf.back() == '/'))
                            buf.pop_back();

                        uint16_t port = 0;
                        const auto lcolon = buf.find_last_of(':');
                        if (lcolon != buf.npos)
                        {
                            buf[lcolon] = '\0';
                            port = uint16_t(std::atoi(&buf[lcolon + 1]));
                        }

                        const auto scheme = buf.find("://");
                        if (scheme != buf.npos)
                        {
                            IpAddress address(&buf[scheme + 3], port);
                            Private::ServerAddress sa = { address, address };
                            p->serverAddress.emplace_back(std::move(sa));
                        }
                    }
                    else
                        break;
                }

                p->removeConnectingSocket(this);
                p->checkConnection();
                break;
            }

            assert(false);
        }
        else
            break;
    }
}

BulkChannel::Client::Client(Private *p, const CommThread::HostId &hostId, size_t requiredSockets)
  : p(p),
    hostId(hostId),
    requiredSockets(requiredSockets)
{
}

BulkChannel::Client::~Client()
{
}

int BulkChannel::Client::remainingSockets() const
{
    const int result = int(requiredSockets) - int(pendingSockets.size() + sockets.size());
    assert(result >= 0);

    return result;
}

void BulkChannel::Client::addSocket(TcpSocket *socket)
{
    static const uint32_t bufferSize = 1048576;

    std::unique_ptr<Socket> s(new Socket(this, socket));
    s->socket->setCipher(p->cipherNull);
    s->socket->setBufferSize(bufferSize, bufferSize);
    s->socket->beginReceiveBulk(s.get());

    pendingSockets.emplace_back(std::move(s));
}

void BulkChannel::Client::removeSocket(Socket *socket)
{
    assert(p->commThread->onCommThread());
    std::lock_guard<std::mutex> _(p->mutex);

    for (auto i = pendingSockets.begin(); i != pendingSockets.end();)
    if (i->get() == socket)
        i = pendingSockets.erase(i);
    else
        i++;

    for (auto i = sockets.begin(); i != sockets.end();)
    if (i->get() == socket)
        i = sockets.erase(i);
    else
        i++;

    if (pendingSockets.empty() && sockets.empty())
    {
        receiveQueue.clear();

        for (auto i = p->clients.begin(); i != p->clients.end(); i++)
        {
            if (i->second.get() == this)
            {
                const auto obsolete = i->second.release();
                p->commThread->post([obsolete]
                {
                    delete obsolete;
                });

                system().traceMsg(
                    "BulkChannel: Connection closed [%s]->[%s]",
                    std::string(i->first).c_str(),
                    std::string(p->hostId).c_str());

                p->clients.erase(i);
                break;
            }
        }
    }
}

BulkChannel::Client::Socket::Socket(Client *parent, TcpSocket *socket) 
  : parent(parent), 
    socket(socket),
    nextDataId(0),
    nextOffset(0),
    nextSize(0),
    receiveSize(0)
{
}

BulkChannel::Client::Socket::~Socket() 
{
}

void BulkChannel::Client::Socket::sendClosed(void)
{
    parent->removeSocket(this);
}

void BulkChannel::Client::Socket::sendSent(const char *buffer, size_t)
{
    assert(parent->p->commThread->onCommThread());

    auto i = parent->sendQueue.find(buffer);
    assert(i != parent->sendQueue.end());
    if (i != parent->sendQueue.end())
        parent->sendQueue.erase(i);
}

void BulkChannel::Client::Socket::receiveClosed(void)
{
    parent->removeSocket(this);
}

void BulkChannel::Client::Socket::acquireBuffer(char *&buffer, size_t &len)
{
    if (nextDataId == 0)
    {
        len = sizeof(OpCodeBuffer) - receiveSize;
        buffer = &opcodeBuffer.data[receiveSize];
    }
    else
    {
        std::lock_guard<std::mutex> _(parent->p->mutex);

        auto reply = parent->receiveQueue.find(nextDataId);
        if (reply != parent->receiveQueue.end())
        {
            len = size_t(std::min(nextSize, reply->second.first.size() - nextOffset) - receiveSize);
            buffer = (char *)(reply->second.first.data() + nextOffset + receiveSize);
        }
    }
}

void BulkChannel::Client::Socket::receiveReceived(const char *, size_t len)
{
    assert(parent->p->commThread->onCommThread());

    receiveSize += len;

    if (nextDataId == 0)
    {
        if (receiveSize >= sizeof(OpCodeBuffer))
        {
            assert(receiveSize == sizeof(OpCodeBuffer));

            switch (eswap(opcodeBuffer.bigEndian.opcode))
            {
            case opActivate:
                {
                    for (auto i = parent->pendingSockets.begin(); i != parent->pendingSockets.end(); i++)
                    if (i->get() == this)
                    {
                        system().traceMsg(
                            "BulkChannel: Received opActivate [%s]",
                            std::string(parent->p->hostId).c_str());

                        if (eswap(opcodeBuffer.bigEndian.dataId) == 0)
                        {
                            OpCodeBuffer response;
                            response.bigEndian.opcode = eswap(opActivate);
                            response.bigEndian.dataId = eswap(uint32_t(1));
                            response.bigEndian.offset = 0;
                            response.bigEndian.size   = 0;
                            socket->beginSend(nullptr, response.data, sizeof(response.data));
                        }

                        std::lock_guard<std::mutex> _(parent->p->mutex);

                        parent->sockets.emplace_back(std::move(*i));
                        parent->pendingSockets.erase(i);

                        if (parent->sockets.size() >= parent->requiredSockets)
                            parent->p->condition.notify_all();

                        break;
                    }
                }
                break;

            case opRequestData:
                {
                    const uint32_t dataId = eswap(opcodeBuffer.bigEndian.dataId);
                    const uint64_t offset = eswap(opcodeBuffer.bigEndian.offset);
                    const uint64_t size   = eswap(opcodeBuffer.bigEndian.size  );
                    auto i = parent->p->sendQueue.find(dataId);
                    if (i != parent->p->sendQueue.end())
                    {
                        const uint64_t actualOffset = std::min(offset, uint64_t(i->second.buffer.size()));
                        const uint64_t actualSize   = std::min(size  , uint64_t(i->second.buffer.size() - actualOffset));

                        OpCodeBuffer response;
                        response.bigEndian.opcode = eswap(opRespondData);
                        response.bigEndian.dataId = eswap(dataId);
                        response.bigEndian.offset = eswap(actualOffset);
                        response.bigEndian.size   = eswap(actualSize);
                        socket->beginSend(nullptr, response.data, sizeof(response.data));

                        if (actualSize > 0)
                        {
                            parent->sendQueue[i->second.buffer.data() + actualOffset] = i->second.buffer;
                            socket->beginSendBulk(this, reinterpret_cast<const char *>(i->second.buffer.data() + actualOffset), size_t(actualSize));
                        }

                        i->second.retrieved += actualSize;
                        if (i->second.retrieved >= (i->second.buffer.size() * std::max(1u, i->second.recipients)))
                            parent->p->sendQueue.erase(i);
                    }
                    else
                    {
                        OpCodeBuffer response;
                        response.bigEndian.opcode = eswap(opRespondData);
                        response.bigEndian.dataId = eswap(dataId);
                        response.bigEndian.offset = eswap(uint64_t(0));
                        response.bigEndian.size   = eswap(uint64_t(-1));
                        socket->beginSend(nullptr, response.data, sizeof(response.data));
                    }
                }
                break;

            case opRespondData:
                {
                    nextDataId = eswap(opcodeBuffer.bigEndian.dataId);
                    nextOffset = eswap(opcodeBuffer.bigEndian.offset);
                    nextSize   = eswap(opcodeBuffer.bigEndian.size  );

                    if (nextSize == 0)
                    {
                        std::lock_guard<std::mutex> _(parent->p->mutex);

                        parent->p->condition.notify_all();

                        nextDataId = 0;
                        nextOffset = 0;
                    }
                    else if (nextSize == uint64_t(-1)) // Failed.
                    {
                        std::lock_guard<std::mutex> _(parent->p->mutex);

                        auto reply = parent->receiveQueue.find(nextDataId);
                        if (reply != parent->receiveQueue.end())
                        {
                            reply->second.first = Buffer();
                            reply->second.second = nextSize;
                            parent->p->condition.notify_all();
                        }

                        nextDataId = 0;
                        nextOffset = 0;
                        nextSize   = 0;
                    }
                }
                break;

            case opForgetData:
                {
                    const uint32_t dataId = eswap(opcodeBuffer.bigEndian.dataId);
                    auto i = parent->p->sendQueue.find(dataId);
                    if (i != parent->p->sendQueue.end())
                        parent->p->sendQueue.erase(i);
                }
                break;
            }

            receiveSize = 0;
        }
    }
    else if (receiveSize >= nextSize)
    {
        std::lock_guard<std::mutex> _(parent->p->mutex);

        assert(receiveSize == nextSize);

        auto reply = parent->receiveQueue.find(nextDataId);
        if (reply != parent->receiveQueue.end())
        {
            reply->second.second += nextSize;
            parent->p->condition.notify_all();
        }

        nextDataId = 0;
        nextOffset = 0;
        nextSize = 0;
        receiveSize = 0;
    }
}


BulkChannel::Buffer::Buffer()
  : p(nullptr)
{
}

BulkChannel::Buffer::Buffer(const uint8_t *data, size_t size, void(*released)(void *cookie), void *cookie)
  : p(new Private())
{
    p->refcount = 1;
    p->data = data;
    p->size = size;

    p->released = released;
    p->cookie = cookie;
}

BulkChannel::Buffer::Buffer(const Buffer &c) noexcept
  : p(c.p)
{
    if (p) p->refcount++;
}

BulkChannel::Buffer::~Buffer()
{
    deref();
    p = nullptr;
}

BulkChannel::Buffer & BulkChannel::Buffer::operator=(const Buffer &c)
{
    if (p != c.p)
    {
        deref();

        p = c.p;
        if (p) p->refcount++;
    }

    return *this;
}

const uint8_t * BulkChannel::Buffer::data() const noexcept
{
    return p ? p->data : nullptr;
}

size_t BulkChannel::Buffer::size() const noexcept
{
    return p ? p->size : 0;
}

void BulkChannel::Buffer::deref()
{
    if (p)
    {
        assert(p->refcount > 0);
        if (--(p->refcount) <= 0)
        {
            if (p->released)
                p->released(p->cookie);

            delete p;
            p = nullptr;
        }
    }
}

BulkChannel::Allocator::Allocator(BulkChannel &bulkChannel)
  : bulkChannel(bulkChannel)
{
    const auto bp = bulkChannel.p;
    bulkChannel.p->commThread->post([this, bp]
    {
        bp->allocators.push_back(this);
    });
}

BulkChannel::Allocator::~Allocator()
{
    const auto bp = bulkChannel.p;
    bulkChannel.p->commThread->send([this, bp]
    {
        for (auto i = bp->allocators.begin(); i != bp->allocators.end(); )
        {
            if (*i == this)
                i = bp->allocators.erase(i);
            else
                i++;
        }
    });
}

} // End of namespaces
