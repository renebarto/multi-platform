#include "network/Socket.h"

#include <fcntl.h>
#include <poll.h>
#include <ifaddrs.h>
#include <cstring>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include "core/DefaultLogger.h"
#include "core/Core.h"
#include "core/Util.h"

using namespace std;
using namespace Network;

const uint32_t Socket::TimeWait = 10;

static size_t BufferSize = 4096;

Socket::Socket()
    : socketHandle(OSAL::Network::InvalidHandleValue)
{
}

Socket::Socket(Socket const & other)
    : socketHandle(other.socketHandle)
{
}

Socket::Socket(Socket && other)
    : socketHandle(other.socketHandle)
{
    other.socketHandle = OSAL::Network::InvalidHandleValue;
}

Socket::~Socket()
{
    Close();
}

Socket & Socket::operator = (Socket const & other)
{
    if (this != & other)
    {
        socketHandle = other.socketHandle;
    }
    return *this;
}

Socket & Socket::operator = (Socket && other)
{
    if (this != & other)
    {
        socketHandle = other.socketHandle;
        other.socketHandle = OSAL::Network::InvalidHandleValue;
    }
    return *this;
}

OSAL::Network::SocketHandle Socket::GetHandle() const
{
    return socketHandle;
}

void Socket::SetHandle(OSAL::Network::SocketHandle handle)
{
    socketHandle = handle;
}

void Socket::Open(SocketFamily socketFamily, SocketType socketType)
{
    Close();
    Lock lock(mutex);
    int result = socket((int)socketFamily, (int)socketType, 0);
    if (result != -1)
        socketHandle = result;
    if (result == -1)
    {
        int errorCode = errno;

        ostringstream stream;
        stream << "socket() failed, errorcode " << dec << errorCode << " (" << hex << setw(2) << setfill('0') << errorCode << "): " << strerror(errorCode);
        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        Core::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
    }
}

void Socket::Close()
{
    Lock lock(mutex);
    int result = 0;
    if (socketHandle != OSAL::Network::InvalidHandleValue)
    {
        result = close(socketHandle);
        socketHandle = OSAL::Network::InvalidHandleValue;
    }
    if (result == -1)
    {
        int errorCode = errno;

        ostringstream stream;
        stream << "close() failed, errorcode " << dec << errorCode << " (" << hex << setw(2) << setfill('0') << errorCode << "): " << strerror(errorCode);
        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        Core::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
    }
}

bool Socket::IsClosed()
{
    return (socketHandle == OSAL::Network::InvalidHandleValue);
}

void Socket::SetSocketOption(SocketOptionLevel level, SocketOption socketOption, void * optionValue, int optionLength)
{
    int result = setsockopt(this->GetHandle(), (int)level, (int)socketOption, optionValue, optionLength);
    if (result == -1)
    {
        int errorCode = errno;

        ostringstream stream;
        stream << "setsockopt() failed, errorcode " << dec << errorCode << " (" << hex << setw(2) << setfill('0') << errorCode << "): " << strerror(errorCode);
        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        Core::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
    }
}

void Socket::GetSocketOption(SocketOptionLevel level, SocketOption socketOption, void * optionValue, unsigned int * optionLength)
{
    int result = getsockopt(this->GetHandle(), (int)level, (int)socketOption, optionValue, optionLength);
    if (result == -1)
    {
        int errorCode = errno;

        ostringstream stream;
        stream << "getsockopt() failed, errorcode " << dec << errorCode << " (" << hex << setw(2) << setfill('0') << errorCode << "): " << strerror(errorCode);
        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        Core::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
    }
}

void Socket::SetSocketOption(SocketOption socketOption, void * optionValue, unsigned int optionLength)
{
    SetSocketOption(SocketOptionLevel::Socket, socketOption, optionValue, optionLength);
}

void Socket::GetSocketOption(SocketOption socketOption, void * optionValue, unsigned int * optionLength)
{
    GetSocketOption(SocketOptionLevel::Socket, socketOption, optionValue, optionLength);
}

void Socket::SetSocketOptionBool(SocketOption socketOption, bool value)
{
    int optionValue = value ? 1 : 0;
    SetSocketOption(socketOption, &optionValue, sizeof(optionValue));
}

bool Socket::GetSocketOptionBool(SocketOption socketOption)
{
    int optionValue;
    socklen_t optionLength = sizeof(optionValue);
    GetSocketOption(socketOption, &optionValue, &optionLength);
    return optionValue != 0;
}

void Socket::SetSocketOptionInt(SocketOption socketOption, int optionValue)
{
    SetSocketOption(SocketOptionLevel::Socket, socketOption, &optionValue, sizeof(optionValue));
}

