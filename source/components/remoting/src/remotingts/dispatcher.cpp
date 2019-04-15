/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#define REMOTINGTS1_DISPATCHER_CPP
#include "remotingts1/dispatcher.h"
#include "remotingts1/eventloop.h"
#include "remoting1/message.h"
#include "remoting1/system.h"

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <locale>
#include <map>
#include <sstream>
#include <string>

#ifdef _MSC_VER
# pragma warning (disable : 4265) // Suppress warning from Microsoft headers
#endif
#include <atomic>
#include <condition_variable>
#include <thread>
#include <mutex>
#ifdef _MSC_VER
# pragma warning (default : 4265)
#endif

namespace RemotingTs1 {

using namespace Remoting1;

CommunicationException::CommunicationException(const std::string &text) throw()
    : std::exception(),
      text(text)
{
    system().traceMsg("CommunicationException: %s", text.c_str());
}

CommunicationException::~CommunicationException() throw()
{
}

const char * CommunicationException::what() const throw()
{
    return text.c_str();
}

struct Dispatcher::Private
{
    typedef std::map<std::thread::id, std::list<std::function<void()>> *> TaskList;
    typedef std::map<void *, UnregisterFunc> Objects;
    typedef std::map<std::string, Factory> Interfaces;
    typedef std::map<std::string, Instance> Instances;
    typedef std::map<std::string, std::set<std::string>> HostInstances;
    struct ProxyFuncTuple { ProxyFunc proxyFunc; ProxyFuncPtr fptr; void *obj; };
    typedef std::vector<ProxyFuncTuple> ProxyFuncList;
    typedef std::map<std::string, ProxyFuncList> ProxyFuncs;

    static std::atomic<int> taskListSpin;
    static volatile int taskListRefCount;
    static std::recursive_mutex * volatile taskListMutex;
    static TaskList * volatile taskList;

    static void acquireTaskList();
    static void releaseTaskList();
    static void removeTasks();
    static void waitForResponseSync(bool &, std::mutex &, std::condition_variable &);
    static void waitForResponseAsync(bool &, std::mutex &, std::condition_variable &);

    std::recursive_mutex mutex;
    Objects objects;
    std::set<std::string> registredInstances;
    Interfaces interfaces;	// keeps track of the registered interfaces (singleton/multiton)
    Instances instances;	// Keeps track of all instances that have been created and the factory used to create the instance
    HostInstances hostInstances; // Keeps track of the multiton instances for a particular client 
    ProxyFuncs proxyFuncs;	// maps the method name as a string (incl. parameters) to a delegate the invokes the method 
    uint64_t lastInstanceId;		// counter used to number of multion instances
    std::string callingHostId;
};

void (* volatile Dispatcher::waitForResponse)(bool &, std::mutex &, std::condition_variable &) = &Dispatcher::Private::waitForResponseSync;
const std::string Dispatcher::empty;
const std::string Dispatcher::singleton_instance = ".";

Dispatcher::Dispatcher(void)
  : p(new Private())
{
    p->lastInstanceId = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count());

    registerProxy(nullptr, "remoting", "/providesType", &Dispatcher::providesType, this);
    registerProxy(nullptr, "remoting", "/createInstance", &Dispatcher::createInstance, this);
    registerProxy(nullptr, "remoting", "/destroyInstance", &Dispatcher::destroyInstance, this);
}

Dispatcher::~Dispatcher()
{
    {
        std::lock_guard<std::recursive_mutex> _(p->mutex);

        for (const auto &i : p->interfaces)
        if ((i.second.factory != nullptr) && (i.second.factory->dispatcher == this))
            i.second.factory->dispatcher = nullptr;

        for (const auto &i : p->objects)
            (this->*(i.second))(i.first);
    }

    delete p;
    *const_cast<Private **>(&p) = nullptr;
}

void Dispatcher::handleDisconnect(const std::string &hostId)
{
    std::lock_guard<std::recursive_mutex> _(p->mutex);

    if (hostId.empty())
        destroyInstances();
    else
        destroyInstances(hostId);
}


