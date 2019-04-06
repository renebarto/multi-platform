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
# pragma warning (disable : 4464) // Suppress warning including relative headers
#endif

#include "sscf1/tcpsocket.h"
#include "sscf1/cipher.h"
#include "sscf1/system.h"
#include "../../include/sscf/commthread.h"
#include <algorithm>
#include <cassert>
#include <climits>
#include <cstring>
#include <vector>

#if defined(__unix__)
# include <netinet/tcp.h>
# include <netdb.h>
# include <ifaddrs.h>
# include <fcntl.h>
#elif defined(__VXWORKS__)
# include <netinet/tcp.h>
# include <ioLib.h>
# include <stdio.h>
# include <ifaddrs.h>
#endif

namespace Sscf1 {

struct TcpSocket::Private
{
	struct SendBuffer
    {
		const char *data;
		size_t size;
        size_t pos;
        SendCallback *callback;
		bool owner;
    };

	static const size_t blockSize = 65536;

	Private() 
	  :	commThread(CommThread::instance()), 
		socket(InvalidFileDesc),
		cipher(nullptr)
	{
	}

    std::recursive_mutex mutex;

    const std::shared_ptr<CommThread> commThread;
	IpAddress::Family family;
    FileDesc socket;
	IpAddress boundAddress;
	IpAddress remoteAddress;

    const Cipher *cipher;
    std::queue<SendBuffer> sendQueue;

	std::vector<char> receiveQueue;

    void resumeSend();

    void encrypt(const char * const *buffer, const int *len, int count, std::vector<char> &dest);
};

TcpSocket::TcpSocket(IpAddress::Family family, const Cipher &cipher)
  : p(new Private())
{
	p->family = family;
    p->cipher = &cipher;

	bool result = false;
	int lastErrorCode = 0;
    p->commThread->send([this, &result, &lastErrorCode]
	{
		int af = AF_UNSPEC;
		switch (p->family)
		{
		case IpAddress::Family::IPv4: af = AF_INET; break;
		case IpAddress::Family::IPv6: af = AF_INET6; break;
		}

#if !defined(_WIN32) || !defined(WSA_FLAG_NO_HANDLE_INHERIT)
		p->socket = ::socket(af, SOCK_STREAM, 0);
#else
		p->socket = ::WSASocketW(af, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_NO_HANDLE_INHERIT);
		if (p->socket == InvalidFileDesc)
			p->socket = ::socket(af, SOCK_STREAM, 0);
#endif

		if (p->socket != InvalidFileDesc)
		{
			const int on = 1;
			result = ::setsockopt(p->socket, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) == 0;
			if(!result) 
				lastErrorCode = lastError();

#if defined(__unix__)
			const int flags = ::fcntl(p->socket, F_GETFL, 0);
			if (flags >= 0)
				result &= ::fcntl(p->socket, F_SETFL, flags | O_NONBLOCK) != -1;
#elif defined (__VXWORKS__)
			int nonBlock = 1;
			result &= ::ioctl(p->socket, FIONBIO, (int)&nonBlock) == 0;
#elif defined(_WIN32)
            u_long nonBlock = 1;
			result &= ::ioctlsocket(p->socket, FIONBIO, &nonBlock) == 0;

			if(!result && lastErrorCode == 0) 
				lastErrorCode = lastError();
#endif
		}
	});

	if (!result)
		system().fail("Failed to create TCP socket (%d)", lastErrorCode); 
}

TcpSocket::TcpSocket(const Cipher &cipher, FileDesc s, IpAddress remoteAddress)
  : p(new Private())
{
    p->socket = s;
	p->remoteAddress = std::move(remoteAddress);
    p->cipher = &cipher;

	bool result = false;
	int lastErrorCode = 0;
    p->commThread->send([this, &result, &lastErrorCode]
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
		if(!result ) 
			lastErrorCode = lastError();
	});

	if (!result)
		system().fail("Failed to enable non-blocking I/O (%d)", lastErrorCode); 
}