int Socket::GetSocketOptionInt(SocketOption socketOption)
{
    int optionValue;
    socklen_t optionLength = sizeof(optionValue);
    GetSocketOption(SocketOptionLevel::Socket, socketOption, &optionValue, &optionLength);
    return optionValue;
}

bool Socket::GetBroadcastOption()
{
    return GetSocketOptionBool(SocketOption::Broadcast);
}

void Socket::SetBroadcastOption(bool value)
{
    SetSocketOptionBool(SocketOption::Broadcast, value);
}

bool Socket::GetReuseAddress()
{
    return GetSocketOptionBool(SocketOption::ReuseAddress);
}

void Socket::SetReuseAddress(bool value)
{
    SetSocketOptionBool(SocketOption::ReuseAddress, value);
}

int Socket::GetReceiveTimeout()
{
    timeval tv;
    bzero(&tv, sizeof(tv));
    socklen_t optionLength = sizeof(tv);
    GetSocketOption(SocketOption::ReceiveTimeout, &tv, &optionLength);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void Socket::SetReceiveTimeout(int timeoutMS)
{
    timeval tv;
    bzero(&tv, sizeof(tv));
    tv.tv_sec = timeoutMS / 1000;
    tv.tv_usec = (timeoutMS % 1000) * 1000;
    SetSocketOption(SocketOption::ReceiveTimeout, &tv, sizeof(tv));
}

int Socket::GetSendTimeout()
{
    timeval tv;
    bzero(&tv, sizeof(tv));
    socklen_t optionLength = sizeof(tv);
    GetSocketOption(SocketOption::SendTimeout, &tv, &optionLength);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void Socket::SetSendTimeout(int timeoutMS)
{
    timeval tv;
    bzero(&tv, sizeof(tv));
    tv.tv_sec = timeoutMS / 1000;
    tv.tv_usec = (timeoutMS % 1000) * 1000;
    SetSocketOption(SocketOption::SendTimeout, &tv, sizeof(tv));
}

bool Socket::GetBlockingMode()
{
    int flags = fcntl(this->GetHandle(), F_GETFL);
    if (flags == -1)
    {
        int errorCode = errno;

        ostringstream stream;
        stream << "fcntl() failed, errorcode " << dec << errorCode << " (" << hex << setw(2) << setfill('0') << errorCode << "): " << strerror(errorCode);
        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        Core::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
    }
    return (flags & O_NONBLOCK) == 0;
}

void Socket::SetBlockingMode(bool value)
{
    int flags = fcntl(this->GetHandle(), F_GETFL);
    if (flags == -1)
    {
        int errorCode = errno;

        ostringstream stream;
        stream << "fcntl() failed, errorcode " << dec << errorCode << " (" << hex << setw(2) << setfill('0') << errorCode << "): " << strerror(errorCode);
        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        Core::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
    }
    int errorCode = fcntl(this->GetHandle(), F_SETFL, value ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK));
    if (errorCode == -1)
    {
        int errorCode = errno;

        ostringstream stream;
        stream << "fcntl() failed, errorcode " << dec << errorCode << " (" << hex << setw(2) << setfill('0') << errorCode << "): " << strerror(errorCode);
        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        Core::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
    }
}

void Socket::Bind(const sockaddr * address, socklen_t addressLength)
{

    int errorCode = ::bind(this->GetHandle(), address, addressLength);
    if (errorCode == -1)
    {
        int errorCode = errno;

        ostringstream stream;
        stream << "bind() failed, errorcode " << dec << errorCode << " (" << hex << setw(2) << setfill('0') << errorCode << "): " << strerror(errorCode);
        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        Core::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
    }
}

bool Socket::Connect(sockaddr const * serverAddress, socklen_t serverAddressLength, OSAL::Network::SocketTimeout timeout)
{
    if (timeout != OSAL::Network::InfiniteTimeout)
    {
        SetBlockingMode(false);
    }
    else
    {
        SetBlockingMode(true);
    }

    int result = ::connect(GetHandle(), serverAddress, serverAddressLength);
    if (result == -1)
    {
        int errorCode = errno;

        ostringstream stream;
        stream << "connect() failed, errorcode " << dec << errorCode << " (" << hex << setw(2) << setfill('0') << errorCode << "): " << strerror(errorCode);
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

                ostringstream stream;
                stream << "poll() failed, errorcode " << dec << errorCode << " (" << hex << setw(2) << setfill('0') << errorCode << "): " << strerror(errorCode);
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
            Core::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
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

        ostringstream stream;
        stream << "listen() failed, errorcode " << dec << errorCode << " (" << hex << setw(2) << setfill('0') << errorCode << "): " << strerror(errorCode);
        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        Core::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
    }
}

bool Socket::Accept(Socket & connectionSocket, sockaddr * clientAddress, socklen_t * clientAddressLength, OSAL::Network::SocketTimeout timeout)
{
    Lock lock(mutex);
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
        result = ::accept(GetHandle(), clientAddress, clientAddressLength);
        if (result == -1)
        {
            int errorCode = errno;

            ostringstream stream;
            stream << "accept() failed, errorcode " << dec << errorCode << " (" << hex << setw(2) << setfill('0') << errorCode << "): " << strerror(errorCode);
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
                Core::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
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

void Socket::GetLocalAddress(sockaddr * address, socklen_t * addressLength)
{
    int errorCode = ::getsockname(this->GetHandle(), (sockaddr *)address, addressLength);
    if (errorCode == -1)
    {
        int errorCode = errno;

        ostringstream stream;
        stream << "getsockname() failed, errorcode " << dec << errorCode << " (" << hex << setw(2) << setfill('0') << errorCode << "): " << strerror(errorCode);
        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        Core::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
    }
}

void Socket::GetRemoteAddress(sockaddr * address, socklen_t * addressLength)
{
    int errorCode = ::getpeername(this->GetHandle(), address, addressLength);
    if (errorCode == -1)
    {
        int errorCode = errno;

        ostringstream stream;
        stream << "getpeername() failed, errorcode " << dec << errorCode << " (" << hex << setw(2) << setfill('0') << errorCode << "): " << strerror(errorCode);
        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        Core::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
    }
}

Core::ByteArray Socket::Receive(int flags)
{
    uint8_t buffer[BufferSize];
    Core::ByteArray result;

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

        result = ::recv(socketHandle, data, bufferSize, flags);
        if (result == -1)
        {
            int errorCode = errno;

#ifdef TRACE
            ostringstream stream;
            stream << "recv() failed, errorcode " << dec << errorCode << " (" << hex << setw(2) << setfill('0') << errorCode << "): " << strerror(errorCode);
            Core::TheLogger().Debug(OSAL::OS::TypeName(*this), stream.str());
#endif
            if ((errorCode != EINTR) && (errorCode != EWOULDBLOCK) && (errorCode != EAGAIN))
                Core::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
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
    catch (const Core::SystemError & e)
    {
        if (e.GetErrorCode() == EBADF)
        {
            numBytes = 0;
        }
    }
    return numBytes;
}

bool Socket::Send(Core::ByteArray & data, size_t bytesToSend, int flags)
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
        ssize_t numBytes = ::send(socketHandle, data + offset, numBytesLeftToSend, flags);
        if (numBytes == -1)
        {
            int errorCode = errno;

            ostringstream stream;
            stream << "send() failed, errorcode " << dec << errorCode << " (" << hex << setw(2) << setfill('0') << errorCode << "): " << strerror(errorCode);
            Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

            if ((errorCode == EPIPE) || (errorCode == ECONNRESET))
            {
                return false;
            }
            Core::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
        }
        else
        {
            offset += numBytes;
            numBytesLeftToSend -= numBytes;
        }
    }
    return true;
}

void Socket::SendTo(sockaddr * address, socklen_t addressLength, const uint8_t * data, size_t bytesToSend)
{
    int errorCode = ::sendto(socketHandle, data, bytesToSend, 0, address, addressLength);
    if (errorCode == -1)
    {
        int errorCode = errno;

        ostringstream stream;
        stream << "sendto() failed, errorcode " << dec << errorCode << " (" << hex << setw(2) << setfill('0') << errorCode << "): " << strerror(errorCode);
        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        Core::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
    }
}

size_t Socket::ReceiveFrom(sockaddr * address, socklen_t * addressLength, uint8_t * data, size_t bufferSize)
{
    ssize_t numBytes = ::recvfrom(socketHandle, data, bufferSize, 0, address, addressLength);
    if (numBytes == -1)
    {
        int errorCode = errno;

#ifdef TRACE
        ostringstream stream;
        stream << "recvfrom() failed, errorcode " << dec << errorCode << " (" << hex << setw(2) << setfill('0') << errorCode << "): " << strerror(errorCode);
        Core::TheLogger().Debug(OSAL::OS::TypeName(*this), stream.str());
#endif

        if ((errorCode != EINTR) && (errorCode != EWOULDBLOCK) && (errorCode != EAGAIN))
            Core::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
        numBytes = 0;
    }

    return numBytes;
}

string Socket::ToString() const
{
    ostringstream stream;
    stream << OSAL::OS::TypeName(*this) << " handle = " << socketHandle;
    return stream.str();
}
