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
#ifdef _MSC_VER
# pragma warning (disable : 4464) // Suppress warning including relative headers
#endif

#endif

#define REMOTING1_IPADDRESS_CPP
#include "remoting1/ipaddress.h"
#include "remoting1/system.h"
#include "../../include/remoting/commthread.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <locale>
#include <map>
#include <set>
#include <sstream>
#include <stdio.h>

#if defined(__unix__)
# include <ifaddrs.h>
# include <netdb.h>
# include <net/if.h>
#elif defined(_WIN32)
# include <iphlpapi.h> // Link with iphlpapi.lib
#endif

#ifdef _MSC_VER
# pragma comment(lib, "ws2_32.lib") 
# pragma comment(lib, "iphlpapi.lib") 
#endif

namespace Remoting1 {

union IpAddress::Private
{
	sockaddr_storage storage;
	sockaddr_in in;
	sockaddr_in6 in6;
	struct sockaddr sockaddr;
};

static sockaddr_in make(uint32_t addr)
{
	sockaddr_in in;
	memset(&in, 0, sizeof(in));
	in.sin_family = AF_INET;
	in.sin_port = 0;
	in.sin_addr.s_addr = htonl(addr);

	return in;
}

static sockaddr_in6 make(const struct in6_addr &addr)
{
	sockaddr_in6 in6;
	memset(&in6, 0, sizeof(in6));
	in6.sin6_family = AF_INET6;
	in6.sin6_port = 0;
	in6.sin6_addr = addr;

	return in6;
}

static IpAddress findLoopback(const IpAddress &network)
{
#if defined(__unix__) // TODO: fix IPv6 multicast on loopback device.
	if (network.family() == IpAddress::Family::IPv6)
	{
		for (auto &&i : IpAddress::listInterfaces())
		{
			if ((i.address.family() == IpAddress::Family::IPv6) && (i.index > 1))
				return std::move(i.address);
		}
	}
#endif

	if (auto iface = IpAddress::findInterface(network))
		return std::move(iface.address);

	return network;
}

// Bug in VS2012 and VS2013 implementation of static initialization.
#if !defined(_MSC_VER) || (_MSC_VER >= 1900)
static const IpAddress & any4()
{
	static const IpAddress a = make(INADDR_ANY);

	return a;
}

static const IpAddress & any6()
{
	static const IpAddress a = make(in6addr_any);

	return a;
}

const IpAddress & IpAddress::any(Family family)
{
	switch (family)
	{
	case Family::IPv4: return any4();
	case Family::IPv6: return any6();
	}

	system().fail("Invalid address family specified");
}

static const IpAddress & detectAny()
{
	for (const auto &i : IpAddress::listInterfaces())
	{
		if (i.address.family() == IpAddress::Family::IPv6)
			return any6();
	}

	return any4();
}

const IpAddress & IpAddress::any()
{
	static const IpAddress &a = detectAny();

	return a;
}

static const IpAddress & loopback4()
{
	static const IpAddress a = findLoopback(make(INADDR_LOOPBACK));

	return a;
}

static const IpAddress & loopback6()
{
	static const IpAddress a = findLoopback(make(in6addr_loopback));

	return a;
}

const IpAddress & IpAddress::loopback(Family family)
{
	switch (family)
	{
	case Family::IPv4: return loopback4();
	case Family::IPv6: return loopback6();
	}

	system().fail("Invalid address family specified");
}

static const IpAddress & detectLoopback()
{
	for (const auto &i : IpAddress::listInterfaces())
	{
		if (i.address.family() == IpAddress::Family::IPv4)
			return loopback4();
	}

	return loopback6();
}

const IpAddress & IpAddress::loopback()
{
	static const IpAddress &a = detectLoopback();

	return a;
}
#else
IpAddress IpAddress::any(Family family)
{
	switch (family)
	{
	case Family::IPv4: return make(INADDR_ANY);
	case Family::IPv6: return make(in6addr_any);
	}

	system().fail("Invalid address family specified");
}

IpAddress IpAddress::any()
{
	for (const auto &i : IpAddress::listInterfaces())
	{
		if (i.address.family() == IpAddress::Family::IPv6)
			return any(Family::IPv6);
	}

	return any(Family::IPv4);
}

IpAddress IpAddress::loopback(Family family)
{
	switch (family)
	{
	case Family::IPv4: return findLoopback(make(INADDR_LOOPBACK));
	case Family::IPv6: return findLoopback(make(in6addr_loopback));
	}

	system().fail("Invalid address family specified");
}

IpAddress IpAddress::loopback()
{
	for (const auto &i : IpAddress::listInterfaces())
	{
		if (i.address.family() == IpAddress::Family::IPv4)
			return loopback(Family::IPv4);
	}

	return loopback(Family::IPv6);
}
#endif

template <typename _type>
static unsigned popcount(const _type &data)
{
	unsigned result = 0;

	for (unsigned i = 0; i < sizeof(data); i++)
	{
		uint8_t b = reinterpret_cast<const uint8_t *>(&data)[i];
        b = static_cast<uint8_t>(b - ((b >> 1) & 0x55));
        b = static_cast<uint8_t>((b & 0x33) + ((b >> 2) & 0x33));
        b = static_cast<uint8_t>((b + (b >> 4)) & 0x0F);
		result += b;	
	}

	return result;
}

std::vector<IpAddress::Interface> IpAddress::listInterfaces()
{
	std::multimap<unsigned, IpAddress::Interface> sorted;

#if defined(__unix__)
	::ifaddrs *ifaddrs = nullptr;
	if (::getifaddrs(&ifaddrs) == 0)
	{
        for (::ifaddrs *i = ifaddrs; i; i = i->ifa_next)
		if (i->ifa_addr && i->ifa_netmask)
		{
            IpAddress::Interface iface;
            if (i->ifa_name)
                iface.index = ::if_nametoindex(i->ifa_name);

			if ((i->ifa_addr->sa_family == AF_INET) && (i->ifa_netmask->sa_family == AF_INET))
			{
				memcpy(iface.address.sockaddr(), i->ifa_addr, sizeof(sockaddr_in));
				memcpy(iface.netmask.sockaddr(), i->ifa_netmask, sizeof(sockaddr_in));

				const auto mask = popcount(
					reinterpret_cast<const sockaddr_in *>(i->ifa_addr)->sin_addr);

				sorted.emplace((mask == 32) ? 0 : mask, std::move(iface));
			}
			else if ((i->ifa_addr->sa_family == AF_INET6) && (i->ifa_netmask->sa_family == AF_INET6))
			{
				memcpy(iface.address.sockaddr(), i->ifa_addr, sizeof(sockaddr_in6));
				memcpy(iface.netmask.sockaddr(), i->ifa_netmask, sizeof(sockaddr_in6));

				const auto mask = popcount(
					reinterpret_cast<const sockaddr_in6 *>(i->ifa_addr)->sin6_addr);

				sorted.emplace((mask == 128) ? 0 : mask, std::move(iface));
			}
		}

		::freeifaddrs(ifaddrs);
	}
#elif defined(_WIN32)
	// All WinSock code must be executed on the communication thread.
	CommThread::instance()->send([&sorted]
	{
#if !defined(WSA_FLAG_NO_HANDLE_INHERIT)
		auto sock = ::socket(AF_INET, SOCK_DGRAM, 0);
#else
		auto sock = ::WSASocketW(AF_INET, SOCK_DGRAM, 0, NULL, 0, WSA_FLAG_NO_HANDLE_INHERIT);
		if (sock == InvalidFileDesc)
			sock = ::socket(AF_INET, SOCK_DGRAM, 0);
#endif

		std::vector<INTERFACE_INFO> info;
		info.resize(1024);
		memset(&info[0], 0, info.size() * sizeof(info[0]));

		DWORD bytes = 0;
		if (::WSAIoctl(
			sock, SIO_GET_INTERFACE_LIST, 0, 0,
			&info[0], DWORD(info.size() * sizeof(info[0])),
			&bytes, 0, 0) != SOCKET_ERROR)
		{
			for (size_t i = 0, n = bytes / sizeof(info[0]); (i < n) && (i < info.size()); i++)
			{
				if ((info[i].iiAddress.Address.sa_family == AF_INET) &&
					(info[i].iiNetmask.Address.sa_family == AF_INET))
				{
					IpAddress::Interface iface;
					memcpy(
						iface.address.sockaddr(),
						&info[i].iiAddress.AddressIn,
						sizeof(info[i].iiAddress.AddressIn));

					memcpy(
						iface.netmask.sockaddr(),
						&info[i].iiNetmask.AddressIn,
						sizeof(info[i].iiNetmask.AddressIn));

					const auto mask = popcount(
						info[i].iiNetmask.AddressIn.sin_addr);

					sorted.emplace((mask == 32) ? 0 : mask, std::move(iface));
				}
			}
		}

		::closesocket(sock);
	});

	// All WinSock code must be executed on the communication thread.
	CommThread::instance()->send([&sorted]
	{
		for (DWORD i = 0, size = 8192; (i < 2) && (size > 0); i++)
		{
			std::vector<char> buffer;
			buffer.resize(size);
			const auto adapters = reinterpret_cast<IP_ADAPTER_ADDRESSES *>(&buffer[0]);

			const auto rc = GetAdaptersAddresses(
				AF_INET6,
				GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST | 
				GAA_FLAG_SKIP_DNS_SERVER | GAA_FLAG_SKIP_FRIENDLY_NAME,
				NULL,
				adapters,
				&size);

			if (rc == 0)
			{
				for (auto j = adapters; j; j = j->Next)
				{
					for (auto k = j->FirstUnicastAddress; k; k = k->Next)
					{
						if (k->Address.lpSockaddr->sa_family == AF_INET6)
						{
							IpAddress::Interface iface;
							memcpy(
								iface.address.sockaddr(),
								k->Address.lpSockaddr,
								static_cast<size_t>(k->Address.iSockaddrLength));

							iface.index = j->Ipv6IfIndex;

							auto netmask = reinterpret_cast<sockaddr_in6 *>(iface.netmask.sockaddr());
							netmask->sin6_family = AF_INET6;
							for (unsigned l = 0; l < k->OnLinkPrefixLength; l++)
							{
								const unsigned byte = l / 8, bit = l % 8;
								reinterpret_cast<uint8_t *>(&netmask->sin6_addr)[byte] |= 1 << bit;
							}

							const auto mask = k->OnLinkPrefixLength;
							sorted.emplace(
								(mask == 128) ? 0 : mask, 
								std::move(iface));
						}
					}
				}
			}
			else if (rc == ERROR_BUFFER_OVERFLOW)
				continue;

			break;
		}
	});
#endif

	std::vector<IpAddress::Interface> result;
	for (auto i = sorted.rbegin(); i != sorted.rend(); i++)
		result.emplace_back(std::move(i->second));

	return result;
}

IpAddress::Interface IpAddress::findInterface(const IpAddress &network)
{
	if (network)
	{
		auto interfaces = listInterfaces();

		IpAddress address = network;
		address.setScopeId(0);

		for (auto &&i : interfaces)
		{
			IpAddress c = i.address;
			c.setScopeId(0);
			if (c == address)
				return std::move(i);
		}

		for (auto &&i : interfaces)
		{
			if ((i.address & i.netmask) == (network & i.netmask))
				return std::move(i);
		}

		system().traceMsg(
			"IpAddress: No interface found for %s",
			network.toString(false).c_str());
	}

    return Interface();
}

IpAddress IpAddress::resolve(const char *hostname, uint16_t port, unsigned scopeId)
{
	IpAddress result;
	if (hostname && (hostname[0] != '\0'))
	{
		CommThread::instance()->send([hostname, port, scopeId, &result]
		{
			std::ostringstream pstr;
			pstr.imbue(std::locale::classic());
			pstr << port;

			struct addrinfo *info = nullptr;
			if (getaddrinfo(hostname, pstr.str().c_str(), nullptr, &info) == 0)
			{
				for (auto *i = info; i && !result; i = i->ai_next)
				{
					memcpy(result.sockaddr(), i->ai_addr, i->ai_addrlen);
					if ((result.sockaddr()->sa_family == AF_INET6) && scopeId)
						result.p->in6.sin6_scope_id = scopeId;
				}

                freeaddrinfo(info);
			}
		});
	}

	return result;
}

IpAddress::IpAddress() 
	: p(new Private())
{
	memset(p, 0, sizeof(*p));
}

IpAddress::IpAddress(const IpAddress &from) 
	: p(from.p ? new Private() : nullptr)
{
	if (p)
		memcpy(p, from.p, sizeof(*p));
}

IpAddress::IpAddress(IpAddress &&from)
	: p(from.p)
{
	from.p = nullptr;
}

IpAddress::IpAddress(const char *address, uint16_t port, unsigned scopeId)
	: p(new Private())
{
	memset(p, 0, sizeof(*p));

	if (inet_pton(AF_INET, address, &p->in.sin_addr) == 1)
	{
		p->in.sin_family = AF_INET;
		p->in.sin_port = htons(port);
	}
    else
    {
        std::string buffer = address;
        const auto pct = buffer.find('%');
        if (pct != buffer.npos)
            buffer[pct] = '\0';

		if (inet_pton(AF_INET6, &buffer[0], &p->in6.sin6_addr) == 1)
		{
			p->in6.sin6_family = AF_INET6;
			p->in6.sin6_port = htons(port);
			if (scopeId)
			{
				p->in6.sin6_scope_id = scopeId;
			}
			else if (pct != buffer.npos)
			{
#if defined(__unix__)
				p->in6.sin6_scope_id = if_nametoindex(&buffer[pct + 1]);
#elif defined(_WIN32)
				p->in6.sin6_scope_id = strtoul(&buffer[pct + 1], nullptr, 10);
#endif
			}
        }
    }
}

IpAddress::IpAddress(const sockaddr_in &from)
	: p(new Private())
{
	memset(p, 0, sizeof(*p));

	p->in = from;
}

IpAddress::IpAddress(const sockaddr_in6 &from)
	: p(new Private())
{
	memset(p, 0, sizeof(*p));

	p->in6 = from;
}

IpAddress::IpAddress(const std::string &from, uint16_t port, unsigned scopeId)
	: p(nullptr)
{
	*this = IpAddress(from.c_str(), port, scopeId);
}

IpAddress::~IpAddress()
{
	delete p;
	p = nullptr;
}

const char * IpAddress::toString(char(&buffer)[96], bool addPort, bool addScope) const
{
#if defined(__unix__)
    static const unsigned nameLen = IF_NAMESIZE;
#elif defined(_WIN32)
    static const unsigned nameLen = 12;
#endif
    static const unsigned portLen = 12;
	static_assert(sizeof(buffer) > (INET_ADDRSTRLEN + 1 + portLen), "buffer too small for IPv4 address");
    static_assert(sizeof(buffer) > (INET6_ADDRSTRLEN + 1 + nameLen + 3 + portLen), "buffer too small for IPv6 address");

	buffer[0] = '\0';

	switch (family())
	{
	case Family::IPv4:
        if (inet_ntop(AF_INET, &p->in.sin_addr, buffer, sizeof(buffer)))
		{
            const auto pos = strlen(buffer);
            if (addPort && p->in.sin_port && ((pos + 1 + portLen) < sizeof(buffer)))
			{
				std::ostringstream str;
				str.imbue(std::locale::classic());
				str << ':' << ntohs(p->in.sin_port);

				std::strncpy(&buffer[pos], str.str().c_str(), sizeof(buffer) - pos);
			}
		}

		break;

	case Family::IPv6:
        if (inet_ntop(AF_INET6, &p->in6.sin6_addr, buffer, sizeof(buffer)))
		{
            auto pos = strlen(buffer);

            const uint16_t linkLocal = htons(0xfe80);
            if (addScope && p->in6.sin6_scope_id &&
                (memcmp(&p->in6.sin6_addr, &linkLocal, sizeof(linkLocal)) == 0) &&
                ((pos + 1 + nameLen) < sizeof(buffer)))
            {
#if defined(__unix__)
                if (if_indextoname(p->in6.sin6_scope_id, &buffer[pos + 1]))
                    buffer[pos] = '%';
#elif defined(_WIN32)
				std::ostringstream str;
				str.imbue(std::locale::classic());
				str << '%' << p->in6.sin6_scope_id;

				std::strncpy(&buffer[pos], str.str().c_str(), sizeof(buffer) - pos);
#endif
                pos = strlen(buffer);
            }

            if (addPort && p->in6.sin6_port && ((pos + 3 + portLen) < sizeof(buffer)))
			{
				memmove(&buffer[1], &buffer[0], pos);
				buffer[0] = '[';

				std::ostringstream str;
				str.imbue(std::locale::classic());
				str << "]:" << ntohs(p->in6.sin6_port);

				std::strncpy(&buffer[pos + 1], str.str().c_str(), sizeof(buffer) - (pos + 1));
			}
		}

		break;
	}

	return buffer;
}

std::string IpAddress::toString(bool addPort, bool addScope) const
{
	char buffer[96];
    return std::string(toString(buffer, addPort, addScope));
}

IpAddress & IpAddress::operator=(const IpAddress &from)
{
	if (&from != this)
	{
		if (p && !from.p)
		{
			delete p;
			p = nullptr;
		}
		if (from.p && !p)
			p = new Private();

		if (p)
			memcpy(p, from.p, sizeof(*p));
	}

	return *this;
}

IpAddress & IpAddress::operator=(IpAddress &&from)
{
	delete p;
	p = from.p;
	from.p = nullptr;

	return *this;
}

IpAddress::operator bool() const
{
	return p && (p->storage.ss_family != 0);
}

bool IpAddress::operator==(const IpAddress &from) const
{
	if (p->storage.ss_family == from.p->storage.ss_family)
	{
		switch (family())
		{
		case Family::IPv4:
		{
			const int cmp = memcmp(
				&p->in.sin_addr,
				&from.p->in.sin_addr,
				sizeof(p->in.sin_addr));

			return (cmp == 0) && (p->in.sin_port == from.p->in.sin_port);
		}
		case Family::IPv6:
		{
			const int cmp = memcmp(
				&p->in6.sin6_addr,
				&from.p->in6.sin6_addr,
				sizeof(p->in6.sin6_addr));

			return 
				(cmp == 0) && 
                (p->in6.sin6_port == from.p->in6.sin6_port) &&
                (p->in6.sin6_scope_id == from.p->in6.sin6_scope_id);
		}
		}
	}

	return false;
}


bool IpAddress::operator<(const IpAddress &from) const
{
	if (p->storage.ss_family == from.p->storage.ss_family)
	{
		switch (family())
		{
		case Family::IPv4:
		{
			const int cmp = memcmp(
				&p->in.sin_addr,
				&from.p->in.sin_addr,
				sizeof(p->in.sin_addr));

			return
				(cmp < 0) ||
				((cmp == 0) && (p->in.sin_port < from.p->in.sin_port));
		}
		case Family::IPv6:
		{
			const int cmp = memcmp(
				&p->in6.sin6_addr,
				&from.p->in6.sin6_addr,
				sizeof(p->in6.sin6_addr));

			return
				(cmp < 0) ||
				((cmp == 0) && 
                 (p->in6.sin6_port < from.p->in6.sin6_port)) ||
				((cmp == 0) && 
				 (p->in6.sin6_port == from.p->in6.sin6_port) && 
                 (p->in6.sin6_scope_id < from.p->in6.sin6_scope_id));
		}
        }
	}

	return p->storage.ss_family < from.p->storage.ss_family;
}

static void memand(void *dst, const void *src, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		reinterpret_cast<uint8_t *>(dst)[i] &= 
			reinterpret_cast<const uint8_t *>(src)[i];
	}
}