std::vector<std::string> Dispatcher::hostIdsForInstance(
    const std::string &instance, const char *name)
{
    std::lock_guard<std::recursive_mutex> _(p->mutex);

    std::vector<std::string> result;

    auto i = p->instances.find(instance);
    if (i != p->instances.end())
    {
        auto j = i->second.hostIds.find(name);
        if (j != i->second.hostIds.end())
            std::copy(j->second.begin(), j->second.end(), std::back_inserter(result));
    }

    return result;
}

void Dispatcher::destroyInstances()
{
    std::lock_guard<std::recursive_mutex> _(p->mutex);
    // Destroy all instances in case server disconnects from client.
    std::vector<std::string> instances;
    for (const auto &i : p->instances)
        instances.push_back(i.first);

    p->hostInstances.clear();

    p->callingHostId.clear();
    for (const auto &i : instances)
        destroyInstance(i);
}

void Dispatcher::destroyInstances(const std::string &hostId)
{
    std::lock_guard<std::recursive_mutex> _(p->mutex);
    // Destroy all instances from the disconnected client.
    auto i = p->hostInstances.find(hostId);
    if (i != p->hostInstances.end())
    {
        std::vector<std::string> instances;
        for (const auto &j : i->second)
            instances.push_back(j);

        p->callingHostId = hostId;

        for (const auto &j : instances)
            destroyInstance(j);

        p->callingHostId.clear();
    }
}

void Dispatcher::handleMessage(const Message &in, Message &out)
{
    std::unique_lock<std::recursive_mutex> l(p->mutex);

    auto i = p->proxyFuncs.find(in.requestPath());
    if (i != p->proxyFuncs.end())
    {
        const char * const hostId = in.field("X-HostId");
        if (hostId)
        {
            out.setResponseStatus(200);

            p->callingHostId = hostId;

            Remoting1::BulkChannel &bulkChannel = this->bulkChannel();
            const Private::ProxyFuncList proxyFuncs = i->second;

            l.unlock();

            for (const auto &j : proxyFuncs)
                j.proxyFunc(bulkChannel, in, out, j.fptr, j.obj);

            l.lock();

            p->callingHostId.clear();
        }
        else
            out.setResponseStatus(400);
    }
    else
        out.setResponseStatus(404);
}

void Dispatcher::registerInterface(
    const char *name, 
    const std::string &instance, 
    IMultitonFactory<void> *factory, 
    CreateFunc createFunc, 
    DestructFunc destructFunc)
{
    std::lock_guard<std::recursive_mutex> _(p->mutex);

    p->registredInstances.insert(std::string(name) + "::" + instance);
    p->interfaces[name] = Factory(factory, createFunc, destructFunc);
}

void Dispatcher::unregisterInterface(const std::string &instance, const char *name)
{
    std::lock_guard<std::recursive_mutex> _(p->mutex);

    p->registredInstances.erase(std::string(name) + "::" + instance);

    if (instance == ".")
    {
        auto i = p->interfaces.find(name);
        if (i != p->interfaces.end())
            p->interfaces.erase(i);
    }
}

void Dispatcher::registerObject(void *object, UnregisterFunc unregisterFunc)
{
    std::lock_guard<std::recursive_mutex> _(p->mutex);
    p->objects[object] = unregisterFunc;
}

void Dispatcher::unregisterObject(void *object)
{
    std::lock_guard<std::recursive_mutex> _(p->mutex);
    auto i = p->objects.find(object);
    if (i != p->objects.end())
        p->objects.erase(i);
}

Dispatcher::ProxyId Dispatcher::registerProxy(
    const std::string *clientHostId, const std::string &instance, const char *path, 
    ProxyFunc proxyFunc, ProxyFuncPtr fptr, void *obj)
{
    std::lock_guard<std::recursive_mutex> _(p->mutex);

    std::ostringstream fullPath;
    fullPath.imbue(std::locale::classic());
    if (clientHostId && !clientHostId->empty())
        fullPath << "/" << *clientHostId << "/" << instance << path;
    else
        fullPath << "/" << instance << path;

    Private::ProxyFuncTuple tuple = { proxyFunc, fptr, obj };
    auto i = p->proxyFuncs.find(fullPath.str());
    if (i != p->proxyFuncs.end())
        i->second.push_back(tuple);
    else
        p->proxyFuncs[fullPath.str()].push_back(tuple);

    return fptr;
}

