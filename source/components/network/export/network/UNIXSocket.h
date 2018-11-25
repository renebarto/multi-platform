#pragma once

#include "Network/Socket.h"

#if defined(LINUX) || defined(APPLE)

namespace Network
{

//class UNIXSocket : public Socket
//{
//public:
//    UNIXSocket();
//    UNIXSocket(const UNIXSocket & other);
//    UNIXSocket(UNIXSocket && other);
//    virtual ~UNIXSocket();
//
//    UNIXSocket & operator = (UNIXSocket const & other);
//    UNIXSocket & operator = (UNIXSocket && other);
//
//    void            Bind(const std::string & address);
//
//    std::string   GetLocalAddress();
//    std::string   GetRemoteAddress();
//
//    void            SendTo(const std::string & address, const Core::ByteArray & data, size_t bytesToSend);
//    void            SendTo(const std::string & address, const uint8_t * data, size_t bytesToSend);
//    Core::ByteArray ReceiveFrom(std::string & clientAddress);
//    size_t          ReceiveFrom(std::string & clientAddress, uint8_t * data, size_t bufferSize);
//
//protected:
//    std::string bindAddress;
//};

} // namespace Network

#endif // defined(LINUX) || defined(APPLE)