TcpSocket::~TcpSocket()
{
    TcpSocket::close();

    delete p;
    *const_cast<Private **>(&p) = nullptr;
}

void TcpSocket::close(void)
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

		std::unique_lock<std::recursive_mutex> l(p->mutex);

		while (!p->sendQueue.empty())
		{
			const Private::SendBuffer buf = p->sendQueue.front();
			p->sendQueue.pop();

			l.unlock();

			if (buf.callback)
				buf.callback->sendClosed();

			if (buf.owner)
				delete [] buf.data;

			l.lock();
		}
	});
}

bool TcpSocket::enableKeepAlive(int timeout)
{
	bool result = false;
    p->commThread->send([this, timeout, &result]
	{
#if defined(__unix__)
        const int enabled = 1;
        const int count = 5;
        const int tosecs = (timeout + 500) / 1000;

        result =
			(::setsockopt(p->socket, SOL_SOCKET, SO_KEEPALIVE, (char*)&enabled, sizeof(enabled)) == 0) &&
            (::setsockopt(p->socket, IPPROTO_TCP, TCP_KEEPINTVL, (char*)&tosecs, sizeof(tosecs)) == 0) &&
            (::setsockopt(p->socket, IPPROTO_TCP, TCP_KEEPIDLE, (char*)&tosecs, sizeof(tosecs)) == 0) &&
			(::setsockopt(p->socket, IPPROTO_TCP, TCP_KEEPCNT, (char*)&count, sizeof(count)) == 0);
#elif defined(_WIN32)
        DWORD bytes = 0;

		tcp_keepalive keepalive;
		keepalive.onoff = 1; // On.
		keepalive.keepalivetime = static_cast<ULONG>(timeout); // Timeout in ms of no inactivity before sending keep-alive packets.
		keepalive.keepaliveinterval = static_cast<ULONG>(timeout); // Interval in ms between successive keep-alive packets.

		const int rc = ::WSAIoctl(
			p->socket, SIO_KEEPALIVE_VALS, 
			&keepalive, sizeof(keepalive),
			nullptr, 0, &bytes, nullptr, nullptr);

		result = rc != SOCKET_ERROR;
#endif
	});

    return result;
}

bool TcpSocket::enableNoDelay(void)
{
	bool result = false;
    p->commThread->send([this, &result]
	{
		const int enabled = 1;
		result = ::setsockopt(p->socket, IPPROTO_TCP, TCP_NODELAY, (char *)&enabled, sizeof(enabled)) == 0;
	});

    return result;
}

bool TcpSocket::setBufferSize(uint32_t send, uint32_t receive)
{
	bool result = false;
    p->commThread->send([this, send, receive, &result]
	{
		result = (::setsockopt(p->socket, SOL_SOCKET, SO_SNDBUF, (char *)&send,    sizeof(send))    == 0) &&
				 (::setsockopt(p->socket, SOL_SOCKET, SO_RCVBUF, (char *)&receive, sizeof(receive)) == 0);

		// check if the size were actually set.
        int actualSend = 0;
        int actualRecv = 0;

        socklen_t lenSend = sizeof(actualSend);
        socklen_t lenRecv = sizeof(actualRecv);

		result &= (::getsockopt(p->socket, SOL_SOCKET, SO_SNDBUF, (char *)&actualSend, &lenSend) == 0) &&
				  (::getsockopt(p->socket, SOL_SOCKET, SO_RCVBUF, (char *)&actualRecv, &lenRecv) == 0);

        result &= (lenSend == sizeof(actualSend)) && (lenRecv == sizeof(actualRecv)) &&
                  (actualSend >= int(send)) && (actualRecv >= int(receive));
	});

    return result;
}

