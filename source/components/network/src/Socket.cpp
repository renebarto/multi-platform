#include "network/Socket.h"

//#include <poll.h>
#include <thread>
#include <osal/Exception.h>
//#include "core/DefaultLogger.h"
//#include "core/Core.h"
//#include "core/Util.h"

using namespace std;

namespace Network {

const OSAL::Network::SocketTimeout socket::TimeWait = 10;

static constexpr size_t BufferSize = 4096;
using ssize_t = int64_t;

socket::socket()
    : _socketFamily(OSAL::Network::SocketFamily::Any)
    , _socketHandle(OSAL::Network::InvalidHandleValue)
    , _isBlocking(true)
{
}

socket::socket(socket const &other)
    : _socketFamily(other._socketFamily)
    , _socketHandle(other._socketHandle)
    , _isBlocking(other._isBlocking)
{
}

socket::socket(socket &&other)
    : _socketFamily(other._socketFamily)
    , _socketHandle(other._socketHandle)
    , _isBlocking(other._isBlocking)
{
    other._socketFamily = OSAL::Network::SocketFamily::Any;
    other._socketHandle = OSAL::Network::InvalidHandleValue;
}

socket::~socket()
{
    Close();
}

socket &socket::operator=(socket const &other)
{
    if (this != &other)
    {
        _socketFamily = other._socketFamily;
        _socketHandle = other._socketHandle;
        _isBlocking = other._isBlocking;
    }
    return *this;
}

socket &socket::operator=(socket &&other)
{
    if (this != &other) {
        _socketFamily = other._socketFamily;
        _socketHandle = other._socketHandle;
        other._socketFamily = OSAL::Network::SocketFamily::Any;
        other._socketHandle = OSAL::Network::InvalidHandleValue;
    }
    return *this;
}

OSAL::Network::SocketHandle socket::GetHandle() const
{
    return _socketHandle;
}

void socket::SetHandle(OSAL::Network::SocketHandle handle)
{
    _socketHandle = handle;
}

void socket::Open(OSAL::Network::SocketFamily socketFamily, OSAL::Network::SocketType socketType)
{
    Close();
    OSAL::Network::SocketHandle handle {};
    int result = OSAL::Network::CreateSocket(socketFamily, socketType, handle);
    if (result == 0)
        SetHandle(handle);
    if (result != 0)
    {
        ostringstream stream;
        stream << "socket() failed, errorcode " << dec << result << " (" << hex << setw(2) << setfill('0')
               << result << "): " << OSAL::Network::GetSocketErrorMessage(result).c_str();
//        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, result);
    }
}

void socket::Close()
{
    int result = 0;
    if (GetHandle() != OSAL::Network::InvalidHandleValue)
    {
        result = OSAL::Network::CloseSocket(GetHandle());
        SetHandle(OSAL::Network::InvalidHandleValue);
    }
    if (result != 0)
    {
        ostringstream stream;
        stream << "close() failed, errorcode " << dec << result << " (" << hex << setw(2) << setfill('0')
               << result << "): " << OSAL::Network::GetSocketErrorMessage(result).c_str();
//        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, result);
    }
}

bool socket::IsClosed()
{
    return (GetHandle() == OSAL::Network::InvalidHandleValue);
}

void socket::SetSocketOption(OSAL::Network::SocketOptionLevel level,
                             OSAL::Network::SocketOption socketOption,
                             void *optionValue, socklen_t optionLength)
{
    int result = OSAL::Network::SetSocketOption(this->GetHandle(), level, socketOption, optionValue, optionLength);
    if (result != 0)
    {
        ostringstream stream;
        stream << "setsockopt() failed, errorcode " << dec << result << " (" << hex << setw(2) << setfill('0')
               << result << "): " << strerror(result);
//        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, result);
    }
}

void socket::GetSocketOption(OSAL::Network::SocketOptionLevel level, OSAL::Network::SocketOption socketOption,
                             void *optionValue, socklen_t &optionLength)
{
    int result = OSAL::Network::GetSocketOption(this->GetHandle(), level, socketOption, optionValue, optionLength);
    if (result != 0)
    {
        ostringstream stream;
        stream << "getsockopt() failed, errorcode " << dec << result << " (" << hex << setw(2) << setfill('0')
               << result << "): " << strerror(result);
//        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, result);
    }
}

void socket::SetSocketOption(OSAL::Network::SocketOption socketOption, void *optionValue, socklen_t optionLength)
{
    SetSocketOption(OSAL::Network::SocketOptionLevel::Socket, socketOption, optionValue, optionLength);
}

void socket::GetSocketOption(OSAL::Network::SocketOption socketOption, void *optionValue, socklen_t &optionLength)
{
    GetSocketOption(OSAL::Network::SocketOptionLevel::Socket, socketOption, optionValue, optionLength);
}

void socket::SetSocketOptionBool(OSAL::Network::SocketOption socketOption, bool value)
{
    int optionValue = value ? 1 : 0;
    SetSocketOption(socketOption, &optionValue, sizeof(optionValue));
}

bool socket::GetSocketOptionBool(OSAL::Network::SocketOption socketOption)
{
    int optionValue;
    socklen_t optionLength = sizeof(optionValue);
    GetSocketOption(socketOption, &optionValue, optionLength);
    return optionValue != 0;
}

void socket::SetSocketOptionInt(OSAL::Network::SocketOption socketOption, int optionValue)
{
    SetSocketOption(OSAL::Network::SocketOptionLevel::Socket, socketOption, &optionValue, sizeof(optionValue));
}

int socket::GetSocketOptionInt(OSAL::Network::SocketOption socketOption)
{
    int optionValue;
    socklen_t optionLength = sizeof(optionValue);
    GetSocketOption(OSAL::Network::SocketOptionLevel::Socket, socketOption, &optionValue, optionLength);
    return optionValue;
}

bool socket::GetBroadcastOption()
{
    return GetSocketOptionBool(OSAL::Network::SocketOption::Broadcast);
}

void socket::SetBroadcastOption(bool value)
{
    SetSocketOptionBool(OSAL::Network::SocketOption::Broadcast, value);
}

bool socket::GetBlockingMode()
{
    bool blockingMode {};
    int result = OSAL::Network::GetBlockingMode(GetHandle(), blockingMode);
    if (result == EINVAL) // In Windows, GetBlockingMode does not work
    {
        return _isBlocking;
    } else if (result != 0)
    {
        ostringstream stream;
        stream << "fcntl() failed, errorcode " << dec << result << " (" << hex << setw(2) << setfill('0')
               << result << "): " << strerror(result);
//        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, result);
    }
    return blockingMode;
}

void socket::SetBlockingMode(bool value)
{
    int result = OSAL::Network::SetBlockingMode(GetHandle(), value);
    if (result != 0)
    {
        ostringstream stream;
        stream << "fcntl() failed, errorcode " << dec << result << " (" << hex << setw(2) << setfill('0')
               << result << "): " << strerror(result);
//        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, result);
    }
    _isBlocking = value;
}

bool socket::GetReuseAddress()
{
    return GetSocketOptionBool(OSAL::Network::SocketOption::ReuseAddress);
}

void socket::SetReuseAddress(bool value)
{
    SetSocketOptionBool(OSAL::Network::SocketOption::ReuseAddress, value);
}

int socket::GetReceiveTimeout()
{
    timeval tv;
    memset(&tv, 0, sizeof(tv));
    socklen_t optionLength = sizeof(tv);
    GetSocketOption(OSAL::Network::SocketOption::ReceiveTimeout, &tv, optionLength);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void socket::SetReceiveTimeout(int timeoutMS)
{
    timeval tv;
    memset(&tv, 0, sizeof(tv));
    tv.tv_sec = timeoutMS / 1000;
    tv.tv_usec = (timeoutMS % 1000) * 1000;
    SetSocketOption(OSAL::Network::SocketOption::ReceiveTimeout, &tv, sizeof(tv));
}

int socket::GetSendTimeout()
{
    timeval tv;
    memset(&tv, 0, sizeof(tv));
    socklen_t optionLength = sizeof(tv);
    GetSocketOption(OSAL::Network::SocketOption::SendTimeout, &tv, optionLength);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void socket::SetSendTimeout(int timeoutMS)
{
    timeval tv;
    memset(&tv, 0, sizeof(tv));
    tv.tv_sec = timeoutMS / 1000;
    tv.tv_usec = (timeoutMS % 1000) * 1000;
    SetSocketOption(OSAL::Network::SocketOption::SendTimeout, &tv, sizeof(tv));
}

void socket::Bind(OSAL::Network::EndPointPtr address)
{
    int result = OSAL::Network::Bind(this->GetHandle(), address);
    if (result != 0)
    {
        ostringstream stream;
        stream << "bind() failed, errorcode " << dec << result << " (" << hex << setw(2) << setfill('0')
               << result << "): " << strerror(result);
//        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, result);
    }
}

bool socket::Connect(OSAL::Network::EndPointPtr serverAddress, OSAL::Network::SocketTimeout timeout)
{
    bool connected;
    int result = OSAL::Network::Connect(this->GetHandle(), serverAddress, connected, timeout);
    if (result != 0)
    {
        ostringstream stream;
        stream << "connect() failed, errorcode " << dec << result << " (" << hex << setw(2) << setfill('0')
               << result << "): " << strerror(result);
//        Core::TheLogger().Debug(OSAL::OS::TypeName(*this), stream.str());
    }

    return connected;
}

void socket::Listen(int numListeners)
{
    int result = OSAL::Network::Listen(this->GetHandle(), numListeners);
    if (result != 0)
    {
        ostringstream stream;
        stream << "listen() failed, errorcode " << dec << result << " (" << hex << setw(2) << setfill('0')
               << result << "): " << strerror(result);
//        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, result);
    }
}

bool socket::Accept(socket &connectionSocket, OSAL::Network::EndPointPtr &clientAddress,
                    OSAL::Network::SocketTimeout timeout)
{
    OSAL::Network::SocketHandle acceptHandle {};
    connectionSocket = {};
    int result = OSAL::Network::Accept(this->GetHandle(), _socketFamily, clientAddress, acceptHandle, timeout);
    if (result != 0)
    {
        ostringstream stream;
        stream << "connect() failed, errorcode " << dec << result << " (" << hex << setw(2) << setfill('0')
               << result << "): " << strerror(result);
//        Core::TheLogger().Debug(OSAL::OS::TypeName(*this), stream.str());
    } else
    {
        connectionSocket.SetHandle(acceptHandle);
    }

    return acceptHandle != OSAL::Network::InvalidHandleValue;
}

void socket::GetLocalAddress(OSAL::Network::EndPointPtr &address)
{
    int result = OSAL::Network::GetSockName(GetHandle(), _socketFamily, address);
    if (result != 0)
    {
        ostringstream stream;
        stream << "getsockname() failed, errorcode " << dec << result << " (" << hex << setw(2)
               << setfill('0') << result << "): " << strerror(result);
//        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, result);
    }
}

void socket::GetRemoteAddress(OSAL::Network::EndPointPtr &address)
{
    int result = OSAL::Network::GetPeerName(GetHandle(), _socketFamily, address);
    if (result != 0)
    {
        ostringstream stream;
        stream << "getpeername() failed, errorcode " << dec << result << " (" << hex << setw(2) << setfill('0')
               << result << "): " << strerror(result);
//        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, result);
    }
}

size_t socket::Receive(OSAL::bytearray &data, int flags)
{
    uint8_t buffer[BufferSize];
    size_t numBytes = 0;
    size_t offset = 0;

    do
    {
        numBytes = Receive(buffer, BufferSize, flags);

        data.set(offset, buffer, numBytes);
        offset += numBytes;
    } while (numBytes > 0);
    return offset;
}

size_t socket::Receive(uint8_t *data, size_t bufferSize, int flags)
{
    size_t numBytes = 0;
    try
    {
        ssize_t result = 0;

        result = OSAL::Network::Receive(GetHandle(), data, bufferSize, flags);
        if (result != 0)
        {
            int errorCode = errno;

#ifdef TRACE
            ostringstream stream;
            stream << "recv() failed, errorcode " << dec << errorCode << " (" << hex << setw(2) << setfill('0') << errorCode << "): " << strerror(errorCode);
//            Core::TheLogger().Debug(OSAL::OS::TypeName(*this), stream.str());
#endif
            if ((errorCode != EINTR) && (errorCode != EWOULDBLOCK) && (errorCode != EAGAIN))
                OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
        } else if (result == 0)
        {
            Close();
        } else
        {
            numBytes = static_cast<size_t>(result);
        }
    }
    catch (const OSAL::SystemError &e)
    {
        if (e.GetErrorCode() == EBADF)
        {
            numBytes = 0;
        }
    }
    return numBytes;
}

bool socket::Send(OSAL::bytearray &data, size_t bytesToSend, int flags)
{
    ASSERT(bytesToSend <= data.size());
    return Send(data.data(), bytesToSend, flags);
}

bool socket::Send(const uint8_t *data, size_t bytesToSend, int flags)
{
    ssize_t numBytesLeftToSend = static_cast<ssize_t>(bytesToSend);
    size_t offset = 0;

    while (numBytesLeftToSend > 0)
    {
        ssize_t numBytes = OSAL::Network::Send(GetHandle(), data + offset, static_cast<size_t>(numBytesLeftToSend), flags);
        if (numBytes == -1)
        {
            int errorCode = errno;

            ostringstream stream;
            stream << "send() failed, errorcode " << dec << errorCode << " (" << hex << setw(2) << setfill('0')
                   << errorCode << "): " << strerror(errorCode);
//            Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

            if ((errorCode == EPIPE) || (errorCode == ECONNRESET))
            {
                return false;
            }
            OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
        } else
        {
            offset += numBytes;
            numBytesLeftToSend -= numBytes;
        }
    }
    return true;
}

void socket::SendTo(const OSAL::Network::EndPointPtr &address, const uint8_t *data, size_t bytesToSend)
{
    ssize_t result = OSAL::Network::SendTo(GetHandle(), data, bytesToSend, 0, address);
    if (result == -1)
    {
        int errorCode = errno;

        ostringstream stream;
        stream << "sendto() failed, errorcode " << dec << errorCode
               << " (" << hex << setw(2) << setfill('0') << errorCode << "): " << strerror(errorCode);
//        Core::TheLogger().Error(OSAL::OS::TypeName(*this), stream.str());

        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
    }
}

size_t socket::ReceiveFrom(OSAL::Network::EndPointPtr &address, uint8_t *data, size_t bufferSize)
{
    ssize_t numBytes = OSAL::Network::ReceiveFrom(GetHandle(), data, bufferSize, 0, _socketFamily, address);
    if (numBytes == -1)
    {
        int errorCode = errno;

#ifdef TRACE
        ostringstream stream;
        stream << "recvfrom() failed, errorcode " << dec << errorCode
               << " (" << hex << setw(2) << setfill('0') << errorCode << "): " << strerror(errorCode);
//        Core::TheLogger().Debug(OSAL::OS::TypeName(*this), stream.str());
#endif

        if ((errorCode != EINTR) && (errorCode != EWOULDBLOCK) && (errorCode != EAGAIN))
            OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
        numBytes = 0;
    }

    return static_cast<size_t>(numBytes);
}

} // namespace Network
