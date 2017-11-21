#include "network/Socket.h"

#include <poll.h>
#include "core/DefaultLogger.h"
#include "core/Core.h"
#include "core/Util.h"

using namespace std;
using namespace Network;

const uint32_t Socket::TimeWait = 10;

static size_t BufferSize = 4096;

Socket::Socket()
    : _socketHandle(OSAL::Network::InvalidHandleValue)
{
}

Socket::Socket(Socket const & other)
    : _socketHandle(other._socketHandle)
{
}

Socket::Socket(Socket && other)
    : _socketHandle(other._socketHandle)
{
    other._socketHandle = OSAL::Network::InvalidHandleValue;
}

Socket::~Socket()
{
    Close();
}

Socket & Socket::operator = (Socket const & other)
{
    if (this != & other)
    {
        _socketHandle = other._socketHandle;
    }
    return *this;
}

Socket & Socket::operator = (Socket && other)
{
    if (this != & other)
    {
        _socketHandle = other._socketHandle;
        other._socketHandle = OSAL::Network::InvalidHandleValue;
    }
    return *this;
}

OSAL::Network::SocketHandle Socket::GetHandle() const
{
    return _socketHandle;
}

void Socket::SetHandle(OSAL::Network::SocketHandle handle)
{
    _socketHandle = handle;
}

void Socket::Open(OSAL::Network::SocketFamily socketFamily, OSAL::Network::SocketType socketType)
{
    Close();
    Lock lock(_mutex);
    int result = OSAL::Network::CreateSocket(socketFamily, socketType);
    if (result != -1)
        SetHandle(result);
    if (result == -1)
    {
        int errorCode = errno;

        basic_ostringstream<OSAL::Char> stream;
        stream << _("socket() failed, errorcode ") << dec << errorCode << _(" (") << hex << setw(2) << setfill(_('0')) << errorCode << _("): ") << strerror(errorCode);
        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
    }
}

void Socket::Close()
{
    Lock lock(_mutex);
    int result = 0;
    if (GetHandle() != OSAL::Network::InvalidHandleValue)
    {
        result = OSAL::Network::CloseSocket(GetHandle());
        SetHandle(OSAL::Network::InvalidHandleValue);
    }
    if (result == -1)
    {
        int errorCode = errno;

        basic_ostringstream<OSAL::Char> stream;
        stream << _("close() failed, errorcode ") << dec << errorCode << _(" (") << hex << setw(2) << setfill(_('0')) << errorCode << _("): ") << strerror(errorCode);
        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
    }
}

bool Socket::IsClosed()
{
    return (GetHandle() == OSAL::Network::InvalidHandleValue);
}

void Socket::SetSocketOption(OSAL::Network::SocketOptionLevel level,
                             OSAL::Network::SocketOption socketOption,
                             void * optionValue, unsigned int optionLength)
{
    int result = OSAL::Network::SetSocketOption(this->GetHandle(), level, socketOption, optionValue, optionLength);
    if (result == -1)
    {
        int errorCode = errno;

        basic_ostringstream<OSAL::Char> stream;
        stream << _("setsockopt() failed, errorcode ") << dec << errorCode << _(" (") << hex << setw(2) << setfill(_('0')) << errorCode << _("): ") << strerror(errorCode);
        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
    }
}

void Socket::GetSocketOption(OSAL::Network::SocketOptionLevel level, OSAL::Network::SocketOption socketOption, void * optionValue, unsigned int & optionLength)
{
    int result = OSAL::Network::GetSocketOption(this->GetHandle(), level, socketOption, optionValue, optionLength);
    if (result == -1)
    {
        int errorCode = errno;

        basic_ostringstream<OSAL::Char> stream;
        stream << _("getsockopt() failed, errorcode ") << dec << errorCode << _(" (") << hex << setw(2) << setfill(_('0')) << errorCode << _("): ") << strerror(errorCode);
        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
    }
}

void Socket::SetSocketOption(OSAL::Network::SocketOption socketOption, void * optionValue, unsigned int optionLength)
{
    SetSocketOption(OSAL::Network::SocketOptionLevel::Socket, socketOption, optionValue, optionLength);
}

void Socket::GetSocketOption(OSAL::Network::SocketOption socketOption, void * optionValue, unsigned int & optionLength)
{
    GetSocketOption(OSAL::Network::SocketOptionLevel::Socket, socketOption, optionValue, optionLength);
}

void Socket::SetSocketOptionBool(OSAL::Network::SocketOption socketOption, bool value)
{
    int optionValue = value ? 1 : 0;
    SetSocketOption(socketOption, &optionValue, sizeof(optionValue));
}