IpAddress IpAddress::operator&(const IpAddress &netmask) const
{
	IpAddress result = *this;
	result &= netmask;
	return result;
}

IpAddress & IpAddress::operator&=(const IpAddress &netmask)
{
	if (p->storage.ss_family == netmask.p->storage.ss_family)
	{
		switch (family())
		{
		case Family::IPv4:
            memand(&p->in.sin_addr, &netmask.p->in.sin_addr, sizeof(p->in.sin_addr));
			p->in.sin_port &= netmask.p->in.sin_port;
			break;

		case Family::IPv6:
            memand(&p->in6.sin6_addr, &netmask.p->in6.sin6_addr, sizeof(p->in6.sin6_addr));
			p->in6.sin6_port &= netmask.p->in6.sin6_port;
			break;

		default:
			memset(p, 0, sizeof(*p));
			break;
		}
	}
	else
		memset(p, 0, sizeof(*p));

	return *this;
}

IpAddress::Family IpAddress::family() const
{
	switch (p->storage.ss_family)
	{
	case AF_INET: 
		return Family::IPv4;

	case AF_INET6: 
		return Family::IPv6;
	}

	return Family(-1);
}

void IpAddress::setScopeId(unsigned scopeId)
{
	switch (family())
	{
	case Family::IPv4:
		break;

	case Family::IPv6:
		p->in6.sin6_scope_id = scopeId;
		break;
	}
}

