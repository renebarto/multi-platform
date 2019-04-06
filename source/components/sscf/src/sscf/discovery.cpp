/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifdef _MSC_VER
# pragma warning (disable : 4464) // Suppress warning including relative headers
#endif

#include "sscf1/discovery.h"
#include "sscf1/message.h"
#include "sscf1/server.h"
#include "sscf1/system.h"
#include "sscf1/udpsocket.h"
#include "../../include/sscf/commthread.h"

#include <cassert>
#include <cstring>
#include <iterator>
#include <locale>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#if defined(_MSC_VER)
# pragma warning (disable : 4355) // Using this in initializer list.
#endif

namespace Sscf1 {

struct Discovery::Private : UdpSocket::ReceiveCallback
{
    Private() 
      :	commThread(CommThread::instance()),
        minTimeout(2),
        maxTimeout(30)
    {
    }

    virtual ~Private() { }

    struct Interface
    { 
        Interface(IpAddress iface) : socket(std::move(iface)) { }
        virtual ~Interface() { }

        UdpSocket socket;
        IpAddress address, netmask;
    };

    struct ServerInterface : Interface, Server::Callback
    {
        explicit ServerInterface(Discovery::Private *p, IpAddress iface)
          : Interface(iface),
            p(p), 
            upnpServer(this, std::move(iface))
        { 
        }

        virtual ~ServerInterface() { }

        Discovery::Private * const p;
        Server upnpServer;

        virtual void handleConnect(const char *) override { }
        virtual void handleDisconnect(const char *) override { }
        virtual void handleRequest(const Message &request, const char *) override;
        virtual void handleResponse(const Message &) override { }
        virtual void handleNotification(const Message &) override { }
    };

    struct Service 
    { 
        std::string id;
        std::string name;
        std::string path;

        std::set<IpAddress> addresses;
        std::set<IpAddress> clients;
    };

    struct Search
    {
        Search(Private *p) : p(p), callback(nullptr), single(false) { }
        ~Search() { p->commThread->removeEvents(this); }

        Private *p;

        std::string name;
        IpAddress address;
        SearchCallback *callback;
        bool single;
    };

    static uint16_t ssdpPort;

    std::recursive_mutex mutex;

    const std::shared_ptr<CommThread> commThread;

    std::string rootDeviceId;

    std::map<IpAddress, std::unique_ptr<Interface>> clientInterfaces;
    std::map<IpAddress, std::unique_ptr<ServerInterface>> serverInterfaces;
    
    Service rootDevice;
    std::map<std::string, Service> publishedServices;

    const std::chrono::seconds minTimeout, maxTimeout;
    std::map<std::string, std::chrono::seconds> intervals;
    std::map<unsigned, std::unique_ptr<Search>> searches;

    void sendSearch(const char *name, const IpAddress &address);
    void sendPublish(const char *name, const IpAddress &address);

    void addClientInterface(const IpAddress &address, IpAddress netmask);
    void addServerInterface(const IpAddress &address, IpAddress netmask);
    void updateInterfaces(const IpAddress &address, void (Private::*)(const IpAddress &, IpAddress));

    void sendSearch(Interface &, const char *);
    void sendSearchResponse(Interface &, Service &, const IpAddress &);
    void sendAlive(Interface &, const Service &);
    void sendByeBye(Interface &, const Service &);

    void foundService(const Service &);

