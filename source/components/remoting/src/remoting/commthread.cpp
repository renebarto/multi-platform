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
# pragma warning (disable : 4464) // Suppress warning including relative headers
#endif

#include "../../include/remoting/commthread.h"
#include "remoting1/system.h"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <stdexcept>

#if defined(__unix__)
# include <errno.h>
# include <signal.h>
# include <uuid/uuid.h>
# include <linux/limits.h>
#elif defined(__VXWORKS__)
# include <netinet/tcp.h>
# include <strings.h>
# include <selectLib.h>
# include <ioLib.h>
# include <stdio.h>
#elif defined(_WIN32)
# include <rpc.h>
#endif

#ifdef _MSC_VER
# pragma comment(lib, "ws2_32.lib") 
# pragma comment(lib, "mswsock.lib") 
# pragma comment(lib, "rpcrt4.lib") 
#endif

// define ANSI/ISO names
#ifdef _MSC_VER
# define snprintf _snprintf
#endif

#if defined(_MSC_VER)
# pragma warning (disable : 4127) // Conditional expression is constant; bug in FD_SET.

// Separate function because this uses an SEH exception.
static void setRemotingThreadName()
{
    const struct { DWORD type; LPCSTR name; DWORD threadId; DWORD flags; } info = { 0x1000, "REMOTING Communication thread", DWORD(-1), 0 };
    __try { ::RaiseException(0x406D1388, 0, sizeof(info) / sizeof(DWORD), (ULONG_PTR *)&info); } __except(EXCEPTION_CONTINUE_EXECUTION) { }
}
#endif

#if defined(_WIN32) || defined(__VXWORKS__)
// Wrapper functions to emulate Unix pipes with Winsock sockets.
static inline int read (Remoting1::FileDesc s,       char *buf, int len)	{ return ::recv(s, buf, len, 0); }
static inline int write(Remoting1::FileDesc s, const char *buf, int len)	{ return ::send(s, buf, len, 0); }
#endif

namespace Remoting1 {

int lastError()
{ 
#if defined(__unix__) || defined(__VXWORKS__)
    return errno; 
#elif defined(_WIN32)
    return ::WSAGetLastError(); 
#endif
}

bool errorIoPending() 
{ 
    const int err = lastError();
#if defined(__unix__) || defined(__VXWORKS__)
    return (err == EINPROGRESS) || (err == EWOULDBLOCK); 
#elif defined(_WIN32)
    return (err == WSAEINPROGRESS) || (err == WSAEWOULDBLOCK) || (err == WSA_IO_PENDING); 
#endif
}

std::shared_ptr<CommThread> CommThread::instance()
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

    static std::weak_ptr<CommThread> instance;
    if (auto ptr = instance.lock())
        return ptr;

    std::shared_ptr<CommThread> ptr(new CommThread(), [](CommThread *ptr)
    {
        std::lock_guard<decltype(mutex)> _(mutex);

        delete ptr;
    });

    instance = ptr;
    return ptr;
}

const char CommThread::serverId[] =
#if defined(__unix__)
        "Unix "
#elif defined(__VXWORKS__)
        "VxWorks "
#elif defined(_WIN32)
        "Windows "
#else
        "Unknown "
#endif
        "UPnP/1.1 REMOTING/1.0";