void Dispatcher::unregisterProxies(const std::string *clientHostId, const std::string &instance, const std::vector<ProxyId> &proxies)
{
    std::lock_guard<std::recursive_mutex> _(p->mutex);

    std::ostringstream fullPath;
    fullPath.imbue(std::locale::classic());
    if (clientHostId && !clientHostId->empty())
        fullPath << "/" << *clientHostId << "/" << instance;
    else
        fullPath << "/" << instance;

    for (auto i = p->proxyFuncs.begin(); i != p->proxyFuncs.end(); )
    if (i->first.substr(0, fullPath.str().length()) == fullPath.str())
    {
        for (auto j = i->second.begin(); j != i->second.end(); )
        if (std::find(proxies.begin(), proxies.end(), j->fptr) != proxies.end())
            j = i->second.erase(j);
        else
            j++;

        if (i->second.empty())
            i = p->proxyFuncs.erase(i);
        else
            i++;
    }
    else
        i++;
}

bool Dispatcher::providesType(const std::string &name)
{
    std::lock_guard<std::recursive_mutex> _(p->mutex);

    return p->interfaces.find(name) != p->interfaces.end();
}

std::string Dispatcher::createInstance(const std::string &name, const std::string &instance)
{
    std::lock_guard<std::recursive_mutex> _(p->mutex);

    auto i = p->interfaces.find(name);
    if (i != p->interfaces.end())
    {
        if ((i->second.factory != nullptr) && (i->second.createFunc != nullptr))
        {
            std::ostringstream str;
            str.imbue(std::locale::classic());
            str << name << "_" << std::hex << std::setfill('0') << std::setw(16) << ++(p->lastInstanceId);

            // Create and register the actual object.
            const std::string inst = str.str();
            void *object = nullptr;
            if ((this->*(i->second.createFunc))(i->second.factory, inst.c_str(), object))
            {
                p->instances[inst] = Instance(i->second, name, p->callingHostId, object);
                p->hostInstances[p->callingHostId].insert(inst);

                return inst;
            }
        }
        else if (!instance.empty()) // Specific instance
        {
            if (p->registredInstances.find(name + "::" + instance) != p->registredInstances.end())
            {
                p->instances[instance].hostIds[name].insert(p->callingHostId);

                return instance;
            }
        }
        else // Singleton
        {
            if (p->registredInstances.find(name + "::.") != p->registredInstances.end())
            {
                p->instances["."].hostIds[name].insert(p->callingHostId);

                return ".";
            }
        }

        system().traceMsg("Dispatcher: Failed to find instance for %s :: %s", name.c_str(), instance.c_str());
    }
    else
        system().traceMsg("Dispatcher: Failed to find interface for %s :: %s", name.c_str(), instance.c_str());

    return empty;
}

void Dispatcher::destroyInstance(const std::string &name)
{
    std::lock_guard<std::recursive_mutex> _(p->mutex);

    auto i = p->instances.find(name);
    if (i != p->instances.end())
    {
        if (!p->callingHostId.empty())
        {
            for (auto j = i->second.hostIds.begin(); j != i->second.hostIds.end();)
            {
                j->second.erase(p->callingHostId);
                if (j->second.empty())
                    j = i->second.hostIds.erase(j);
                else
                    j++;
            }
        }
        else
            i->second.hostIds.clear();

        if (i->second.hostIds.empty())
        {
            if ((i->second.factory != nullptr) && (i->second.destructFunc != nullptr))
                (this->*(i->second.destructFunc))(i->second.factory, i->second.object);

            p->instances.erase(i);
        }

        auto j = p->hostInstances.find(p->callingHostId);
        if (j != p->hostInstances.end())
        {
            auto k = j->second.find(name);
            if (k != j->second.end())
                j->second.erase(k);

            if (j->second.empty())
                p->hostInstances.erase(j);
        }
    }
}