bool TcpSocket::bind(BindCallback *callback, const IpAddress &address)
{
	bool result = false;

	p->commThread->send([this, callback, address, &result]
	{
		if (::bind(p->socket, address.sockaddr(), socklen_t(address.addrlen())) != -1)
		{
			if (callback)
			{
				if (::listen(p->socket, SOMAXCONN) != -1)
				{
					p->commThread->setReadEvent(p->socket, [this, callback]
					{
						IpAddress remoteAddress;
						socklen_t addrSize = sizeof(sockaddr_storage);

						const FileDesc s = ::accept(p->socket, remoteAddress.sockaddr(), &addrSize);
						if (s != InvalidFileDesc)
						{
							system().traceMsg(
								"TcpSocket: Incoming connection on port %d, from %s", 
								p->boundAddress.port(), 
                                remoteAddress.toString().c_str());

							if (callback)
							{
								callback->bindAccepted(new TcpSocket(*p->cipher, s, remoteAddress));
							}
							else
							{
#if defined(__unix__)
								::close(s);
#elif defined(_WIN32)
								::closesocket(s);
#endif
							}
						}
						else
							system().traceMsg("TcpSocket: Accept failed on port %d with lastError %d", p->boundAddress.port(), lastError());
					});

					// Read the actually bound address and port number.
					socklen_t addrSize = sizeof(sockaddr_storage);
					if (::getsockname(p->socket, p->boundAddress.sockaddr(), &addrSize) == 0)
					{
						system().traceMsg(
							"TcpSocket: Bound %s",
                            p->boundAddress.toString().c_str());
					}
					else
					{
						system().traceMsg(
							"TcpSocket: Failed to get socket name %s with lastError %d", 
							p->boundAddress.toString().c_str(), lastError());
					}

					result = true;
				}
			}
			else // No callback; only bind, no listen.
				result = true;
		}

		if (!result)
		{
			system().traceMsg(
				"TcpSocket: Failed to bind %s with lastError %d", 
				address.toString().c_str(), lastError());
		}
	});

    return result;
}

const IpAddress & TcpSocket::address(void) const
{
	return p->boundAddress;
}

bool TcpSocket::connect(ConnectCallback *callback, IpAddress address)
{
	bool result = false;

	p->remoteAddress = std::move(address);

    p->commThread->send([this, callback, &result]
	{
		system().traceMsg(
			"TcpSocket: Connect to %s", 
            p->remoteAddress.toString().c_str());

		const int rc = ::connect(p->socket, p->remoteAddress.sockaddr(), socklen_t(p->remoteAddress.addrlen()));
		if ((rc == -1) && errorIoPending())
		{
			p->commThread->setWriteEvent(p->socket, [this, callback]
			{
				int val = 1;
				socklen_t len = sizeof(val);

				if (::getsockopt(p->socket, SOL_SOCKET, SO_ERROR, (char *)&val, &len) != 0)
					val = -1;

				system().traceMsg(
					"TcpSocket: Connect to %s finished with SO_ERROR %d", 
                    p->remoteAddress.toString().c_str(),
					val);

				if (callback)
					callback->connectFinished(val == 0);
			});
			
			p->commThread->setErrorEvent(p->socket, [this, callback]
			{
				int val = 1;
				socklen_t len = sizeof(val);

				if (::getsockopt(p->socket, SOL_SOCKET, SO_ERROR, (char *)&val, &len) != 0)
					val = -1;

				system().traceMsg(
					"TcpSocket: Connect to %s failed with SO_ERROR %d", 
                    p->remoteAddress.toString().c_str(),
					val);

				if (callback)
					callback->connectFinished(false);
			});

			result = true;
		}
		else if (rc == 0)
		{
			system().traceMsg(
				"TcpSocket: Connect to %s finished with rc 0", 
                p->remoteAddress.toString().c_str());

			if (callback)
				callback->connectFinished(true);

			result = true;
		}
		else
		{
			system().traceMsg(
				"TcpSocket: Connect to %s failed with lastError %d", 
                p->remoteAddress.toString().c_str(),
				lastError());
		}
	});

    return result;
}

void TcpSocket::setCipher(const Cipher &cipher)
{
    p->cipher = &cipher;
}

