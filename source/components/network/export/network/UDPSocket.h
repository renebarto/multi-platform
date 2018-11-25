#pragma once

#include "network/Socket.h"

namespace Network
{

class UDPSocket : public socket
{
public:
    UDPSocket() {}
    virtual ~UDPSocket() {}

    void Open();
//
//protected:
//    using Socket::Open;
};

} // namespace Network