CommThread::HostId CommThread::generateHostId()
{
#if defined(__unix__)
    union { uuid_t u; uint8_t a[16]; } uuid;
    ::uuid_generate(uuid.u);
#elif defined(__VXWORKS__)
    union { uint8_t a[16]; uint32_t d[4]; } uuid;
    static uint32_t count = 0x1A84F29BLL;
    uuid.d[0] = uint32_t(getpid()) ^ 0x61BC73E0LL;
    uuid.d[1] = (uint32_t(rand()) << 16) | uint32_t(rand() & 0xFFFF);
    uuid.d[2] = (uint32_t(rand()) << 16) | uint32_t(rand() & 0xFFFF);
    uuid.d[3] = __sync_fetch_and_add(&count, 1);
    uuid.a[6] = (uuid.a[6] & 0x0F) | 0x40; // Version 4 (random)
    uuid.a[8] = (uuid.a[8] & 0x3F) | 0x10; // Variant
#elif defined(_WIN32)
    union { UUID u; uint8_t a[16]; uint32_t d[4]; } uuid;
    if (::UuidCreate(&uuid.u) != RPC_S_UUID_NO_ADDRESS)
    {
        uuid.u.Data1 = _byteswap_ulong (uuid.u.Data1);
        uuid.u.Data2 = _byteswap_ushort(uuid.u.Data2);
        uuid.u.Data3 = _byteswap_ushort(uuid.u.Data3);
    }
    else // No network adapter, use pid.
    {
        static volatile LONG lastId = 0x1A84F29B;
        uuid.d[0] = uint32_t(::GetCurrentProcessId()) ^ 0x61BC73E0;
        uuid.d[1] = (uint32_t(rand()) << 16) | uint32_t(rand() & 0xFFFF);
        uuid.d[2] = (uint32_t(rand()) << 16) | uint32_t(rand() & 0xFFFF);
        uuid.d[3] = static_cast<uint32_t>(InterlockedIncrement(&lastId));
        uuid.a[6] = static_cast<uint8_t>((uuid.a[6] & 0x0F) | 0x40); // Version 4 (random)
        uuid.a[8] = static_cast<uint8_t>((uuid.a[8] & 0x3F) | 0x10); // Variant
    }
#endif

    return CommThread::HostId(uuid.a);
}

CommThread::CommThread()
    : threadRunning(true),
      thread(nullptr)
{
    system().traceMsg("Starting communication thread");

    std::string moduleFile;

#if defined(__unix__)
    moduleFile.resize(PATH_MAX);
    const int r = ::readlink("/proc/self/exe", &(moduleFile[0]), moduleFile.size());
    if (r > 0)
    {
        moduleFile.resize(r);

        currentModuleName = moduleFile.substr(moduleFile.find_last_of('/') + 1);
    }

    currentModuleName = "Unknown";
#elif defined(__VXWORKS__)
    currentModuleName = "Unknown";
#elif defined(_WIN32)
    moduleFile.resize(MAX_PATH);
    moduleFile.resize(::GetModuleFileNameA(::GetModuleHandle(NULL), &(moduleFile[0]), DWORD(moduleFile.size())));

    const std::string::size_type start = moduleFile.find_last_of('\\') + 1;
    const std::string::size_type stop = moduleFile.find_last_of('.');
    currentModuleName = moduleFile.substr(start, (stop != std::string::npos) ? (stop - start) : std::string::npos);
#endif

    pipe[0] = pipe[1] = InvalidFileDesc;
    FD_ZERO(&readSet);
    FD_ZERO(&writeSet);
    FD_ZERO(&errorSet);

    // Create a pipe to handle custom events.
#if defined(__unix__)
    if (::pipe(pipe) == 0)
    {
        FD_SET(pipe[0], &readSet);
        readEvents[pipe[0]] = std::make_shared<std::function<void()>>(
            std::bind(&CommThread::handlePipeEvent, this));

        thread.reset(new std::thread(&CommThread::socketThreadFunc, this));
    }
#elif defined(__VXWORKS__)
    if (createPipe(pipe) == 0)
    {
        FD_SET(pipe[0], &readSet);
        readEvents[pipe[0]] = std::bind(&CommThread::handlePipeEvent, this);
        
        thread.reset(new std::thread(&CommThread::socketThreadFunc, this));
    }
#elif defined(_WIN32)
    struct T
    {
        static DWORD WINAPI threadFunc(LPVOID arg)
        {
#if defined(_MSC_VER)
            setRemotingThreadName();
#endif

            WSAData wsaData;
            ::WSAStartup(MAKEWORD(2, 2), &wsaData);

            T * t = reinterpret_cast<T *>(arg);
            CommThread * const volatile me = t->me;

            if (createPipe(me->pipe) == 0)
            {
                FD_SET(me->pipe[0], &me->readSet);
                me->readEvents[me->pipe[0]] = std::make_shared<std::function<void()>>(
                    std::bind(&CommThread::handlePipeEvent, me));

                ::SetEvent(t->event);
                t = nullptr; // t becomes invalid after the event has been set.

                me->socketThreadFunc();

                ::closesocket(me->pipe[0]);
                ::closesocket(me->pipe[1]);
            }

            system().traceMsg("Perform WSACleanup"); // Tracing added to find the root cause of a hangup during shutdown
            ::WSACleanup();
            system().traceMsg("Finished WSACleanup");

            return 0;
        }

        CommThread * me;
        HANDLE event;
    } t = { this, ::CreateEvent(NULL, FALSE, FALSE, NULL) };

    thread = ::CreateThread(NULL, 0, &T::threadFunc, &t, 0, &threadId);
    ::WaitForSingleObject(t.event, INFINITE);
    ::CloseHandle(t.event);

    ::SetThreadPriority(thread, THREAD_PRIORITY_BELOW_NORMAL);
#endif
}