void TcpSocket::beginSend(SendCallback *callback, const char *buffer, size_t len)
{
    if (buffer)
    {
        if (len <= 0)
            len = ::strlen(buffer);

        beginSend(callback, &buffer, &len, 1);
    }
}

void TcpSocket::beginSend(SendCallback *callback, const char * const *buffer, const size_t *len, int count)
{
    std::lock_guard<std::recursive_mutex> _(p->mutex);

    assert(p->socket != InvalidFileDesc);

    size_t totalSize = 0, totalEncryptedSize = 0;
    for (int i=0; i<count; i++)
	{
        totalSize += len[i];
        totalEncryptedSize += p->cipher->encryptBufferSize(len[i]);
	}

	char * const encryptBuffer = new char[totalEncryptedSize];
	size_t encryptedSize = 0;
    for (int i=0; i<count; i++)
	{
		size_t size = totalEncryptedSize - encryptedSize;
        p->cipher->encrypt(buffer[i], len[i], encryptBuffer + encryptedSize, size);
		encryptedSize += size;
	}

	if (encryptedSize > 0)
	{
		Private::SendBuffer buf;
		buf.data = encryptBuffer;
		buf.size = encryptedSize;
		buf.owner = true;
		buf.pos = 0;
		buf.callback = callback;

		const bool restart = p->sendQueue.empty();
		p->sendQueue.push(buf);
		if (restart)
			p->commThread->post(std::bind(&Private::resumeSend, p));
	}
	else
		delete [] encryptBuffer;
}

void TcpSocket::beginSendBulk(SendCallback *callback, const char *buffer, size_t len)
{
	assert(p->socket != InvalidFileDesc);

	if (len > 0)
	{
		std::lock_guard<std::recursive_mutex> _(p->mutex);

		Private::SendBuffer buf;
		buf.data = buffer;
		buf.size = len;
		buf.owner = false;
		buf.pos = 0;
		buf.callback = callback;

		const bool restart = p->sendQueue.empty();
		p->sendQueue.push(buf);
		if (restart)
			p->commThread->post(std::bind(&Private::resumeSend, p));
	}
}

void TcpSocket::beginReceive(ReceiveCallback *callback)
{
    p->commThread->setReadEvent(p->socket, [this, callback]
	{
		const size_t pos = p->receiveQueue.size();
		p->receiveQueue.resize(pos + Private::blockSize);

        size_t size = static_cast<size_t>(::recv(p->socket, &(p->receiveQueue[pos]), Private::blockSize, 0));
        if ((size == 0) || ((size == size_t(-1)) && !errorIoPending()))
        {
			auto lastErrorCode = lastError();
			p->receiveQueue.clear();
			p->commThread->removeEvents(p->socket);

			system().traceMsg(
				"TcpSocket: Connection to %s was closed with lastError %d", 
                p->remoteAddress.toString().c_str(),
				(size == size_t(-1)) ? lastErrorCode : 0);

			if (callback)
				callback->receiveClosed();
		}
        else if ((size != size_t(-1)) && callback)
		{
			p->receiveQueue.resize(pos + size);

			std::vector<char> output;
            size_t outputSize = p->cipher->decryptBufferSize(p->receiveQueue.size());
			output.resize(outputSize);
            const size_t read = p->cipher->decrypt(&(p->receiveQueue[0]), p->receiveQueue.size(), &(output[0]), outputSize);
			if (outputSize > 0)
				callback->receiveReceived(&(output[0]), outputSize);

			if (read < p->receiveQueue.size())
			{
				memmove(&(p->receiveQueue[0]), &(p->receiveQueue[read]), p->receiveQueue.size() - read);
				p->receiveQueue.resize(p->receiveQueue.size() - read);
			}
			else
				p->receiveQueue.clear();
		}
        else
        {
            auto lastErrorCode = lastError();
            system().traceMsg(
                "TcpSocket: Connection to %s recv call did not return any data, lastError %d",
                p->remoteAddress.toString().c_str(),
                lastErrorCode);
            p->receiveQueue.resize(pos); // resize back to the original buffer size as no data was received
        }
	});
}