    virtual void receiveReceived(const char *buffer, int len, IpAddress address);
};

uint16_t Discovery::Private::ssdpPort = defaultSsdpPort;

void Discovery::setSsdpPort(uint16_t port)
{
    Private::ssdpPort = port;
}

IpAddress Discovery::ssdpAddress(const IpAddress &iface)
{
    switch (iface.family())
    {
    case IpAddress::Family::IPv4:
        return IpAddress(
            (Private::ssdpPort == defaultSsdpPort) ? "239.255.255.250" : "224.0.0.1",
            Private::ssdpPort);

    case IpAddress::Family::IPv6:
        return IpAddress(
            (Private::ssdpPort == defaultSsdpPort) ? "ff02::c" : "ff11::c",
            Private::ssdpPort);
    }

    return IpAddress();
}

std::shared_ptr<Discovery> Discovery::instance()
{
#if !defined(_MSC_VER) || (_MSC_VER >= 1900)
    static std::mutex mutex;
#else // Workaround for bug in Visual Studio versions before 2015.
    static struct Spinlock
    {
        void lock() { while (InterlockedCompareExchange(&value, 1, 0) != 0) Sleep(0); }
        void unlock() { InterlockedCompareExchange(&value, 0, 1); }
        volatile LONG value;
    } mutex = { 0 };
#endif

    std::lock_guard<decltype(mutex)> _(mutex);

    static std::weak_ptr<Discovery> instance;
    if (auto ptr = instance.lock())
        return ptr;

    std::shared_ptr<Discovery> ptr(new Discovery(), [](Discovery *ptr)
    {
        std::lock_guard<decltype(mutex)> _(mutex);

        delete ptr;
    });

    instance = ptr;
    return ptr;
}

Discovery::Discovery()
  : p(new Private())
{
    p->rootDeviceId = CommThread::generateHostId();

    p->rootDevice.name = "upnp:rootdevice";
    p->rootDevice.id = p->rootDeviceId + "::" + p->rootDevice.name;
    p->rootDevice.path = "/DeviceDescription";
}

Discovery::~Discovery()
{
    assert(!p->commThread->onCommThread());

    std::vector<std::unique_ptr<Private::Search>> searches;
    {
        std::lock_guard<std::recursive_mutex> _(p->mutex);

        for (auto &&i : p->searches)
            searches.emplace_back(std::move(i.second));

        p->searches.clear();
    }

    searches.clear();

    std::vector<std::unique_ptr<Private::Interface>> interfaces;
    {
        std::lock_guard<std::recursive_mutex> _(p->mutex);

        for (auto &&i : p->clientInterfaces)
            interfaces.emplace_back(std::move(i.second));

        p->clientInterfaces.clear();

        for (auto &&i : p->serverInterfaces)
        {
            p->sendByeBye(*i.second, p->rootDevice);

            interfaces.emplace_back(std::move(i.second));
        }

        p->serverInterfaces.clear();
    }

    // These have to be closed outside the mutex as they may trigger a
    // receiveReceived().
    for (const auto &i : interfaces)
        i->socket.close();

    interfaces.clear();

    delete p;
    *const_cast<Private **>(&p) = nullptr;
}

void Discovery::publishService(const char *name, const char *id, IpAddress address)
{
    std::ostringstream str;
    str.imbue(std::locale::classic());
    str << id << "::" << name;
    const std::string fullId = str.str();

    std::lock_guard<std::recursive_mutex> _(p->mutex);

    auto i = p->publishedServices.find(fullId);
    if (i == p->publishedServices.end())
    {
        Private::Service service;
        service.id = fullId;
        service.name = name;
        service.path = "/";
        service.addresses.emplace(address);

        p->publishedServices[service.id] = service;
    }
    else
        i->second.addresses.emplace(address);

    p->commThread->post([this, fullId, address]
    {
        p->sendPublish(fullId.c_str(), address); 
    });
}

void Discovery::unpublishService(const char *name, const char *id)
{
    std::ostringstream fullId;
    fullId.imbue(std::locale::classic());
    fullId << id << "::" << name;

    std::lock_guard<std::recursive_mutex> _(p->mutex);

    auto i = p->publishedServices.find(fullId.str());
    if (i != p->publishedServices.end())
    {
        for (const auto &j : p->serverInterfaces)
        {
            // Multicast byebye on all interfaces that can reach this service.
            for (auto k = i->second.addresses.begin();
                 k != i->second.addresses.end();
                 k++)
            {
                if (!*k || (j.second->address & j.second->netmask) == (*k & j.second->netmask))
                    p->sendByeBye(*j.second, i->second);
            }
        }

        p->publishedServices.erase(i);
    }
}

bool Discovery::searchService(SearchCallback *callback, const char *name, const char *network, int timeout, bool single)
{
    return searchService(callback, name, IpAddress(network), timeout, single);
}

bool Discovery::searchService(SearchCallback *callback, const char *name, IpAddress network, int timeout, bool single)
{
    std::lock_guard<std::recursive_mutex> _(p->mutex);

    std::unique_ptr<Private::Search> search(new Private::Search(p));
    search->name = name;
    search->address = std::move(network);
    search->callback = callback;
    search->single = single;

    std::chrono::seconds interval(1);
    auto i = p->intervals.find(name);
    if (i != p->intervals.end())
        interval = i->second;

    const auto searchId = 
        !p->searches.empty() 
        ? ((--(p->searches.end()))->first + 1)
        : 0;

    p->commThread->post([this, searchId]
    {
        std::lock_guard<std::recursive_mutex> _(p->mutex);

        auto i = p->searches.find(searchId);
        if (i != p->searches.end())
            p->sendSearch(i->second->name.c_str(), i->second->address);
    });

    const auto timeoutSeconds = (timeout < 0 ) ? (single ? p->maxTimeout : p->minTimeout) : std::chrono::seconds(timeout / 1000);
    for (; interval < timeoutSeconds; interval *= 2)
    {
        p->commThread->setTimerEvent(search.get(), interval, [this, searchId, interval]
        {
            std::lock_guard<std::recursive_mutex> _(p->mutex);

            auto i = p->searches.find(searchId);
            if (i != p->searches.end())
            {
                p->intervals[i->second->name] = interval;
                p->sendSearch(i->second->name.c_str(), i->second->address);
            }
        });
    }

    p->commThread->setTimerEvent(search.get(), timeoutSeconds, [this, searchId, timeoutSeconds]
    {
        std::unique_ptr<Private::Search> obsolete;
        {
            std::lock_guard<std::recursive_mutex> _(p->mutex);

            auto i = p->searches.find(searchId);
            if (i != p->searches.end())
            {
                system().traceMsg(
                    "Discovery: Timeout while searching for \"%s\"",
                    i->second->name.c_str());

                auto j = p->intervals.find(i->second->name);
                if (j != p->intervals.end())
                    j->second = std::max(j->second, timeoutSeconds);
                else
                    p->intervals.emplace(i->second->name, timeoutSeconds);

                obsolete = std::move(i->second); // invoke callback and delete search outside of the lock
                p->searches.erase(i);
            }
        }

        const IpAddress none;
        if (obsolete)
            obsolete->callback->searchFound(none);
    });

    p->searches.emplace(searchId, std::move(search));

    return true;
}

IpAddress Discovery::searchService(const char *name, const char *network, int timeout)
{
    return searchService(name, IpAddress(network), timeout);
}

IpAddress Discovery::searchService(const char *name, IpAddress network, int timeout)
{
#ifdef _MSC_VER
# pragma warning (disable : 4265) // Compiler bug
#endif
    struct T : SearchCallback
    {
        virtual void searchFound(const IpAddress &r)
        {
            result = r;

            std::lock_guard<std::mutex> _(*mutex);
            finished = true;
            event.notify_all();
        }

        std::mutex *mutex;
        bool finished;
        std::condition_variable event;
        IpAddress result;
    } t;

#ifdef _MSC_VER
# pragma warning (default : 4265)
#endif

    if (auto instance = Discovery::instance())
    {
        std::mutex mutex;
        std::unique_lock<std::mutex> l(mutex);

        t.mutex = &mutex;
        t.finished = false;
        instance->searchService(&t, name, std::move(network), timeout);

        while (!t.finished)
            t.event.wait(l);
    }

    return std::move(t.result);
}

std::vector<IpAddress> Discovery::searchServices(const char *name, const char *network)
{
    return searchServices(name, IpAddress(network));
}

std::vector<IpAddress> Discovery::searchServices(const char *name, IpAddress network)
{
#ifdef _MSC_VER
# pragma warning (disable : 4265) // Compiler bug
#endif
    struct T : SearchCallback
    {
        virtual void searchFound(const IpAddress &r)
        {
            if (r)
            {
                result.insert(r);
            }
            else
            {
                std::lock_guard<std::mutex> _(*mutex);
                finished = true;
                event.notify_all();
            }
        }

        std::mutex *mutex;
        bool finished;
        std::condition_variable event;
        std::set<IpAddress> result;
    } t;

#ifdef _MSC_VER
# pragma warning (default : 4265)
#endif

    if (auto instance = Discovery::instance())
    {
        std::mutex mutex;
        std::unique_lock<std::mutex> l(mutex);

        t.mutex = &mutex;
        t.finished = false;
        instance->searchService(&t, name, std::move(network), -1, false);

        while (!t.finished)
            t.event.wait(l);
    }

    std::vector<IpAddress> result;
    result.reserve(t.result.size());
    for (auto i = t.result.begin(); i != t.result.end(); i = t.result.erase(i))
        result.emplace_back(std::move(*i));

    return result;
}

void Discovery::stopSearches(SearchCallback *callback)
{
    std::vector<std::unique_ptr<Private::Search>> searches;
    {
        std::lock_guard<std::recursive_mutex> _(p->mutex);

        for (auto i = p->searches.begin(); i != p->searches.end(); )
        {
            if (i->second->callback == callback)
            {
                searches.push_back(std::move(i->second));
                i = p->searches.erase(i);
            }
            else
                i++;
        }
    }

    // cleanup outside of the lock to prevent deadlock
}

Discovery::SearchCallback::SearchCallback()
{
}

Discovery::SearchCallback::~SearchCallback()
{
}

void Discovery::Private::sendSearch(const char *name, const IpAddress &address)
{
    std::lock_guard<std::recursive_mutex> _(mutex);

    updateInterfaces(address, &Private::addServerInterface);
    updateInterfaces(address, &Private::addClientInterface);

    for (const auto &i : clientInterfaces)
    {
        if (!address || ((address & i.second->netmask) == (i.second->address & i.second->netmask)))
            sendSearch(*i.second, name);
    }
}

void Discovery::Private::sendPublish(const char *fullId, const IpAddress &address)
{
    std::lock_guard<std::recursive_mutex> _(mutex);

    updateInterfaces(address, &Private::addServerInterface);

    auto i = publishedServices.find(fullId);
    if (i != publishedServices.end())
    for (const auto &j : serverInterfaces)
    {
        if (!address || ((address & j.second->netmask) == (j.second->address & j.second->netmask)))
            sendAlive(*j.second, i->second);
    }
}

void Discovery::Private::addClientInterface(const IpAddress &address, IpAddress netmask)
{
    if (clientInterfaces.find(address) == clientInterfaces.end())
    {
        std::unique_ptr<Interface> iface(new Interface(address));
        iface->address = address;
        iface->netmask = std::move(netmask);
        iface->socket.bind(this, 0, true);

        clientInterfaces[address] = std::move(iface);
    }
}

void Discovery::Private::addServerInterface(const IpAddress &address, IpAddress netmask)
{
    if (serverInterfaces.find(address) == serverInterfaces.end())
    {
        const auto group = ssdpAddress(address);

        std::unique_ptr<ServerInterface> iface(new ServerInterface(this, address));
        iface->socket.enableMulticastLoopback();
        iface->socket.bind(this, group, group.port());
        iface->upnpServer.listen();
        
        iface->address = iface->upnpServer.address();
        iface->netmask = std::move(netmask);

        IpAddress serverAddress = address;
        serverAddress.setPort(iface->upnpServer.address().port());

        rootDevice.addresses.emplace(std::move(serverAddress));
        sendAlive(*iface, rootDevice);

        serverInterfaces[address] = std::move(iface);
    }
}

void Discovery::Private::updateInterfaces(const IpAddress &address, void (Private::* addInterface)(const IpAddress &, IpAddress))
{
    if (auto iface = IpAddress::findInterface(address))
        (this->*addInterface)(iface.address, std::move(iface.netmask));
}

void Discovery::Private::sendSearch(Interface &iface, const char *st)
{
    const auto address = ssdpAddress(iface.address);

    std::ostringstream host;
    host.imbue(std::locale::classic());
    host << address;

    std::ostringstream mx;
    mx.imbue(std::locale::classic());
    mx << (minTimeout.count() / 2);

    Message search("M-SEARCH", "*");
    search.setField("HOST", host.str().c_str());
    search.setField("MAN", "\"ssdp:discover\"");
    search.setField("MX", mx.str().c_str());
    search.setField("ST", st);

    iface.socket.beginSendTo(nullptr, address, search.header());

    system().traceMsg(
        "Discovery: Sent M-SEARCH for \"%s\" on %s to %s", 
        st,
        iface.socket.address().toString().c_str(),
        address.toString().c_str());
}

void Discovery::Private::sendSearchResponse(Interface &iface, Service &service, const IpAddress &address)
{
    for (const auto &j : service.addresses)
    {
        if ((j & iface.netmask) == (iface.address & iface.netmask))
        {
            std::ostringstream location;
            location.imbue(std::locale::classic());
            location << "http://" << j.toString(true, false) << service.path;

            Message response(200);
            response.setField("SERVER", CommThread::serverId);
            response.setField("USN", service.id.c_str());
            response.setField("CACHE-CONTROL", "no-cache");
            response.setField("LOCATION", location.str().c_str());
            response.setField("ST", service.name.c_str());

            iface.socket.beginSendTo(nullptr, address, response.header());
            service.clients.insert(address);

            system().traceMsg(
                "Discovery: Sent 200 OK for \"%s\" at \"%s\" on %s to %s", 
                service.name.c_str(),
                location.str().c_str(),
                iface.socket.address().toString().c_str(),
                address.toString().c_str());
        }
    }
}

void Discovery::Private::sendAlive(Interface &iface, const Service &service)
{
    const auto address = ssdpAddress(iface.address);

    std::ostringstream host;
    host.imbue(std::locale::classic());
    host << address;

    for (const auto &j : service.addresses)
    {
        if ((j & iface.netmask) == (iface.address & iface.netmask))
        {
            std::ostringstream location;
            location.imbue(std::locale::classic());
            location << "http://" << j.toString(true, false) << service.path;

            Message alive("NOTIFY", "*");
            alive.setField("SERVER", CommThread::serverId);
            alive.setField("HOST", host.str().c_str());
            alive.setField("USN", service.id.c_str());
            alive.setField("CACHE-CONTROL", "no-cache");
            alive.setField("LOCATION", location.str().c_str());
            alive.setField("NT", service.name.c_str());
            alive.setField("NTS", "ssdp:alive");

            iface.socket.beginSendTo(nullptr, address, alive.header());
 
            system().traceMsg(
                "Discovery: Sent NOTIFY ssdp:alive for \"%s\" at \"%s\" on %s to %s", 
                service.name.c_str(),
                location.str().c_str(),
                iface.socket.address().toString().c_str(),
                address.toString().c_str());
       }
    }
}

void Discovery::Private::sendByeBye(Interface &iface, const Service &service)
{
    const auto address = ssdpAddress(iface.address);

    std::ostringstream host;
    host.imbue(std::locale::classic());
    host << address;

    Message byebye("NOTIFY", "*");
    byebye.setField("SERVER", CommThread::serverId);
    byebye.setField("HOST", host.str().c_str());
    byebye.setField("USN", service.id.c_str());
    byebye.setField("NT", service.name.c_str());
    byebye.setField("NTS", "ssdp:byebye");

    iface.socket.beginSendTo(nullptr, address, byebye.header());
 
    system().traceMsg(
        "Discovery: Sent NOTIFY ssdp:byebye for \"%s\" on %s", 
        service.name.c_str(),
        iface.socket.address().toString().c_str());
}

void Discovery::Private::foundService(const Service &service)
{
    for (auto i = searches.begin(); i != searches.end(); )
    {
        bool found = false;

        if (i->second->name == service.name)
        for (const auto &j : service.addresses)
        {
            // Loop through the network interfaces to see if the result is in the same network as the request
            for (auto k = clientInterfaces.begin(); (k != clientInterfaces.end()) && !found; k++)
            {
                if (!i->second->address ||
                    (((j & k->second->netmask) == (k->second->address & k->second->netmask)) &&
                     ((i->second->address & k->second->netmask) == (k->second->address & k->second->netmask))))
                {
                    found = true;
                }
            }

            if (found)
            {
                if (i->second->callback)
                    i->second->callback->searchFound(j);

                break;
            }
        }

        if (found && i->second->single)
            i = searches.erase(i);
        else
            i++;
    }
}

void Discovery::Private::receiveReceived(const char *buffer, int len, IpAddress address)
{
    const Message message(buffer, len);

    std::lock_guard<std::recursive_mutex> _(mutex);

    // Find out on which interface this message was received.
    Interface * clientInterface = nullptr;
    for (auto i = clientInterfaces.begin();
         (i != clientInterfaces.end()) && (clientInterface == nullptr);
         i++)
    {
        if ((address & i->second->netmask) == (i->second->address & i->second->netmask))
            clientInterface = i->second.get();
    }

    ServerInterface * serverInterface = nullptr;
    for (auto i = serverInterfaces.begin();
         (i != serverInterfaces.end()) && (serverInterface == nullptr);
         i++)
    {
        if ((address & i->second->netmask) == (i->second->address & i->second->netmask))
            serverInterface = i->second.get();
    }

    // Handle the message.
    if (message.responseStatus() == 200)
    {
        const char * const usn      = message.field("USN");
        const char * const location = message.field("LOCATION");
        const char * const st       = message.field("ST");
        if (usn && location && st && clientInterface)
        {
            system().traceMsg(
                "Discovery: Received 200 OK for \"%s\" at \"%s\" from %s", 
                st,
                location,
                address.toString().c_str());

            intervals.erase(st);

            char host[256];
            uint16_t port = 0;
            if (Message::parseUrl(location, host, port))
            {
                if (auto result = IpAddress::resolve(host, port, address.scopeId()))
                {
                    Service service;
                    service.id = usn;
                    service.name = st;
                    service.addresses.emplace(std::move(result));
                    foundService(service);
                }
            }
        }
    }
    else if (::strcmp(message.requestMethod(), "M-SEARCH") == 0)
    {
        const char * const mx = message.field("MX");
        const char * const st = message.field("ST");
        if (mx && st && serverInterface)
        {
            const bool all = ::strcmp(st, "ssdp:all") == 0;
            if (all || (rootDevice.name == st))
                sendSearchResponse(*serverInterface, rootDevice, address);

            for (auto &i : publishedServices)
            {
                if (all || (i.second.name == st))
                    sendSearchResponse(*serverInterface, i.second, address);
            }
        }
    }
    else if (::strcmp(message.requestMethod(), "NOTIFY") == 0)
    {
        const char * const nts = message.field("NTS");
        if (nts)
        {
            if (::strcmp(nts, "ssdp:alive") == 0)
            {
                const char * const usn      = message.field("USN");
                const char * const location = message.field("LOCATION");
                const char * const nt       = message.field("NT");
                if (usn && location && nt && clientInterface)
                {
                    intervals.erase(nt);

                    char host[256];
                    uint16_t port = 0;
                    if (Message::parseUrl(location, host, port))
                    {
                        if (auto result = IpAddress::resolve(host, port, address.scopeId()))
                        {
                            Service service;
                            service.id = usn;
                            service.name = nt;
                            service.addresses.emplace(std::move(result));
                            foundService(service);
                        }
                    }
                }
            }
        }
    }
}

void Discovery::Private::ServerInterface::handleRequest(const Message &request, const char *hostId)
{
    if (::strcmp(request.requestPath(), "/DeviceDescription") == 0)
    {
        IpAddress urlBase = address;
        urlBase.setPort(upnpServer.address().port());

        std::ostringstream deviceDescription;
        deviceDescription.imbue(std::locale::classic());
        deviceDescription <<
            "<?xml version=\"1.0\"?>\n"
            "<root xmlns=\"urn:schemas-upnp-org:device-1-0\">\n"
            " <specVersion><major>1</major><minor>1</minor></specVersion>\n"
            " <URLBase>http://" << urlBase.toString(true, false) << "/</URLBase>\n"
            " <device>\n"
            "  <deviceType>urn:schemas-upnp-org:device:Basic:1</deviceType>\n"
            "  <friendlyName>" << p->commThread->moduleName() << "</friendlyName>\n"
            "  <manufacturer>Philips</manufacturer>\n"
            "  <manufacturerURL>http://www.philips.com/</manufacturerURL>\n"
            "  <modelName>" << p->commThread->moduleName() << "</modelName>\n"
            "  <UDN>" << p->rootDeviceId << "</UDN>\n"
            "  <serviceList>\n";

        for (const auto &i : p->publishedServices)
        {
            for (const auto &j : i.second.addresses)
            {
                if (!j || (j == address))
                {
                    deviceDescription <<
                        "   <service>\n"
                        "    <serviceId>" << i.second.id << "</serviceId>\n"
                        "    <serviceType>" << i.second.name << "</serviceType>\n"
                        "    <SCPDURL>/ServiceDescription?id=" << i.second.id << "</SCPDURL>\n"
                        "    <SSCFURL>http://" << urlBase.toString(true, false) << i.second.path << "</SSCFURL>\n"
                        "    <controlURL>/</controlURL>\n"
                        "    <eventSubURL>/</eventSubURL>\n"
                        "   </service>\n";
                }
            }
        }

        deviceDescription <<
            "  </serviceList>\n"
            " </device>\n"
            "</root>\n";

        Message response(200);
        response.setField("Server", CommThread::serverId);
        response.setField("Content-Type", "text/xml");
        const std::string content = deviceDescription.str();
        std::copy(content.begin(), content.end(), std::back_inserter(response.content()));
        upnpServer.sendResponse(response, hostId);
    }
    else if (::strncmp(request.requestPath(), "/ServiceDescription?", 20) == 0)
    {
        std::ostringstream serviceDescription;
        serviceDescription.imbue(std::locale::classic());
        serviceDescription <<
            "<?xml version=\"1.0\"?>\n"
            "<scpd xmlns=\"urn:schemas-upnp-org:service-1-0\">\n"
            " <specVersion><major>1</major><minor>1</minor></specVersion>\n"
            "</scpd>\n";

        Message response(200);
        response.setField("Server", CommThread::serverId);
        response.setField("Content-Type", "text/xml");
        const std::string content = serviceDescription.str();
        std::copy(content.begin(), content.end(), std::back_inserter(response.content()));
        upnpServer.sendResponse(response, hostId);
    }
    else
    {
        Message response(404);
        response.setField("Server", CommThread::serverId);
        upnpServer.sendResponse(response, hostId);
    }
}

} // End of namespaces