CommThread::~CommThread()
{
    system().traceMsg("Stopping communication thread");

    send([this]
    {
        threadRunning = false;
    });

#if defined(__unix__)
    thread->join();

    ::close(pipe[0]);
    ::close(pipe[1]);
#elif defined(_WIN32)
    ::WaitForSingleObject(thread, INFINITE);
    ::CloseHandle(thread);
    thread = nullptr;
#endif
}

const std::string & CommThread::moduleName() const
{
    return currentModuleName;
}

bool CommThread::onCommThread() const
{
#if defined(__unix__)
    return thread && (thread->get_id() == std::this_thread::get_id());
#elif defined(_WIN32)
    return thread && (::GetCurrentThreadId() == threadId);
#endif
}

void CommThread::post(std::function<void()> &&func)
{
    std::lock_guard<std::mutex> _(mutex);

    pipeEventQueue.emplace(std::make_tuple(std::move(func), nullptr, nullptr));
    while (::write(pipe[1], "E", 1) == 0) 
        continue;
}

void CommThread::send(std::function<void()> &&func)
{
    if (!onCommThread())
    {
        std::unique_lock<std::mutex> l(mutex);

        std::condition_variable event;
        bool finished = false;

        pipeEventQueue.emplace(std::make_tuple(std::move(func), &finished, &event));
        while (::write(pipe[1], "E", 1) == 0) 
            continue;

        while (!finished)
            event.wait(l);
    }
    else if (func)
        func();
}

void CommThread::setReadEvent(FileDesc fd, std::function<void()> &&func)
{
    std::lock_guard<std::mutex> _(mutex);
    readEvents[fd] = std::make_shared<std::function<void()>>(std::move(func));
    FD_SET(fd, &readSet);

    if (readEvents.size() > FD_SETSIZE)
        system().fail("Number of readEvents exceeds maximum number of events");

    while (::write(pipe[1], "N", 1) == 0) 
        continue;
}

void CommThread::setWriteEvent(FileDesc fd, std::function<void()> &&func)
{
    std::lock_guard<std::mutex> _(mutex);

    writeEvents[fd] = std::move(func);
    FD_SET(fd, &writeSet);

    if (writeEvents.size() > FD_SETSIZE)
        system().fail("Number of writeEvents exceeds maximum number of events");

    while (::write(pipe[1], "N", 1) == 0) 
        continue;
}

void CommThread::setErrorEvent(FileDesc fd, std::function<void()> &&func)
{
    std::lock_guard<std::mutex> _(mutex);
    errorEvents[fd] = std::move(func);
    FD_SET(fd, &errorSet);

    if (errorEvents.size() > FD_SETSIZE)
        system().fail("Number of errorEvents exceeds maximum number of events");

    while (::write(pipe[1], "N", 1) == 0) 
        continue;
}

void CommThread::removeEvents(FileDesc fd)
{
    {
        std::lock_guard<std::mutex> _(mutex);

        auto read = readEvents.find(fd);
        if (read != readEvents.end())
        {
            readEvents.erase(read);
            FD_CLR(fd, &readSet);
        }

        auto write = writeEvents.find(fd);
        if (write != writeEvents.end())
        {
            writeEvents.erase(write);
            FD_CLR(fd, &writeSet);
        }

        auto error = errorEvents.find(fd);
        if (error != errorEvents.end())
        {
            errorEvents.erase(error);
            FD_CLR(fd, &errorSet);
        }
    }

    // This makes sure the events will no longer be invoked.
    send(std::function<void()>());
}

