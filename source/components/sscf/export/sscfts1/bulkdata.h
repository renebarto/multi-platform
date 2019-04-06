/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2014, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifndef SSCFTS1_BULKDATA_H
#define SSCFTS1_BULKDATA_H

#include "sscf1/bulkchannel.h"
#ifndef _M_CEE
# include "eventloop.h"
# ifdef _MSC_VER
#  pragma warning (disable : 4265) // Suppress warning from Microsoft headers
# endif
# include <functional>
# ifdef _MSC_VER
#  pragma warning (default : 4265)
# endif
# include <vector>
#endif
#include <cstdint>
#include <cstdlib>

#if defined(_MSC_VER)
# define noexcept throw()
#endif

namespace SscfTs1 {

/*! This class can be used to transfer large blocks of data. Note that the
	memory in a BulkData object can be in use for a while while the data is
	being transmitted asynchronously and must remain available until the
	released() function that is provided to the constructor is invoked.
 */
class BulkData : public Sscf1::BulkChannel::Buffer
{
public:
	/*! Constructs an empty BulkData object.
	 */
	BulkData();

#ifndef _M_CEE
	/*! Constructs a BulkData object with the specified data block.
		\param data			The pointer to the first element in the data block.
		\param size			The number of elements in the data block.
		\param eventLoop	The EventLoop where the released event is raised.
		\param released		Function that is called when the memory in the data
							block can be released.
	 */
	BulkData(const uint8_t *data, size_t size, IEventLoop &eventLoop, std::function<void()> released);

	/*! Constructs a BulkData object from the specified vector. Note that the
		vector is moved, so the memory ownership will be taken over by the
		new BulkData object.
		\param data			The vector containing the data.
	 */
	BulkData(std::vector<uint8_t> &&data);
#endif

	~BulkData();

	BulkData(const BulkData &c) noexcept;
	BulkData(const Sscf1::BulkChannel::Buffer &c) noexcept;
	BulkData & operator=(const BulkData &);
	BulkData & operator=(const Sscf1::BulkChannel::Buffer &);

	const uint8_t * data() const noexcept; //!< Returns a pointer to the first element in the data block.
	size_t size() const noexcept; //!< Returns the number of elements in the data block.
};

} // End of namespaces

#if defined(_MSC_VER)
# undef noexcept
#endif

#endif
