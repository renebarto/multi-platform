/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifndef SSCF1_IPADDRESS_H
#define SSCF1_IPADDRESS_H

#include <cstdint>
// For C++/CLI <string> and <vector> give LNK4210: .CRTMP section exists; there may be unhandled static initializers or terminators
#ifndef _M_CEE
# include <ostream>
# include <string>
# include <vector>
struct sockaddr;
struct sockaddr_in;
struct sockaddr_in6;
#endif

namespace Sscf1 {

/*! This class represents an IPv4 or IPv6 address.
 */
class IpAddress
{
public:
	struct Interface;

	/*! The IP address family (i.e. IPv4 or IPv6).
	 */
	enum class Family { IPv4, IPv6 };

public:
	// Bug in VS2012 and VS2013 implementation of static initialization.
#if !defined(_MSC_VER) || (_MSC_VER >= 1900)
	/*! Returns the any IpAddress for the specified address family.
		\param family The address family.
	 */
	static const IpAddress & any(Family family);

	/*! Returns an IpAddress for :: if IPv6 is supported, otherwise returns an
		IpAddress for 0.0.0.0.
	 */
	static const IpAddress & any();

	/*! Returns the loopback IpAddress for the specified address family.
		\param family The address family.
	 */
	static const IpAddress & loopback(Family family);

    /*! Returns an IpAddress for 127.0.0.1 if IPv4 is supported, otherwise
     *  returns an IpAddress for ::1.
	 */
	static const IpAddress & loopback();
#else
	static IpAddress any(Family family);
	static IpAddress any();
	static IpAddress loopback(Family family);
	static IpAddress loopback();
#endif

#ifndef _M_CEE
	/*! Returns a list with the addresses and netmasks of all available network
		interfaces.
	 */
	static std::vector<Interface> listInterfaces();
#endif

    /*! Finds the interface for the specified network address.
     */
	static Interface findInterface(const IpAddress &network);

	/*! Resolves the address of the specified hostname or network address.
	 */
	static IpAddress resolve(const char *hostname, uint16_t port = 0, unsigned scopeId = 0);

#ifndef _M_CEE
	/*! Resolves the address of the specified hostname or network address.
	 */
	static IpAddress resolve(const std::string &hostname, uint16_t port = 0, unsigned scopeId = 0)
	{
		return resolve(hostname.c_str(), port, scopeId);
	}
#endif

public:
	/*! Creates an invalid IpAddress.
	 */
	IpAddress();

	/*! Creates an IpAddress from the string representation of the address.
	 */
	IpAddress(const char *address, uint16_t port = 0, unsigned scopeId = 0);

#ifndef _M_CEE
	IpAddress(const sockaddr_in &);
	IpAddress(const sockaddr_in6 &);
#endif

	IpAddress(const IpAddress &from);
	IpAddress(IpAddress &&from);
	~IpAddress();

#ifndef _M_CEE
	/*! Creates an IpAddress from the string representation of the address.
	*/
	IpAddress(const std::string &from, uint16_t port = 0, unsigned scopeId = 0);

    /*! Converts the address to a string.
     */
    std::string toString(bool addPort = true, bool addScope = true) const;
#endif

#if defined(_M_CEE) || defined(SSCF1_IPADDRESS_CPP)
    const char * toString(char(&buffer)[96], bool addPort = true, bool addScope = true) const;
#endif

	IpAddress & operator=(const IpAddress &from);
	IpAddress & operator=(IpAddress &&from);

	operator bool() const;
	bool operator==(const IpAddress &from) const;
	bool operator!=(const IpAddress &from) const { return !operator==(from); }
	bool operator<(const IpAddress &from) const;

	IpAddress operator&(const IpAddress &netmask) const;
	IpAddress & operator&=(const IpAddress &netmask);

	/*! Returns the address family.
	 */
	Family family() const;

	/*! Sets the IPv6 scope ID.
	*/
	void setScopeId(unsigned);

	/*! Returns the IPv6 scope ID.
	 */
	unsigned scopeId() const;

	/*! Sets the port number for the address.
	 */
	void setPort(uint16_t);

	/*! Returns the port number for the address.
	 */
	uint16_t port() const;

#ifndef _M_CEE
	const struct sockaddr * sockaddr() const;
	struct sockaddr * sockaddr();
	size_t addrlen() const;
#endif

private:
	union Private;
	Private *p;
};

/*! Provides information on a specific network interface.
 */
struct IpAddress::Interface
{
	Interface();

	operator bool() const { return address;  }

	IpAddress address, netmask; 
	unsigned index;
};

#ifndef _M_CEE
std::ostream & operator<<(std::ostream &, const IpAddress &);
#endif

} // End of namespaces

#endif
