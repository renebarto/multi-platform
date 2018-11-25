#include "Network/UNIXSocket.h"

#if defined(LINUX) || defined(APPLE)

//#include <sstream>
//#include <unistd.h>
//#include <sys/un.h>
//#include <format.h>
//#include "Core/DefaultLogger.h"

using namespace std;
using namespace Network;

//static size_t BufferSize = 4096;
//
//UNIXSocket::UNIXSocket()
//{
//
//}
//
//UNIXSocket::UNIXSocket(UNIXSocket const & other)
//    : Socket(other)
//{
//}
//
//UNIXSocket::UNIXSocket(UNIXSocket && other)
//    : Socket(std::move(other))
//{
//}
//
//UNIXSocket::~UNIXSocket()
//{
//    if (!bindAddress.empty())
//    {
//        ::unlink(bindAddress.c_str());
//    }
//}
//
//UNIXSocket & UNIXSocket::operator = (UNIXSocket const & other)
//{
//    if (this != & other)
//    {
//        *(Socket *)this = other;
//    }
//    return *this;
//}
//
//UNIXSocket & UNIXSocket::operator = (UNIXSocket && other)
//{
//    if (this != & other)
//    {
//        *(Socket *)this = std::move(other);
//    }
//    return *this;
//}
//
//void UNIXSocket::Bind(std::string const & address)
//{
//    sockaddr_un bindAddress;
//    bindAddress.sun_family = AF_UNIX;
//    if (address.length() > sizeof(bindAddress.sun_path) - 1)
//    {
//        throw Core::RuntimeError(__func__, __FILE__, __LINE__,
//                                   fmt::format("Address too long: {}, size={}, should be no more than {}",
//                                               address, address.length(), sizeof(bindAddress)));
//    }
//    strcpy(bindAddress.sun_path, address.c_str());
//    ::unlink(bindAddress.sun_path);
//
//    Socket::Bind((sockaddr *)&bindAddress, sizeof(bindAddress));
//
//    this->bindAddress = address;
//}
//
//std::string UNIXSocket::GetLocalAddress()
//{
//    sockaddr_un address;
//    socklen_t size = sizeof(address);
//    Socket::GetLocalAddress((sockaddr *)&address, &size);
//    size_t nameSize = std::min(strlen(address.sun_path), sizeof(address.sun_path));
//
//    return std::string(address.sun_path, nameSize);
//}
//
//std::string UNIXSocket::GetRemoteAddress()
//{
//    sockaddr_un address;
//    socklen_t size = sizeof(address);
//    Socket::GetRemoteAddress((sockaddr *)&address, &size);
//    size_t nameSize = std::min(strlen(address.sun_path), sizeof(address.sun_path));
//
//    return std::string(address.sun_path, nameSize);
//}
//
//void UNIXSocket::SendTo(std::string const & address, const Core::ByteArray & data, size_t bytesToSend)
//{
//    if (bytesToSend > data.Size())
//        throw Core::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "bytesToSend", "Must be within size of data");
//
//    SendTo(address, data.Data(), bytesToSend);
//}
//
//void UNIXSocket::SendTo(std::string const & address, const uint8_t * data, size_t bytesToSend)
//{
//    sockaddr_un serverAddress;
//    serverAddress.sun_family = AF_UNIX;
//    if (address.length() > sizeof(serverAddress.sun_path) - 1)
//    {
//        throw Core::RuntimeError(__func__, __FILE__, __LINE__,
//                                   fmt::format("Address too long: {}, size={}, should be no more than {}",
//                                               address, address.length(), sizeof(serverAddress)));
//    }
//    strcpy(serverAddress.sun_path, address.c_str());
//    Socket::SendTo((sockaddr *)&serverAddress, sizeof(serverAddress), data, bytesToSend);
//}
//
//Core::ByteArray UNIXSocket::ReceiveFrom(std::string & clientAddress)
//{
//    uint8_t buffer[BufferSize];
//    size_t numBytes = ReceiveFrom(clientAddress, buffer, BufferSize);
//    return Core::ByteArray(buffer, numBytes);
//}
//
//size_t UNIXSocket::ReceiveFrom(std::string & clientAddress, uint8_t * data, size_t bufferSize)
//{
//    sockaddr_un serverAddress;
//    serverAddress.sun_family = AF_UNIX;
//    socklen_t serverAddressLength = sizeof(serverAddress);
//    clientAddress.clear();
//    size_t numBytes = Socket::ReceiveFrom((sockaddr *)&serverAddress, &serverAddressLength, data, bufferSize);
//    if (numBytes > 0)
//    {
//        if (serverAddressLength > sizeof(serverAddress.sun_family))
//        {
//            clientAddress = std::string(serverAddress.sun_path);
//        }
//    }
//
//    return numBytes;
//}

#endif // defined(LINUX) || defined(APPLE)