void CommThread::setTimerEvent(void *h, const std::chrono::milliseconds &timeout, std::function<void()> &&func)
{
    std::lock_guard<std::mutex> _(mutex);

#if !defined(_MSC_VER) || ((_MSC_VER < 1700) || (_MSC_VER >= 1900)) // Bug in VS2012 and VS2013 implementation of std::chrono::steady_clock.
    timerEventMap[h].emplace_back(std::make_pair(std::chrono::steady_clock::now() + timeout, std::move(func)));
#else
    timerEventMap[h].emplace_back(std::make_pair(::GetTickCount64() + timeout.count(), std::move(func)));
#endif
    while (::write(pipe[1], "N", 1) == 0) 
        continue;
}

void CommThread::removeEvents(void *h)
{
    {
        std::lock_guard<std::mutex> _(mutex);

        auto timer = timerEventMap.find(h);
        if (timer != timerEventMap.end())
            timerEventMap.erase(timer);
    }

    // This makes sure the events will no longer be invoked.
    send(std::function<void()>());
}

void CommThread::handlePipeEvent()
{
    char buffer[1024];
    const int size = ::read(pipe[0], buffer, int(sizeof(buffer)));

    for (int i=0; i<size; i++)
    switch (buffer[i])
    {
    case 'E': // Event
        {
            std::unique_lock<std::mutex> l(mutex);

            if (!pipeEventQueue.empty())
            {
                auto event = std::move(pipeEventQueue.front());
                pipeEventQueue.pop();

                l.unlock();
                if (std::get<0>(event)) std::get<0>(event)();
                l.lock();

                if (std::get<1>(event))
                    *std::get<1>(event) = true;

                if (std::get<2>(event))
                    std::get<2>(event)->notify_all();
            }
        }

        break;

    case 'N': // No-op
        break;
    }
}

