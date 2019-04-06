/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifdef _MSC_VER
# pragma warning (disable : 4464) // Suppress warning including relative headers
#endif

#include "sscf1/udpsocket.h"
#include "sscf1/system.h"
#include "sscf1/tcpsocket.h"
#include "../../include/sscf/commthread.h"
#include <cassert>
#include <cstring>
#ifdef _MSC_VER
# pragma warning (disable : 4265) // Suppress warning from Microsoft headers
#endif
#include <functional>
#ifdef _MSC_VER
# pragma warning (default : 4265)
#endif

#if defined(__unix__)
# include <netinet/tcp.h>
# include <fcntl.h>
#elif defined(__VXWORKS__)
# include <ioLib.h>
# include <stdio.h> 
#endif

namespace Sscf1 {

struct UdpSocket::Private
{
    struct SendBuffer
    {
        std::string data;
		IpAddress address;
        SendCallback *callback;
    };

	Private() 
	  :	commThread(CommThread::instance()), 
		socket(InvalidFileDesc)
	{
	}

    std::recursive_mutex mutex;

    const std::shared_ptr<CommThread> commThread;
	IpAddress iface;
	FileDesc socket;

    std::queue<SendBuffer> sendQueue;

    void resumeSend();
};

UdpSocket::UdpSocket(IpAddress iface)
  : p(new Private())
{
	bool result = false;

	p->iface = std::move(iface);

    p->commThread->send([this, &result]
	{
#if !defined(_WIN32) || !defined(WSA_FLAG_NO_HANDLE_INHERIT)
		p->socket = ::socket(p->iface.sockaddr()->sa_family, SOCK_DGRAM, 0);
#else
		p->socket = ::WSASocketW(p->iface.sockaddr()->sa_family, SOCK_DGRAM, 0, NULL, 0, WSA_FLAG_NO_HANDLE_INHERIT);
		if (p->socket == InvalidFileDesc)
			p->socket = ::socket(p->iface.sockaddr()->sa_family, SOCK_DGRAM, 0);
#endif

		if (p->socket != InvalidFileDesc)
		{
#if defined(__unix__)
			const int flags = ::fcntl(p->socket, F_GETFL, 0);
			if (flags >= 0)
				result = ::fcntl(p->socket, F_SETFL, flags | O_NONBLOCK) != -1;
#elif defined (__VXWORKS__)
			int nonBlock = 1;
			result = ::ioctl(p->socket, FIONBIO, (int)&nonBlock) == 0;
#elif defined(_WIN32)
			u_long nonBlock = 1;
			result = ::ioctlsocket(p->socket, FIONBIO, &nonBlock) == 0;
#endif
		}
	});

	if (!result)
		system().fail("Failed to create UDP socket (%d)", lastError());
}

UdpSocket::~UdpSocket()
{
    UdpSocket::close();

    delete p;
    *const_cast<Private **>(&p) = nullptr;
}

void UdpSocket::close(void)
{
    p->commThread->removeEvents(p->socket);

    p->commThread->send([this]
	{
		if (p->socket != InvalidFileDesc)
		{
#if defined(__unix__)
            ::close(p->socket);
#elif defined(_WIN32)
            ::closesocket(p->socket);
#endif
			p->socket = InvalidFileDesc;
		}
	});

    std::lock_guard<std::recursive_mutex> _(p->mutex);

    while (!p->sendQueue.empty())
        p->sendQueue.pop();
}

bool UdpSocket::enableMulticastLoopback()
{
	bool result = false;
	p->commThread->send([this, &result]
	{
		const int enabled = 1;

		switch (p->iface.family())
		{
		case IpAddress::Family::IPv4:
			result = ::setsockopt(
				p->socket,
				IPPROTO_IP, IP_MULTICAST_LOOP,
				(char *)&enabled, sizeof(enabled)) == 0;

			break;

		case IpAddress::Family::IPv6:
			result = ::setsockopt(
				p->socket,
				IPPROTO_IPV6, IPV6_MULTICAST_LOOP,
				(char *)&enabled, sizeof(enabled)) == 0;

			break;
		}
	});

	return result;
}

bool UdpSocket::bind(ReceiveCallback *callback, uint16_t port, bool multicast)
{
    return bind(
        callback,
        multicast ? IpAddress::any(p->iface.family()) : IpAddress(),
        port);
}

bool UdpSocket::bind(ReceiveCallback *callback, IpAddress group, uint16_t port)
{
	bool result = false;
	p->commThread->send([this, callback, &group, port, &result]
	{
		IpAddress addr = 
            group
#if defined(_WIN32)
            && (p->iface.family() == IpAddress::Family::IPv6)
#endif
            ? IpAddress::any(p->iface.family())
            : p->iface;

		addr.setPort(port);

		if (addr.port() != 0)
		{
			const int on = 1;
			result = ::setsockopt(p->socket, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) == 0;
		}
		else
			result = true;

		if (::bind(p->socket, addr.sockaddr(), socklen_t(addr.addrlen())) != -1)
		{
			// Read the acutally bound port number.
			socklen_t addrSize = sizeof(sockaddr_storage);
			if (::getsockname(p->socket, addr.sockaddr(), &addrSize) == 0)
			{
				system().traceMsg(
					"UdpSocket: Bound %s",
                    addr.toString().c_str());

				p->iface.setPort(addr.port());

				switch (p->iface.family())
				{
				case IpAddress::Family::IPv4:
					// Set multicast settings.
					if (group && (group.family() == IpAddress::Family::IPv4))
					{
                        const auto ifaceAddr = reinterpret_cast<const sockaddr_in *>(p->iface.sockaddr());
                        const auto groupAddr = reinterpret_cast<const sockaddr_in *>(group.sockaddr());

                        if (groupAddr->sin_addr.s_addr != htonl(INADDR_ANY))
                        {
                            ::ip_mreq imr;
                            imr.imr_multiaddr = groupAddr->sin_addr;
                            imr.imr_interface = ifaceAddr->sin_addr;
                            result &= ::setsockopt(p->socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&imr, sizeof(imr)) == 0;

                            system().traceMsg(
                                "UdpSocket: %s %s to %s on %s",
                                result ? "Joined" : "Failed to join",
                                addr.toString().c_str(),
                                group.toString(false).c_str(),
                                p->iface.toString(false).c_str());
                        }

                        if (result && ifaceAddr->sin_addr.s_addr)
                        {
                            ::in_addr mif;
                            mif = ifaceAddr->sin_addr;
                            result &= ::setsockopt(p->socket, IPPROTO_IP, IP_MULTICAST_IF, (char *)&mif, sizeof(mif)) == 0;

                            system().traceMsg(
                                "UdpSocket: %s multicast interface for %s to %s",
                                result ? "Set" : "Failed to set",
                                addr.toString().c_str(),
                                p->iface.toString(false).c_str());
                        }
                    }

					break;

                case IpAddress::Family::IPv6:
					// Set multicast settings.
					if (group && (group.family() == IpAddress::Family::IPv6))
					{
                        const unsigned ifIndex = IpAddress::findInterface(p->iface).index;
						const auto groupAddr = reinterpret_cast<const sockaddr_in6 *>(group.sockaddr());

                        if (memcmp(&groupAddr->sin6_addr, &in6addr_any, sizeof(in6addr_any)) != 0)
                        {
                            ::ipv6_mreq imr;
                            imr.ipv6mr_multiaddr = groupAddr->sin6_addr;
                            imr.ipv6mr_interface = ifIndex;
                            result &= ::setsockopt(p->socket, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP, (char *)&imr, sizeof(imr)) == 0;

                            system().traceMsg(
                                "UdpSocket: %s %s to %s on %u(%s)",
                                result ? "Joined" : "Failed to join",
                                addr.toString().c_str(),
                                group.toString(false).c_str(),
                                ifIndex,
                                p->iface.toString(false).c_str());
                        }

                        if (result && ifIndex)
                        {
                            result &= ::setsockopt(p->socket, IPPROTO_IPV6, IPV6_MULTICAST_IF, (char *)&ifIndex, sizeof(ifIndex)) == 0;

                            system().traceMsg(
                                "UdpSocket: %s multicast interface for %s to %u(%s)",
                                result ? "Set" : "Failed to set",
                                addr.toString().c_str(),
                                ifIndex,
                                p->iface.toString(false).c_str());
                        }
                    }

					break;
				}

				p->commThread->setReadEvent(p->socket, [this, callback]
				{
					std::vector<char> buffer;
					buffer.resize(65536);

					IpAddress from;
					socklen_t fromSize = sizeof(sockaddr_storage);
					size_t size = static_cast<size_t>(::recvfrom(p->socket, &buffer[0], int(buffer.size()), 0, from.sockaddr(), &fromSize));

                    if ((size != size_t(-1)) && callback)
						callback->receiveReceived(&buffer[0], int(size), std::move(from));
				});

				result = true;
			}
			else
				result = false;
		}
		else
			result = false;

		if (!result)
		{
			system().traceMsg(
				"UdpSocket: Failed to bind %s",
                p->iface.toString().c_str());
		}
	});

    return result;
}

const IpAddress & UdpSocket::address(void) const
{
	return p->iface;
}

void UdpSocket::beginSendTo(SendCallback *callback, IpAddress address, const char *buffer, int len)
{
    if (buffer)
    {
        if (len <= 0)
            len = int(::strlen(buffer));

        beginSendTo(callback, std::move(address), &buffer, &len, 1);
    }
}

void UdpSocket::beginSendTo(SendCallback *callback, IpAddress address, const char * const *buffer, const int *len, int count)
{
    std::lock_guard<std::recursive_mutex> _(p->mutex);

    assert(p->socket != InvalidFileDesc);

    size_t totalSize = 0;
    for (int i=0; i<count; i++)
        totalSize += len[i];

    Private::SendBuffer buf;
    buf.data.resize(totalSize);
	buf.address = std::move(address);
    buf.callback = callback;

    size_t pos = 0;
    for (int i=0; i<count; i++)
	if (len[i] > 0)
    {
        ::memcpy(&(buf.data[pos]), buffer[i], static_cast<size_t>(len[i]));
        pos += len[i];
    }

    const bool restart = p->sendQueue.empty();
    p->sendQueue.push(buf);
    if (restart)
        p->commThread->post(std::bind(&Private::resumeSend, p));
}

UdpSocket::SendCallback::SendCallback()
{
}

UdpSocket::SendCallback::~SendCallback()
{
}

UdpSocket::ReceiveCallback::ReceiveCallback()
{
}

UdpSocket::ReceiveCallback::~ReceiveCallback()
{
}

void UdpSocket::Private::resumeSend()
{
	std::unique_lock<std::recursive_mutex> l(mutex);

    while (!sendQueue.empty())
    {
        assert(socket != InvalidFileDesc);

        const SendBuffer buf = std::move(sendQueue.front());
		const auto sent = ::sendto(
			socket, 
			(char *)&(buf.data[0]), 
			int(buf.data.size()), 
			0, 
			buf.address.sockaddr(), 
			socklen_t(buf.address.addrlen()));

        if (sent != -1)
        {
            sendQueue.pop();

            l.unlock();

            if (buf.callback)
                buf.callback->sendSent(&(buf.data[0]), int(buf.data.size()));

            l.lock();
        }
		else if (!errorIoPending())
		{
			while (!sendQueue.empty())
				sendQueue.pop();

			commThread->removeEvents(socket);
			break;
		}
		else
            commThread->setWriteEvent(socket, std::bind(&Private::resumeSend, this));
    }
}

} // End of namespaces
