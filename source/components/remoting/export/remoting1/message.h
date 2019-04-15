/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifndef REMOTING1_MESSAGE_H
#define REMOTING1_MESSAGE_H

#include <cstdint>

// For C++/CLI <vector> gives LNK4210: .CRTMP section exists; there may be unhandled static initializers or terminators
#ifndef _M_CEE
# include <vector>
#endif
#include "remoting/exports.h"

#if defined(_MSC_VER)
# define noexcept throw()
#endif

namespace Remoting1 {

/*! This class formats an HTTP 1.1 message. An HTTP message consists of a text
    formatted header and a binary content.
 */
class REMOTING_EXPORT Message
{
public:
    Message(void);

    /*! Constructs a message by parsing the provided buffer. When len = 0 the
        buffer is interpreted as a zero terminated string.
     */
    explicit Message(const char *data, int len = 0);

    /*! Constructs an empty response message with the provided status code.
     */
    explicit Message(int status);

#ifndef _M_CEE // For C++/CLI <vector> gives LNK4210
    /*! Constructs a response message with the provided status code and
        content.
     */
	Message(int status, const std::vector<uint8_t> &content);

    /*! Constructs a response message with the provided status code and
        content.
     */
	Message(int status, std::vector<uint8_t> &&content);
#endif

    /*! Constructs an empty request message with the provided method and path.
        \param method   For example: GET, POST, TRACE, OPTIONS, NOTIFY, etc.
        \param path     For example: "/".
     */
    Message(const char *method, const char *path);

#ifndef _M_CEE // For C++/CLI <vector> gives LNK4210
    /*! Constructs a request message with the provided method and path and
        content.
     */
	Message(const char *method, const char *path, const std::vector<uint8_t> &content);

    /*! Constructs a response message with the provided status code and
        content.
     */
	Message(const char *method, const char *path, std::vector<uint8_t> &&content);
#endif

    Message(const Message &);
    Message(Message &&) noexcept;
	~Message();

    Message & operator=(const Message &);
    Message & operator=(Message &&) noexcept;

    /*! Parses the provided buffer. When len = 0 the buffer is interpreted as a
        zero terminated string.
     */
    int parse(const char *data, int len = 0);

    /*! Parses an URL and fills the provided host and port.
     */
    static bool parseUrl(const char *url, char (& host)[256], uint16_t &port);

    const char * requestMethod(void) const; //!< Returns the request method. (i.e. GET, POST, etc.)
    void setRequestMethod(const char *method); //!< Sets the request method. (i.e. GET, POST, etc.)
    const char * requestPath(void) const; //!< Returns the request path
    void setRequestPath(const char *path); //!< Sets the request path
    int responseStatus(void) const; //!< Returns the response status (200 = OK)
    void setResponseStatus(int status); //!< Sets the response status (200 = OK)

    const char * field(const char *name, unsigned index = 0) const; //!< Returns the value of the specified field, or nullptr if not present.
    void setField(const char *name, const char *value); //!< Sets the value of the specified field, nullptr removes it.
	void addField(const char *name, const char *value); //!< Adds an extra instance of the specified field.

    const char * header(void) const; //!< Returns the formatted header text.

#ifndef _M_CEE // For C++/CLI <vector> gives LNK4210
	const std::vector<uint8_t> & content(void) const; //!< Returns a reference to the the content.
	std::vector<uint8_t> & content(void); //!< Returns a reference to the the content.
#endif

#if defined(_M_CEE) || defined(REMOTING1_MESSAGE_CPP) // For C++/CLI <vector> gives LNK4210; these methods are replacements
	void clrResizeContent(int newLen);
	void * clrContentPtr();
	int clrContentLen();
#endif

private:
    struct Private;
    Private * const p;
};

} // End of namespaces

#if defined(_MSC_VER)
# undef noexcept
#endif

#endif
