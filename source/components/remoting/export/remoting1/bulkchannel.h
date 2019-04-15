/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2014, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifndef REMOTING1_BULKCHANNEL_H
#define REMOTING1_BULKCHANNEL_H

#include <cstdint>
#include <cstdlib>
#include "ipaddress.h"
#include "remoting/exports.h"

#if defined(_MSC_VER)
# define noexcept throw()
#endif

namespace Remoting1 {

class Cipher;
class TcpSocket;

/*! This class is used for high-speed data transfers.
 */
class REMOTING_EXPORT BulkChannel
{
public:
#ifndef REMOTING1_BULKCHANNEL_CPP
    struct BulkId { uint8_t data[28]; };
#else
    struct BulkId;
#endif

    class Buffer
    {
    public:
        Buffer();

        Buffer(const uint8_t *data, size_t size, void(*released)(void *cookie), void *cookie);
        ~Buffer();

        Buffer(const Buffer &c) noexcept;
        Buffer & operator=(const Buffer &);

        const uint8_t * data() const noexcept; //!< Returns a pointer to the first element in the data block.
        size_t size() const noexcept; //!< Returns the number of elements in the data block.

    private:
        void deref();

    private:
        struct Private;
        Private *p;
    };

    class Allocator
    {
    public:
        Allocator(BulkChannel &);
        virtual ~Allocator();

        /*! This method is invoked when a buffer needs to be allocated.
        */
        virtual Buffer allocate(size_t) = 0;

    private:
        Allocator(const Allocator &);
        Allocator & operator=(const Allocator &);

        BulkChannel &bulkChannel;
    };

public:
    /*! Creates a BulkChannel with the specified cipher and hostId
     */
    BulkChannel(const Cipher &cipher, const uint8_t(&hostId)[16]);

    ~BulkChannel();

    /*! Sets the address where the server can be found. If no connection is
        present, the BulkChannel will connect there.
     */
    void setServerAddress(IpAddress address, IpAddress network);

    /*! Adds a socket with a connection to the specified hostId.
     */
    void addSocket(TcpSocket *, const uint8_t(&hostId)[16], size_t numSockets);

    /*! Queues a Buffer for retrieval, the returned BulkId can be used to
        retrieve the Buffer.
     */
    BulkId enqueue(const Buffer &);

    /*! Adds a recipient for a buffer.
     */
    void addRecipient(const BulkId &);

    /*! Adds recipients for a buffer.
     */
    void addRecipients(const BulkId *, size_t);

    /*! Retrieves the Buffer with the associated BulkId.
     */
    Buffer retrieve(const BulkId &);

    /*! Discards buffer.
     */
    void discard(const BulkId &);

    /*! Discards buffers.
     */
    void discard(const BulkId *, size_t);

private:
    BulkChannel(const BulkChannel &);
    BulkChannel & operator=(const BulkChannel &);

private:
    struct Client;
    struct Private;
    Private * const p;
};

} // End of namespaces

#if defined(_MSC_VER)
# undef noexcept
#endif

#endif