bool Socket::GetSocketOptionBool(OSAL::Network::SocketOption socketOption)
{
    int optionValue;
    socklen_t optionLength = sizeof(optionValue);
    GetSocketOption(socketOption, &optionValue, optionLength);
    return optionValue != 0;
}

void Socket::SetSocketOptionInt(OSAL::Network::SocketOption socketOption, int optionValue)
{
    SetSocketOption(OSAL::Network::SocketOptionLevel::Socket, socketOption, &optionValue, sizeof(optionValue));
}

int Socket::GetSocketOptionInt(OSAL::Network::SocketOption socketOption)
{
    int optionValue;
    socklen_t optionLength = sizeof(optionValue);
    GetSocketOption(OSAL::Network::SocketOptionLevel::Socket, socketOption, &optionValue, optionLength);
    return optionValue;
}

bool Socket::GetBroadcastOption()
{
    return GetSocketOptionBool(OSAL::Network::SocketOption::Broadcast);
}

void Socket::SetBroadcastOption(bool value)
{
    SetSocketOptionBool(OSAL::Network::SocketOption::Broadcast, value);
}

bool Socket::GetReuseAddress()
{
    return GetSocketOptionBool(OSAL::Network::SocketOption::ReuseAddress);
}

void Socket::SetReuseAddress(bool value)
{
    SetSocketOptionBool(OSAL::Network::SocketOption::ReuseAddress, value);
}