unsigned IpAddress::scopeId() const
{
	switch (family())
	{
	case Family::IPv4:
		return 0;

	case Family::IPv6:
		return p->in6.sin6_scope_id;
	}

	return 0;
}

void IpAddress::setPort(uint16_t port)
{
	switch (family())
	{
	case Family::IPv4: 
		p->in.sin_port = htons(port);
		break;

	case Family::IPv6:
		p->in6.sin6_port = htons(port);
		break;
	}
}

uint16_t IpAddress::port() const
{
	switch (family())
	{
	case Family::IPv4:
		return ntohs(p->in.sin_port);

	case Family::IPv6:
		return ntohs(p->in6.sin6_port);
	}

	return 0;
}

const struct sockaddr * IpAddress::sockaddr() const
{
	return &p->sockaddr;
}

struct sockaddr * IpAddress::sockaddr()
{
	return &p->sockaddr;
}

size_t IpAddress::addrlen() const
{
	switch (family())
	{
	case Family::IPv4:
		return sizeof(p->in);

	case Family::IPv6:
		return sizeof(p->in6);
	}

	return 0;
}

IpAddress::Interface::Interface()
  : address(), netmask(),
	index(0)
{
}

std::ostream & operator<<(std::ostream &str, const IpAddress &address)
{
    return str << address.toString();
}

} // End of namespaces
