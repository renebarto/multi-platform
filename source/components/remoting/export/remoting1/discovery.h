/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifndef REMOTING1_DISCOVERY_H
#define REMOTING1_DISCOVERY_H

#include <cstdint>
#ifndef _M_CEE // Issue with including <memory> and <vector> in C++/CLR
# include <memory>
# include <vector>
#endif
#include "ipaddress.h"

namespace Remoting1 {

/*! This class provides service discovery functionality. It publishes and finds
    services though the SSDP(UPnP) protocol.
 */
class Discovery
{
public:
    struct SearchCallback
    {
		SearchCallback();
		virtual ~SearchCallback();

        /*! Is invoked when the search has finished.
         */
		virtual void searchFound(const IpAddress &) = 0;
    };

public:
	/*! The default SSDP port.
	*/
	static const uint16_t defaultSsdpPort = 1900;

	/*! Sets the SSDP port used. Can be changed for testing purposes.
	 */
	static void setSsdpPort(uint16_t);

	/*! Returns the SSDP address to be used for the specified interface.
	 */
	static IpAddress ssdpAddress(const IpAddress &iface);

#ifndef _M_CEE // Issue with including <memory> in C++/CLR
	/*! Returns an instance to the Discovery singleton.
	 */
    static std::shared_ptr<Discovery> instance();
#endif

	/*! Publishes the service on the specified address and port through SSDP.
        \param name     The UPnP name of the service 
                        (e.g. "urn:healthcare-philips-com:service:MyService:1").
        \param id       The unique id of the service (e.g. UUID).
        \param address  The address and port the service is listening on.
     */
	void publishService(const char *name, const char *id, IpAddress address);

    /*! Unpublishes the specified service.
        \param name     The UPnP resource name of the service 
                        (e.g. "urn:healthcare-philips-com:service:MyService:1").
        \param id       The unique id of the service (e.g. UUID).
     */
    void unpublishService(const char *name, const char *id);

    /*! Searches for an instance of the specified service and stops when a
		service is found or the timeout expires (whichever comes first).
		\param callback The callback to invoke for the found service.
        \param name     The UPnP resource name of the service 
                        (e.g. "urn:healthcare-philips-com:service:MyService:1").
		\param network  The network address to search on.
        \param timeout  Timeout in milliseconds.
		\param single   When true, the search stops after the first service is
					    found and the callback is invoked exactly once. When
					    false, the search continues until the timeout expires
					    and the callback can be invoked multiple times, the
					    callback is invoked with a null endpoint to indicate that
					    the search has finished.
     */
	bool searchService(SearchCallback *callback, const char *name, const char *network, int timeout = -1, bool single = true);

    /*! Searches for an instance of the specified service and stops when a
		service is found or the timeout expires (whichever comes first).
		\param callback The callback to invoke for the found service.
        \param name     The UPnP resource name of the service 
                        (e.g. "urn:healthcare-philips-com:service:MyService:1").
		\param network  The network address to search on.
        \param timeout  Timeout in milliseconds.
		\param single   When true, the search stops after the first service is
					    found and the callback is invoked exactly once. When
					    false, the search continues until the timeout expires
					    and the callback can be invoked multiple times, the
					    callback is invoked with a null endpoint to indicate that
					    the search has finished.
     */
	bool searchService(SearchCallback *callback, const char *name, IpAddress network, int timeout = -1, bool single = true);

    /*! Searches for an instance of the specified service.
        \param name     The UPnP resource name of the service 
                        (e.g. "urn:healthcare-philips-com:service:MyService:1").
		\param network  The network address to search on.
        \param timeout  Timeout in milliseconds.
     */
	static IpAddress searchService(const char *name, const char *network, int timeout = -1);

    /*! Searches for an instance of the specified service.
        \param name     The UPnP resource name of the service 
                        (e.g. "urn:healthcare-philips-com:service:MyService:1").
		\param network  The network address to search on.
        \param timeout  Timeout in milliseconds.
     */
	static IpAddress searchService(const char *name, IpAddress network, int timeout = -1);

#ifndef _M_CEE // Issue with including <vector> in C++/CLR
    /*! Searches for instances of the specified service.
        \param name     The UPnP resource name of the service 
                        (e.g. "urn:healthcare-philips-com:service:MyService:1").
		\param network  The network address to search on.
     */
	static std::vector<IpAddress> searchServices(const char *name, const char *network);

    /*! Searches for instances of the specified service.
        \param name     The UPnP resource name of the service 
                        (e.g. "urn:healthcare-philips-com:service:MyService:1").
		\param network  The network address to search on.
     */
	static std::vector<IpAddress> searchServices(const char *name, IpAddress network);
#endif

    /*! Blocks until all searches for the specified callback have finished.
     */
    void stopSearches(SearchCallback *callback);

#if defined(_MSC_VER) && (_MSC_VER < 1800)
public:
#else
private:
#endif
    ~Discovery();

private:
	Discovery();
    Discovery(const Discovery &);
    Discovery & operator=(const Discovery &);

private:
    struct Private;
    Private * const p;
};

} // End of namespaces

#endif