void CommThread::socketThreadFunc()
{
    system().traceMsg("Started communication thread");

#if defined(__unix__)
    signal(SIGPIPE, SIG_IGN);
#endif

    while (threadRunning)
    {
        ::fd_set read, write, error;
        int nfds = 0;

#if !defined(_MSC_VER) || ((_MSC_VER < 1700) || (_MSC_VER >= 1900)) // Bug in VS2012 and VS2013 implementation of std::chrono::steady_clock.
        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
#else
        uint64_t now = ::GetTickCount64();
#endif
        std::chrono::milliseconds timeout(-1);
        {
            std::lock_guard<std::mutex> _(mutex);

            ::memcpy(&read , &readSet , sizeof(read));
            ::memcpy(&write, &writeSet, sizeof(write));
            ::memcpy(&error, &errorSet, sizeof(error));

#if defined(__unix__) // nfds is ignored on Windows.
            if (!readEvents .empty()) nfds = std::max(nfds, readEvents .rbegin()->first);
            if (!writeEvents.empty()) nfds = std::max(nfds, writeEvents.rbegin()->first);
            if (!errorEvents.empty()) nfds = std::max(nfds, errorEvents.rbegin()->first);
#endif

            for (const auto &i : timerEventMap)
            for (const auto &j : i.second)
            {
#if !defined(_MSC_VER) || ((_MSC_VER < 1700) || (_MSC_VER >= 1900)) // Bug in VS2012 and VS2013 implementation of std::chrono::steady_clock.
                const std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(j.first - now);
#else
                const std::chrono::milliseconds duration(j.first - now);
#endif
                if (timeout.count() >= 0)
                    timeout = std::max(std::min(timeout, duration), std::chrono::milliseconds(0));
                else
                    timeout = std::max(duration, std::chrono::milliseconds(0));
            }
        }

        ::timeval timeval;
        timeval.tv_sec = long(timeout.count() / 1000);
        timeval.tv_usec = long(timeout.count() % 1000) * 1000;

        // Wait for one of the file descriptors to be ready for read/write. 
        // When debugging you will end up here when the communication thread
        // is idle and functioning normally.
        if (::select(nfds + 1, &read, &write, &error, (timeout.count() >= 0) ? &timeval : nullptr) >= 0)
        {
            // Handle pending read events.
            for (FileDesc i = InvalidFileDesc;;)
            {
                std::shared_ptr<std::function<void()>> event;
                {
                    std::lock_guard<std::mutex> _(mutex);

                    for (auto j = (i != InvalidFileDesc)
                                    ? readEvents.upper_bound(i)
                                    : readEvents.begin();
                         (j != readEvents.end()) && !event;
                         j++)
                    {
                        if (FD_ISSET(j->first, &read))
                        {
                            event = j->second;
                            i = j->first;
                        }
                    }
                }

                if (event)
                    (*event)();
                else
                    break;
            }

            // Handle pending write events.
            for (FileDesc i = InvalidFileDesc;;)
            {
                std::function<void()> event;
                {
                    std::lock_guard<std::mutex> _(mutex);

                    for (auto j = (i != InvalidFileDesc) 
                                    ? writeEvents.upper_bound(i) 
                                    : writeEvents.begin();
                         (j != writeEvents.end()) && !event;)
                    {
                        if (FD_ISSET(j->first, &write))
                        {
                            event = std::move(j->second);
                            i = j->first;
                            FD_CLR(j->first, &writeSet);
                            j = writeEvents.erase(j);
                        }
                        else
                            j++;
                    }
                }

                if (event)
                    event();
                else
                    break;
            }

            // Handle pending error events.
            for (FileDesc i = InvalidFileDesc;;)
            {
                std::function<void()> event;
                {
                    std::lock_guard<std::mutex> _(mutex);

                    for (auto j = (i != InvalidFileDesc) 
                                    ? errorEvents.upper_bound(i) 
                                    : errorEvents.begin();
                         (j != errorEvents.end()) && !event;)
                    {
                        if (FD_ISSET(j->first, &error))
                        {
                            event = std::move(j->second);
                            i = j->first;
                            FD_CLR(j->first, &errorSet);
                            j = errorEvents.erase(j);
                        }
                        else
                            j++;
                    }
                }

                if (event)
                    event();
                else
                    break;
            }

            // Handle one timer event (to prevent starvation).
            std::function<void()> event;
            {
                std::lock_guard<std::mutex> _(mutex);

#if !defined(_MSC_VER) || ((_MSC_VER < 1700) || (_MSC_VER >= 1900)) // Bug in VS2012 and VS2013 implementation of std::chrono::steady_clock.
                now = std::chrono::steady_clock::now();
#else
                now = ::GetTickCount64();
#endif
                for (auto i = timerEventMap.begin(); (i != timerEventMap.end()) && !event; i++)
                for (auto j = i->second.begin(); (j != i->second.end()) && !event; )
                if (j->first <= now)
                {
                    event = std::move(j->second);
                    j = i->second.erase(j);
                }
                else
                    j++;
            }

            if (event)
                event();
        }
        else
            system().fail("select() failed (%d), this is usually the result of a network stack problem.", lastError());
    }

#if defined(__unix__)
    signal(SIGPIPE, SIG_DFL);
#endif

    system().traceMsg("Stopped communication thread");
}

#if defined(_WIN32) || defined(__VXWORKS__)
int CommThread::createPipe(FileDesc (& pipe)[2])
{
    int result = -1;

    sockaddr_in addr;
    ::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(0x7F000001);
    addr.sin_port = htons(0);

#if !defined(_MSC_VER) || !defined(WSA_FLAG_NO_HANDLE_INHERIT)
    FileDesc s = ::socket(AF_INET, SOCK_STREAM, 0);
#else
    FileDesc s = ::WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_NO_HANDLE_INHERIT);
    if (s == InvalidFileDesc)
        s = ::socket(AF_INET, SOCK_STREAM, 0);
