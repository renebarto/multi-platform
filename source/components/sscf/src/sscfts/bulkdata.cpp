/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#include "sscfts1/bulkdata.h"
#include "sscf1/bulkchannel.h"
#include <atomic>
#include <cassert>

#if defined(_MSC_VER)
# define noexcept throw()
#endif

namespace SscfTs1 {

struct Cookie 
{ 
	Cookie(IEventLoop &eventLoop, std::function<void()> &&released)
	  : eventLoop(eventLoop), released(std::move(released))
	{
	}

	Cookie &operator=(const Cookie &);

	IEventLoop &eventLoop; 
	std::function<void()> released; 
};

static void release(void *p) 
{ 
	Cookie * const cookie = reinterpret_cast<Cookie *>(p);
	if (cookie->released)
		cookie->eventLoop.post(std::move(cookie->released));

	delete cookie;
}

BulkData::BulkData()
  : Sscf1::BulkChannel::Buffer()
{
}

BulkData::BulkData(const uint8_t *data, size_t size, IEventLoop &eventLoop, std::function<void()> released)
  : Sscf1::BulkChannel::Buffer(data, size, &release, new Cookie(eventLoop, std::move(released)))
{
}

static void deleteVector(void *p) 
{ 
	delete reinterpret_cast<std::vector<uint8_t> *>(p);
}

static Sscf1::BulkChannel::Buffer createFromVector(std::vector<uint8_t> &&data)
{
	std::vector<uint8_t> * const v = new std::vector<uint8_t>(std::move(data));

	return Sscf1::BulkChannel::Buffer(v->data(), v->size(), &deleteVector, v);
}

BulkData::BulkData(std::vector<uint8_t> &&data)
  : Sscf1::BulkChannel::Buffer(createFromVector(std::move(data)))
{
}

BulkData::BulkData(const BulkData &c) noexcept
  : Sscf1::BulkChannel::Buffer(c)
{
}

BulkData::BulkData(const Sscf1::BulkChannel::Buffer &c) noexcept
  : Sscf1::BulkChannel::Buffer(c)
{
}

BulkData::~BulkData()
{
}

BulkData & BulkData::operator=(const BulkData &c)
{
	Sscf1::BulkChannel::Buffer::operator=(c);

	return *this;
}

BulkData & BulkData::operator=(const Sscf1::BulkChannel::Buffer &c)
{
	Sscf1::BulkChannel::Buffer::operator=(c);

	return *this;
}

const uint8_t * BulkData::data() const noexcept
{
	return Sscf1::BulkChannel::Buffer::data();
}

size_t BulkData::size() const noexcept
{
	return Sscf1::BulkChannel::Buffer::size();
}

} // End of namespaces