int Socket::GetReceiveTimeout()
{
    timeval tv;
    bzero(&tv, sizeof(tv));
    socklen_t optionLength = sizeof(tv);
    GetSocketOption(OSAL::Network::SocketOption::ReceiveTimeout, &tv, optionLength);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void Socket::SetReceiveTimeout(int timeoutMS)
{
    timeval tv;
    bzero(&tv, sizeof(tv));
    tv.tv_sec = timeoutMS / 1000;
    tv.tv_usec = (timeoutMS % 1000) * 1000;
    SetSocketOption(OSAL::Network::SocketOption::ReceiveTimeout, &tv, sizeof(tv));
}

int Socket::GetSendTimeout()
{
    timeval tv;
    bzero(&tv, sizeof(tv));
    socklen_t optionLength = sizeof(tv);
    GetSocketOption(OSAL::Network::SocketOption::SendTimeout, &tv, optionLength);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void Socket::SetSendTimeout(int timeoutMS)
{
    timeval tv;
    bzero(&tv, sizeof(tv));
    tv.tv_sec = timeoutMS / 1000;
    tv.tv_usec = (timeoutMS % 1000) * 1000;
    SetSocketOption(OSAL::Network::SocketOption::SendTimeout, &tv, sizeof(tv));
}

bool Socket::GetBlockingMode()
{
    int flags = OSAL::Network::Fcntl(this->GetHandle(), F_GETFL);
    if (flags == -1)
    {
        int errorCode = errno;

        basic_ostringstream<OSAL::Char> stream;
        stream << _("fcntl() failed, errorcode ") << dec << errorCode << _(" (") << hex << setw(2) << setfill(_('0')) << errorCode << _("): ") << strerror(errorCode);
        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
    }
    return (flags & O_NONBLOCK) == 0;
}

void Socket::SetBlockingMode(bool value)
{
    int flags = OSAL::Network::Fcntl(this->GetHandle(), F_GETFL);
    if (flags == -1)
    {
        int errorCode = errno;

        basic_ostringstream<OSAL::Char> stream;
        stream << _("fcntl() failed, errorcode ") << dec << errorCode << _(" (") << hex << setw(2) << setfill(_('0')) << errorCode << _("): ") << strerror(errorCode);
        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
    }
    int errorCode = OSAL::Network::Fcntl(this->GetHandle(), F_SETFL, value ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK));
    if (errorCode == -1)
    {
        int errorCode = errno;

        basic_ostringstream<OSAL::Char> stream;
        stream << _("fcntl() failed, errorcode ") << dec << errorCode << _(" (") << hex << setw(2) << setfill(_('0')) << errorCode << _("): ") << strerror(errorCode);
        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
    }
}

void Socket::Bind(OSAL::Network::AddressPtr address)
{

    int errorCode = OSAL::Network::Bind(this->GetHandle(), address);
    if (errorCode == -1)
    {
        int errorCode = errno;

        basic_ostringstream<OSAL::Char> stream;
        stream << _("bind() failed, errorcode ") << dec << errorCode << _(" (") << hex << setw(2) << setfill(_('0')) << errorCode << _("): ") << strerror(errorCode);
        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
    }
}

bool Socket::Connect(OSAL::Network::AddressPtr serverAddress, OSAL::Network::SocketTimeout timeout)
{
    if (timeout != OSAL::Network::InfiniteTimeout)
    {
        SetBlockingMode(false);
    }
    else
    {
        SetBlockingMode(true);
    }

    int result = OSAL::Network::Connect(GetHandle(), serverAddress);
    if (result == -1)
    {
        int errorCode = errno;

        basic_ostringstream<OSAL::Char> stream;
        stream << _("connect() failed, errorcode ") << dec << errorCode << _(" (") << hex << setw(2) << setfill(_('0')) << errorCode << _("): ") << strerror(errorCode);
        Core::TheLogger().Debug(OSAL::OS::TypeName(*this), stream.str());

        if ((errorCode == EINPROGRESS) || (errorCode == EALREADY))
        {
            pollfd descriptor;
            descriptor.fd = GetHandle();
            descriptor.events = POLLIN | POLLOUT;
            descriptor.revents = 0;
            int pollResult = ::poll(&descriptor, 1, timeout);
            if (pollResult == -1)
            {
                errorCode = errno;

                basic_ostringstream<OSAL::Char> stream;
                stream << _("poll() failed, errorcode ") << dec << errorCode << _(" (") << hex << setw(2) << setfill(_('0')) << errorCode << _("): ") << strerror(errorCode);
                Core::TheLogger().Debug(OSAL::OS::TypeName(*this), stream.str());
            }
            else if (pollResult == 0)
            {
                return false;
            }
            else
            {
                if ((descriptor.revents & POLLHUP))
                    return false;
                result = 0;
            }
        }
        else if ((errorCode != EWOULDBLOCK) && (errorCode != EAGAIN))
            OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
    }

    SetBlockingMode(true);
    return (result != -1);
}

void Socket::Listen(int numListeners)
{
    int result = ::listen(GetHandle(), numListeners);
    if (result == -1)
    {
        int errorCode = errno;

        basic_ostringstream<OSAL::Char> stream;
        stream << _("listen() failed, errorcode ") << dec << errorCode << _(" (") << hex << setw(2) << setfill(_('0')) << errorCode << _("): ") << strerror(errorCode);
        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
    }
}

bool Socket::Accept(Socket & connectionSocket, OSAL::Network::AddressPtr & clientAddress, OSAL::Network::SocketTimeout timeout)
{
    Lock lock(_mutex);
    if (timeout != OSAL::Network::InfiniteTimeout)
    {
        SetBlockingMode(false);
    }
    else
    {
        SetBlockingMode(true);
    }

    uint32_t waitTime = 0;
    if (timeout != OSAL::Network::InfiniteTimeout)
    {
        waitTime = timeout;
    }

    int result = 0;
    do
    {
        result = OSAL::Network::Accept(GetHandle(), _socketFamily, clientAddress);
        if (result == -1)
        {
            int errorCode = errno;

            basic_ostringstream<OSAL::Char> stream;
            stream << _("accept() failed, errorcode ") << dec << errorCode << _(" (") << hex << setw(2) << setfill(_('0')) << errorCode << _("): ") << strerror(errorCode);
            Core::TheLogger().Debug(OSAL::OS::TypeName(*this), stream.str());

            if (((errorCode == EWOULDBLOCK) || (errorCode == EAGAIN)) && (waitTime > 0))
            {
                Core::Util::Sleep(TimeWait);
                waitTime -= min(waitTime, TimeWait);
            }
            else if (errorCode == EBADF)
            {
                return false;
            }
            else
                OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
        }
    }
    while ((result == -1) && (waitTime > 0));

    SetBlockingMode(true);
    if (result != -1)
    {
        connectionSocket.SetHandle(result);
    }
    else
    {
        connectionSocket.Close();
    }
    return (result != -1);
}

void Socket::GetLocalAddress(OSAL::Network::AddressPtr & address)
{
    int errorCode = OSAL::Network::GetSockName(GetHandle(), _socketFamily, address);
    if (errorCode == -1)
    {
        int errorCode = errno;

        basic_ostringstream<OSAL::Char> stream;
        stream << _("getsockname() failed, errorcode ") << dec << errorCode << _(" (") << hex << setw(2) << setfill(_('0')) << errorCode << _("): ") << strerror(errorCode);
        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
    }
}

void Socket::GetRemoteAddress(OSAL::Network::AddressPtr & address)
{
    int errorCode = OSAL::Network::GetPeerName(GetHandle(), _socketFamily, address);
    if (errorCode == -1)
    {
        int errorCode = errno;

        basic_ostringstream<OSAL::Char> stream;
        stream << _("getpeername() failed, errorcode ") << dec << errorCode << _(" (") << hex << setw(2) << setfill(_('0')) << errorCode << _("): ") << strerror(errorCode);
        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
    }
}

OSAL::ByteArray Socket::Receive(int flags)
{
    uint8_t buffer[BufferSize];
    OSAL::ByteArray result;

    size_t numBytes = 0;
    size_t offset = 0;

    do
    {
        numBytes = Receive(buffer, BufferSize, flags);

        result.Set(offset, buffer, numBytes);
        offset += numBytes;
    }
    while (numBytes > 0);
    return result;
}

size_t Socket::Receive(uint8_t * data, size_t bufferSize, int flags)
{
    size_t numBytes = 0;
    try
    {
        ssize_t result = 0;

        result = OSAL::Network::Receive(GetHandle(), data, bufferSize, flags);
        if (result == -1)
        {
            int errorCode = errno;

#ifdef TRACE
            basic_ostringstream<OSAL::Char> stream;
            stream << _("recv() failed, errorcode ") << dec << errorCode << _(" (") << hex << setw(2) << setfill(_('0')) << errorCode << _("): ") << strerror(errorCode);
            Core::TheLogger().Debug(OSAL::OS::TypeName(*this), stream.str());
#endif
            if ((errorCode != EINTR) && (errorCode != EWOULDBLOCK) && (errorCode != EAGAIN))
                OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
        }
        else if (result == 0)
        {
            Close();
        }
        else
        {
            numBytes = result;
        }
    }
    catch (const OSAL::SystemError & e)
    {
        if (e.GetErrorCode() == EBADF)
        {
            numBytes = 0;
        }
    }
    return numBytes;
}

bool Socket::Send(OSAL::ByteArray & data, size_t bytesToSend, int flags)
{
    assert(bytesToSend <= data.Size());
    return Send(data.Data(), bytesToSend, flags);
}

bool Socket::Send(const uint8_t * data, size_t bytesToSend, int flags)
{
    ssize_t numBytesLeftToSend = bytesToSend;
    size_t offset = 0;

    while (numBytesLeftToSend > 0)
    {
        ssize_t numBytes = OSAL::Network::Send(GetHandle(), data + offset, numBytesLeftToSend, flags);
        if (numBytes == -1)
        {
            int errorCode = errno;

            basic_ostringstream<OSAL::Char> stream;
            stream << _("send() failed, errorcode ") << dec << errorCode << _(" (") << hex << setw(2) << setfill(_('0')) << errorCode << _("): ") << strerror(errorCode);
            Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

            if ((errorCode == EPIPE) || (errorCode == ECONNRESET))
            {
                return false;
            }
            OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
        }
        else
        {
            offset += numBytes;
            numBytesLeftToSend -= numBytes;
        }
    }
    return true;
}

void Socket::SendTo(const OSAL::Network::AddressPtr & address, const uint8_t * data, size_t bytesToSend)
{
    int errorCode = OSAL::Network::SendTo(GetHandle(), data, bytesToSend, 0, address);
    if (errorCode == -1)
    {
        int errorCode = errno;

        basic_ostringstream<OSAL::Char> stream;
        stream << _("sendto() failed, errorcode ") << dec << errorCode 
               << _(" (") << hex << setw(2) << setfill(_('0')) << errorCode << _("): ") << strerror(errorCode);
        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
    }
}

size_t Socket::ReceiveFrom(OSAL::Network::AddressPtr & address, uint8_t * data, size_t bufferSize)
{
    ssize_t numBytes = OSAL::Network::ReceiveFrom(GetHandle(), data, bufferSize, 0, _socketFamily, address);
    if (numBytes == -1)
    {
        int errorCode = errno;

#ifdef TRACE
        basic_ostringstream<OSAL::Char> stream;
        stream << _("recvfrom() failed, errorcode ") << dec << errorCode 
               << _(" (") << hex << setw(2) << setfill(_('0')) << errorCode << _("): ") << strerror(errorCode);
        Core::TheLogger().Debug(OSAL::OS::TypeName(*this), stream.str());
#endif

        if ((errorCode != EINTR) && (errorCode != EWOULDBLOCK) && (errorCode != EAGAIN))
            OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
        numBytes = 0;
    }

    return numBytes;
}

std::ostream & Socket::PrintTo(std::ostream & stream) const
{
    stream << OSAL::OS::TypeName(*this) << _(" handle = ") << GetHandle();
    return stream;
}