#endif

    if (s != InvalidFileDesc)
    {
        if (::bind(s, (struct sockaddr *)&addr, sizeof(addr)) != -1)
        {
            socklen_t addrSize = sizeof(addr);

            if (::getsockname(s, (struct sockaddr *)&addr, &addrSize) != -1)
            if (::listen(s, 1) != -1)
            {
#if !defined(_MSC_VER) || !defined(WSA_FLAG_NO_HANDLE_INHERIT)
                pipe[0] = ::socket(AF_INET, SOCK_STREAM, 0);
#else
                pipe[0] = ::WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_NO_HANDLE_INHERIT);
                if (pipe[0] == InvalidFileDesc)
                    pipe[0] = ::socket(AF_INET, SOCK_STREAM, 0);
#endif

                if (pipe[0] != InvalidFileDesc)
                {
#if defined(__VXWORKS__)
                    int nonBlock = 1;
                    if (::ioctl(pipe[0], FIONBIO, (int)&nonBlock) == 0) &&
#else
                    u_long nonBlock = 1;
                    if ((::ioctlsocket(pipe[0], FIONBIO, &nonBlock) != -1) &&
#endif
                        (::connect(pipe[0], (struct sockaddr *)&addr, sizeof(addr)) == -1) &&
                        errorIoPending())
                    {
                        pipe[1] = ::accept(s, nullptr, nullptr);
                        if (pipe[1] != InvalidFileDesc)
                        {
                            const int enabled = 1;
                            if ((::setsockopt(
                                    pipe[0], 
                                    IPPROTO_TCP, TCP_NODELAY,
                                    (char *)&enabled, sizeof(enabled)) != -1) &&
                                (::setsockopt(
                                    pipe[1], 
                                    IPPROTO_TCP, TCP_NODELAY, 
                                    (char *)&enabled, sizeof(enabled)) != -1))
                            {
                                result = 0;
                            }
                            else
                            {
#if defined(__VXWORKS__)
                                ::close(pipe[1]);
#else
                                ::closesocket(pipe[1]);
#endif
                                pipe[1] = InvalidFileDesc;
                            }
                        }
                    }

                    if (result != 0)
                    {
#if defined(__VXWORKS__)
                        ::close(pipe[0]);
#else
                        ::closesocket(pipe[0]);
#endif
                        pipe[0] = InvalidFileDesc;
                    }
                }
            }
        }

#if defined(__VXWORKS__)
        ::close(s);
#else
        ::closesocket(s);
#endif
    }

    if (result != 0)
    {
        system().fail(
            "Failed to create pipe, this is usually the result of a network stack problem "
            "OR the binary is located on a network share or CC dynamic view.");
    }

    return result;
}
#endif

static const char hostIdFormat[] = "uuid:%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x";

CommThread::HostId::HostId()
{
    memset(this->data, 0, sizeof(this->data));
}

CommThread::HostId::HostId(const uint8_t(&data)[16])
{
    memcpy(this->data, data, sizeof(this->data));
}

static void parseHostId(uint8_t(&data)[16], const char *str)
{
    int val[16];
    const int r = std::sscanf(
        str, hostIdFormat,
        val +  0, val +  1, val +  2, val +  3, val +  4, val +  5, val +  6, val +  7,
        val +  8, val +  9, val + 10, val + 11, val + 12, val + 13, val + 14, val + 15);

    if (r != sizeof(data))
        memset(data, 0, sizeof(data));
    else for (int i = 0; (i < int(sizeof(data))) && (i < r); i++)
        data[i] = uint8_t(val[i]);
}

CommThread::HostId::HostId(const char *str)
{
    parseHostId(data, str);
}

CommThread::HostId::HostId(const std::string &str)
{
    parseHostId(data, str.c_str());
}

CommThread::HostId & CommThread::HostId::operator=(const HostId &from)
{
    memcpy(this->data, from.data, sizeof(this->data));

    return *this;
}

CommThread::HostId & CommThread::HostId::operator=(const uint8_t(&d)[16])
{
    memcpy(data, d, sizeof(data));

    return *this;
}

bool CommThread::HostId::operator<(const HostId &c) const
{
    return memcmp(this->data, c.data, sizeof(this->data)) < 0;
}

CommThread::HostId::operator std::string() const
{
    std::string result;
    result.resize(64);

    const int n = snprintf(
        &result[0], result.size(), hostIdFormat, 
        data[ 0], data[ 1], data[ 2], data[ 3], data[ 4], data[ 5], data[ 6], data[ 7],
        data[ 8], data[ 9], data[10], data[11], data[12], data[13], data[14], data[15]);

    assert(n == 41);
    if (n > 0)
        result.resize(std::min(size_t(n), result.size()));
    else
        result.clear();

    return result;
}

} // End of namespaces