EventBase::Subscription Dispatcher::subscribeNotificationDelegate(Event<void()> &event, const std::string &instance, const char *name, const char *path)
{
    return event.subscribe([this, instance, name, path]
    {
        Message in;

        sendNotification(
            hostIdsForInstance(instance, name),
            std::vector<Remoting1::BulkChannel::BulkId>(),
            instance,
            path,
            in);
    });
}

void Dispatcher::invokeAsync(const std::string &hostId, const std::string &instance, const char *path)
{
    Message in;

    sendRequestAsync(hostId, instance, path, in);
}

void Dispatcher::parallelInvoke(std::function<void()> task1, std::function<void()> task2)
{
    std::list<std::function<void()>> tasks;
    tasks.emplace_back(std::move(task1));
    tasks.emplace_back(std::move(task2));

    execAsync(tasks);
}

std::atomic<int>                            Dispatcher::Private::taskListSpin;
volatile int                                Dispatcher::Private::taskListRefCount = 0;
std::recursive_mutex             * volatile Dispatcher::Private::taskListMutex = nullptr;
Dispatcher::Private::TaskList    * volatile Dispatcher::Private::taskList = nullptr;

void Dispatcher::execAsync(std::list<std::function<void()>> &newTasks)
{
    Private::acquireTaskList();

    std::list<std::function<void()>> *useTasks = nullptr;
    {
        std::lock_guard<std::recursive_mutex> _(*Private::taskListMutex);

        auto tasks = Private::taskList->find(std::this_thread::get_id());
        if ((tasks != Private::taskList->end()) && (tasks->second != nullptr) && !tasks->second->empty())
        {
            for (const auto &i : newTasks)
                tasks->second->push_back(i);

            useTasks = tasks->second;
        }
        else
            (*Private::taskList)[std::this_thread::get_id()] = useTasks = &newTasks;
    }

    try
    {
        while (!useTasks->empty())
        {
            auto task = std::move(useTasks->front());
            useTasks->pop_front();
            task();
        }
    }
    catch (...)
    {
        Private::removeTasks();
        Private::releaseTaskList();

        throw;
    }

    Private::removeTasks();
    Private::releaseTaskList();
}

void Dispatcher::Private::acquireTaskList()
{
    int val;
    while (!taskListSpin.compare_exchange_strong(val = 0, 1))
        std::this_thread::yield();

    if ((taskListMutex == nullptr) && (taskList == nullptr))
    {
        taskListMutex = new std::recursive_mutex();
        taskList = new TaskList();
    }

    taskListRefCount++;

    taskListSpin = 0;
}

void Dispatcher::Private::releaseTaskList()
{
    int val;
    while (!taskListSpin.compare_exchange_strong(val = 0, 1))
        std::this_thread::yield();

    if (--taskListRefCount == 0)
    {
        delete taskListMutex;
        taskListMutex = nullptr;
        delete taskList;
        taskList = nullptr;
    }

    taskListSpin = 0;
}

void Dispatcher::Private::removeTasks()
{
    std::lock_guard<std::recursive_mutex> _(*Private::taskListMutex);

    auto tasks = Private::taskList->find(std::this_thread::get_id());
    if (tasks != Private::taskList->end())
        Private::taskList->erase(tasks);
}

void Dispatcher::Private::waitForResponseSync(bool &finished, std::mutex &mutex, std::condition_variable &event)
{
    std::unique_lock<std::mutex> l(mutex);
    while (!finished)
        event.wait(l);
}

void Dispatcher::Private::waitForResponseAsync(bool &finished, std::mutex &mutex, std::condition_variable &event)
{
    std::unique_lock<std::mutex> l(mutex);
    while (!finished)
    {
        l.unlock();

        acquireTaskList();

        std::unique_lock<std::recursive_mutex> tl(*Private::taskListMutex);

        auto tasks = Private::taskList->find(std::this_thread::get_id());
        if ((tasks != Private::taskList->end()) && (tasks->second != nullptr) && !tasks->second->empty())
        {
            auto task = std::move(tasks->second->front());
            tasks->second->pop_front();
            tl.unlock();
            releaseTaskList();

            task();
        }
        else
        {
            tl.unlock();
            releaseTaskList();

            l.lock();
            while (!finished)
                event.wait(l);

            return;
        }

        l.lock();
    }
}

} // End of namespaces