void TcpSocket::beginReceiveBulk(ReceiveBulkCallback *callback)
{
	p->commThread->setReadEvent(p->socket, [this, callback]
	{
		if (callback)
		{
			char *buffer = nullptr;
			size_t bufferLen = 0;
			callback->acquireBuffer(buffer, bufferLen);
			assert(buffer);
			assert(bufferLen);

			size_t size = static_cast<size_t>(::recv(p->socket, buffer, int(bufferLen), 0));
			if ((size == 0) || ((size == size_t(-1)) && !errorIoPending()))
			{
				auto lastErrorCode = lastError();
				p->commThread->removeEvents(p->socket);

				system().traceMsg(
					"TcpSocket: Bulk Connection to %s was closed with lastError %d", 
                    p->remoteAddress.toString().c_str(),
					(size == size_t(-1)) ? lastErrorCode : 0);

				callback->receiveReceived(buffer, 0);
				callback->receiveClosed();
			}
			else if (size != size_t(-1))
				callback->receiveReceived(buffer, size);
			else
				callback->receiveReceived(buffer, 0);
		}
	});
}

TcpSocket::BindCallback::BindCallback()
{
}

TcpSocket::BindCallback::~BindCallback()
{
}

TcpSocket::ConnectCallback::ConnectCallback()
{
}

TcpSocket::ConnectCallback::~ConnectCallback()
{
}

TcpSocket::SendCallback::SendCallback()
{
}

TcpSocket::SendCallback::~SendCallback()
{
}

TcpSocket::ReceiveCallback::ReceiveCallback()
{
}

TcpSocket::ReceiveCallback::~ReceiveCallback()
{
}

void TcpSocket::Private::resumeSend()
{
	std::unique_lock<std::recursive_mutex> l(mutex);

    static const int flags =
#if defined(__linux__)
            MSG_NOSIGNAL;
#else
            0;
#endif

    while (!sendQueue.empty())
    {
        assert(socket != InvalidFileDesc);

        SendBuffer &buf = sendQueue.front();
		const int send = int(std::min(buf.size - buf.pos, size_t(INT_MAX)));
        size_t sent = static_cast<size_t>(::send(socket, buf.data + buf.pos, send, flags));
        if (sent == size_t(-1))
        {
			auto lastErrorCode = lastError();
			if (!errorIoPending())
            {
                while (!sendQueue.empty())
                {
                    const SendBuffer sb = sendQueue.front();
                    sendQueue.pop();

                    l.unlock();

                    if (sb.callback)
						sb.callback->sendClosed();

					if (sb.owner)
						delete [] sb.data;

                    l.lock();
                }

                commThread->removeEvents(socket);

				system().traceMsg(
					"TcpSocket: Connection to %s was closed unexpectedly with lastError %d", 
                    remoteAddress.toString().c_str(),
                    lastErrorCode);
            }
            else
                commThread->setWriteEvent(socket, std::bind(&Private::resumeSend, this));

            break;
        }
        else
            buf.pos += sent;

        if (buf.pos >= buf.size)
        {
            const SendBuffer sb = sendQueue.front();
            sendQueue.pop();

            l.unlock();

            if (sb.callback)
				sb.callback->sendSent(&(sb.data[0]), sb.size);

			if (sb.owner)
				delete [] sb.data;

            l.lock();
        }
    }
}

void TcpSocket::Private::encrypt(const char * const *buffer, const int *len, int count, std::vector<char> &dest)
{
    size_t totalSize = 0;
    for (int i=0; i<count; i++)
        totalSize += len[i];

    dest.resize(totalSize);

    size_t pos = 0;
    for (int i=0; i<count; i++)
	if (len[i] > 0)
    {
        ::memcpy(&(dest[pos]), buffer[i], static_cast<size_t>(len[i]));
        pos += len[i];
    }
}

} // End of namespaces
