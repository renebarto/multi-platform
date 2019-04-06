/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifndef SSCF1_COMMTHREAD_H
#define SSCF1_COMMTHREAD_H

//#define VALUE_TO_STRING(x) #x
//#define VALUE(x) VALUE_TO_STRING(x)
//#define VAR_NAME_VALUE(var) #var "="  VALUE(var)

#include <chrono>
#include <cstdint>
#include <map>
#include <memory>
#include <queue>
#include <vector>
#ifdef _MSC_VER
# pragma warning (disable : 4265) // Suppress warning from Microsoft headers
#endif
#include <functional>
#include <condition_variable>
#include <mutex>
#include <string>
#include <thread>
#ifdef _MSC_VER
# pragma warning (default : 4265)
#endif
#if defined(__unix__)
# include <arpa/inet.h>
# include <sys/ioctl.h>
# include <sys/select.h>
# include <sys/socket.h>
# include <sys/time.h>
# include <netinet/in.h>
# include <unistd.h>
#elif defined(__VXWORKS__)
# include <arpa/inet.h>
# include <sys/ioctl.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <sockLib.h>
#elif defined(_WIN32)
#if defined(_WINDOWS_) || defined(_WINBASE_) || defined(_WINNT_) || defined(_WINSOCK2API_)
#  error Do not include windows.h before ws2tcpip.h, see: http://support.microsoft.com/kb/257460
#endif
# define WIN32_LEAN_AND_MEAN
# define FD_SETSIZE (256)	// By default the max number of sockets in the set is 64
# ifdef MSC_VER
#  pragma warning(push)
#  pragma warning(disable: 4365)
#  pragma warning(disable: 4514)
#  pragma warning(disable: 4574)
#  pragma warning(disable: 4668)
# endif
# include <winsock2.h>
# include <ws2tcpip.h> // Link with ws2_32.lib
# include <mswsock.h> // Link with mswsock.lib
# include <mstcpip.h>
//#pragma message(VAR_NAME_VALUE(_WIN32_WINNT))
# if (_WIN32_WINNT < 0x0600)
#  error SSCF requires Windows Vista SDK or later; set _WIN32_WINNT >= 0x600
# endif
# undef min
# undef max
# ifdef MSC_VER
#  pragma warning(pop)
# endif
#endif

namespace Sscf1 {

#if defined(__unix__)
typedef int FileDesc;
static const FileDesc InvalidFileDesc = -1;
#elif defined(_WIN32)
typedef SOCKET FileDesc;
static const FileDesc InvalidFileDesc = INVALID_SOCKET;
#endif

int lastError();
bool errorIoPending();

/*! The communication thread singleton handles all asynchronous communication
    for all sockets.
 */
class CommThread
{
public:
	/*! A UUID-based host ID.
	 */
	struct HostId
	{
		HostId();
		HostId(const uint8_t(&data)[16]);
        HostId(const char *); //!< Converts from "uuid:xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"
		HostId(const std::string &); //!< Converts from "uuid:xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"

		HostId & operator=(const HostId &);
		HostId & operator=(const uint8_t(&data)[16]);
		bool operator<(const HostId &) const;
		operator std::string() const; //!< Converts to "uuid:xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"

		uint8_t data[16];
	};

public:
	/*! Returns an instance to the CommThread singleton.
	 */
    static std::shared_ptr<CommThread> instance();

    /*! The server ID in the form of: "<OS name> <UPnP version> <SSCF version>".
     */
    static const char serverId[];

	/*! Generates a host id.
	 */
	static HostId generateHostId();

	/*! Returns the file name of the executing module.
	*/
	const std::string & moduleName() const;

	/*! Returns true if invoked from the communication thread.
	 */
    bool onCommThread() const;

	/*! Asynchronously performs the specified action on the communication
		thread.
	 */
    void post(std::function<void()> &&);

	/*! Synchronously performs the specified action on the communication
		thread.
	 */
    void send(std::function<void()> &&);


	/*! Performs the specified action on the communication thread when the
		specified file descriptor is ready to read.
	 */
    void setReadEvent(FileDesc, std::function<void()> &&);

	/*! Performs the specified action on the communication thread when the
		specified file descriptor is ready to write.
	 */
    void setWriteEvent(FileDesc, std::function<void()> &&);

	/*! Performs the specified action on the communication thread when the
		specified file descriptor has an error.
	 */
    void setErrorEvent(FileDesc, std::function<void()> &&);

	/*! Removes all events associated with the specified file descriptor.
	 */
    void removeEvents(FileDesc);


	/*! Performs the specified action on the communication thread after the
		specified timeout has occurred.
	 */
    void setTimerEvent(void *, const std::chrono::milliseconds &, std::function<void()> &&);

	/*! Removes all timer events associated with the specified file handle.
	 */
	void removeEvents(void *);

private:
    CommThread();
    CommThread(const CommThread &);
    ~CommThread();
    CommThread & operator=(const CommThread &);

    void handlePipeEvent();
    void socketThreadFunc();

#if defined(_WIN32) || defined(__VXWORKS__)
    static int createPipe(FileDesc (&)[2]);
#endif

private:
	std::string currentModuleName;

    std::mutex mutex;

	FileDesc pipe[2]; // pipe[0] = read, pipe[1] = write;

    std::queue<std::tuple<std::function<void()>, bool *, std::condition_variable *>> pipeEventQueue;

	std::map<FileDesc, std::shared_ptr<std::function<void()>>> readEvents;
	std::map<FileDesc, std::function<void()>> writeEvents, errorEvents;

	// Bug in VS2012 and VS2013 implementation of std::chrono::steady_clock.
	// See also: https://connect.microsoft.com/VisualStudio/feedback/details/753115/
#if !defined(_MSC_VER) || (_MSC_VER >= 1900)
    typedef std::vector<std::pair<std::chrono::steady_clock::time_point, std::function<void()>>> TimerEvents;
#else
    typedef std::vector<std::pair<uint64_t, std::function<void()>>> TimerEvents;
#endif
    std::map<void *, TimerEvents> timerEventMap;
    ::fd_set readSet, writeSet, errorSet;

    bool threadRunning;
#if !defined(_WIN32)
    std::unique_ptr<std::thread> thread;
#else
	HANDLE thread;
	DWORD threadId;
#endif
};

} // End of namespaces

#endif
