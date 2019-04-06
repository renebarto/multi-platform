/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifdef _MSC_VER
# define _CRT_SECURE_NO_WARNINGS 1
# define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#endif

#define SSCF1_MESSAGE_CPP
#include "sscf1/message.h"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <iomanip>
#include <locale>
#include <map>
#include <sstream>
#include <string>

#if defined(_MSC_VER)
# define noexcept throw()
#endif

#ifdef _MSC_VER
# define strcasecmp _stricmp
#endif

namespace Sscf1 {

struct Message::Private
{
    Private() { }
    Private(const std::vector<uint8_t> &content) : content(content) { }
    Private(std::vector<uint8_t> &&content) : content(content) { }

    // Case-insensitive less.
    struct less : std::binary_function<std::string, std::string, bool> 
    {
        bool operator()(const std::string &a, const std::string &b) const noexcept
        {
            return strcasecmp(a.c_str(), b.c_str()) < 0;
        }
    };

    std::string statusLine[3];
    std::string requestPath;
    std::map<std::string, std::vector<std::string>, less> fields;
    std::string header;
    std::vector<uint8_t> content;
};

Message::Message(void)
  : p(new Private())
{
}

Message::Message(const char *data, int len)
  : p(new Private())
{
    parse(data, len);
}

Message::Message(int status)
  : p(new Private())
{
    setResponseStatus(status);
}

Message::Message(int status, const std::vector<uint8_t> &content)
  : p(new Private(content))
{
    setResponseStatus(status);
}

Message::Message(int status, std::vector<uint8_t> &&content)
  : p(new Private(content))
{
    setResponseStatus(status);
}

Message::Message(const char *method, const char *path)
  : p(new Private())
{
    setRequestMethod(method);
    setRequestPath(path);
}

Message::Message(const char *method, const char *path, const std::vector<uint8_t> &content)
  : p(new Private(content))
{
    setRequestMethod(method);
    setRequestPath(path);
}

Message::Message(const char *method, const char *path, std::vector<uint8_t> &&content)
  : p(new Private(content))
{
    setRequestMethod(method);
    setRequestPath(path);
}

Message::Message(const Message &from)
  : p(new Private(*(from.p)))
{
}

Message::Message(Message &&from) noexcept
  : p(nullptr)
{
    std::swap(*const_cast<Private **>(&p), *const_cast<Private **>(&from.p));
}

Message::~Message()
{
    delete p;
    *const_cast<Private **>(&p) = nullptr;
}

Message & Message::operator=(const Message &from)
{
    delete p;
    *const_cast<Private **>(&p) = new Private(*(from.p));

    return *this;
}

Message & Message::operator=(Message &&from) noexcept
{
    delete p;
    *const_cast<Private **>(&p) = from.p;
    *const_cast<Private **>(&from.p) = nullptr;

    return *this;
}

int Message::parse(const char *data, int len)
{
    if (len <= 0)
        len = int(::strlen(data));

    p->statusLine[0].clear();
    p->statusLine[1].clear();
    p->statusLine[2].clear();
    p->requestPath.clear();
    p->fields.clear();
    p->content.clear();
    p->header.clear();

    for (int i=0; i<len; )
    {
        const char * const el = reinterpret_cast<const char *>(::memchr(data + i, '\r', static_cast<size_t>(len - i)));
        if (el && (el[1] == '\n'))
        {
            const size_t n = size_t(el) - size_t(data + i);
            if (n > 0)
            {
                if (p->statusLine[0].length() > 0)
                {
                    // Parse field.
                    const char * const cl = reinterpret_cast<const char *>(::memchr(data + i, ':', static_cast<size_t>(len - i)));
                    if (cl)
                    {
                        const size_t nn = size_t(cl) - size_t(data + i);
                        if ((nn > 0) && (nn < n))
                        {
                            size_t ofs = 1;
                            while ((ofs < n - nn) && std::isspace(cl[ofs]))
                                ofs++;

                            p->fields[std::string(data + i, nn)].push_back(std::string(cl + ofs, n - nn - ofs));
                        }
                    }
                }
                else
                {
                    // Parse status line.
                    const char * const s1 = reinterpret_cast<const char *>(::memchr(data + i, ' ', n));
                    if (s1)
                    {
                        const size_t n1 = size_t(s1) - size_t(data + i);
                        if ((n1 > 0) && (n1 < n))
                            p->statusLine[0] = std::string(data + i, n1);

                        const char * const s2 = reinterpret_cast<const char *>(::memchr(s1 + 1, ' ', n - (n1 + 1)));
                        const size_t n2 = size_t(s2) - size_t(s1 + 1);
                        if (s2 && (n2 > 0) && (n2 < n - n1))
                        {
                            p->statusLine[1] = std::string(s1 + 1, n2);
                            p->statusLine[2] = std::string(s2 + 1, n - (n1 + n2 + 2));
                        }
                        else
                            p->statusLine[1] = std::string(s1 + 1, n - (n1 + 1));

                        std::ostringstream fp;
                        fp.imbue(std::locale::classic());
                        const std::string path = p->statusLine[1];
                        for (size_t j = 0; j < path.size(); j++)
                        if (path[j] == '%')
                        {
                            int code = 0;   
                            std::stringstream s;
                            s << std::hex << path.substr(j + 1, 2);
                            s >> code;

                            fp << char(code);

                            j += 2;
                        }
                        else
                            fp << path[j];

                        p->requestPath = fp.str();
                    }
                }

                i += int(n + 2);
                continue;
            }
            else
                i += 2;
        }

        // Check if the HTTP header is complete (i.e. it ends with \r\n\r\n).
        if ((i <= len) && (i >= 4) && (::memcmp(data + i - 4, "\r\n\r\n", 4) == 0))
        {
            const char * const contentLenField = field("Content-Length");
            if (contentLenField)
            {
                const int contentLen = ::atoi(contentLenField);
                if ((len - i) >= contentLen)
                {
                    if (contentLen > 0)
                    {
                        p->content.resize(static_cast<uint64_t>(contentLen));
                        ::memcpy(&(p->content[0]), data + i, static_cast<size_t>(contentLen));
                    }

                    return i + contentLen;
                }
            }
            else
                return i;
        }

        break;
    }

    return -1;
}

bool Message::parseUrl(const char *url, char (& host)[256], uint16_t &port)
{
    const char * const css = ::strstr(url, "://");
    if (css != nullptr)
    {
        const char * const bro = ::strchr(css + 3, '[');
        const char * const brc = bro ? ::strchr(bro, ']') : nullptr;
        const char * const col = ::strchr(brc ? (brc + 1) : (css + 3), ':');
        if (col != nullptr)
        {
            if (brc != nullptr)
            {
                const size_t len = std::min(size_t(brc) - size_t(bro + 1), sizeof(host) - 1);
                ::strncpy(host, bro + 1, len);
                host[len] = '\0';
            }
            else
            {
                const size_t len = std::min(size_t(col) - size_t(css + 3), sizeof(host) - 1);
                ::strncpy(host, css + 3, len);
                host[len] = '\0';
            }

            const char * const sl = ::strchr(col, '/');
            if (sl != nullptr)
            {
                char p[16];
                const size_t ln = std::min(size_t(sl) - size_t(col + 1), sizeof(p) - 1);
                ::strncpy(p, col + 1, ln);
                p[ln] = '\0';

                port = uint16_t(::atoi(p));
                return true;
            }
        }
        else
        {
            if (brc != nullptr)
            {
                const size_t len = std::min(size_t(brc) - size_t(bro + 1), sizeof(host) - 1);
                ::strncpy(host, bro + 1, len);
                host[len] = '\0';
            }
            else
            {
                const char * const sl = ::strchr(css + 3, '/');
                if (sl != nullptr)
                {
                    const size_t len = std::min(size_t(sl) - size_t(css + 3), sizeof(host) - 1);
                    ::strncpy(host, css + 3, len);
                    host[len] = '\0';
                }
            }

            port = 0;
            return true;
        }
    }

    return false;
}

const char * Message::requestMethod(void) const
{
    return p->statusLine[0].c_str();
}

void Message::setRequestMethod(const char *method)
{
    p->statusLine[0] = method;
    p->statusLine[2] = "HTTP/1.1";
}

const char * Message::requestPath(void) const
{
    return p->requestPath.c_str();
}

void Message::setRequestPath(const char *path)
{
    std::ostringstream fp;
    fp.imbue(std::locale::classic());
    fp << std::hex << std::setfill('0');

    for (const unsigned char *i = reinterpret_cast<const unsigned char *>(path); *i; i++)
    if (((*i >= 'A') && (*i <= 'Z')) || ((*i >= 'a') && (*i <= 'z')) ||
        ((*i >= '0') && (*i <= '9')) || ((*i >= '&') && (*i <= '/')) || 
        (*i == ':') || (*i == ';') || (*i == '@') || (*i == '_') || 
        (*i == '?') || (*i == '=') || (*i == '&') ||
        (*i == '#'))
    {
        fp << char(*i);
    }
    else
    {
        fp << '%' << std::setw(2) << int(*i);
    }

    p->requestPath = path;
    p->statusLine[1] = fp.str();
}

int Message::responseStatus(void) const
{
    if (!p->statusLine[1].empty())
        return ::atoi(p->statusLine[1].c_str());

    return -1;
}

void Message::setResponseStatus(int status)
{
    std::ostringstream str;
    str.imbue(std::locale::classic());
    str << status;

    p->statusLine[0] = "HTTP/1.1";
    p->statusLine[1] = str.str();
    switch (status)
    {
    case 101:   p->statusLine[2] = "Switching Protocols";   break;
    case 200:   p->statusLine[2] = "OK";                    break;
    case 307:   p->statusLine[2] = "Temporary Redirect";    break;
    case 404:   p->statusLine[2] = "Not Found";             break;
    case 405:   p->statusLine[2] = "Method Not Allowed";    break;
    case 500:   p->statusLine[2] = "Internal Server Error"; break;
    case 504:   p->statusLine[2] = "Gateway Time-out";		break;
    default:    p->statusLine[2] = std::string();           break;
    }
}

const char * Message::field(const char *name, unsigned index) const
{
    if (name)
    {
        auto f = p->fields.find(name);
        if ((f != p->fields.end()) && (index < f->second.size()))
            return f->second[index].c_str();
    }

    return nullptr;
}

void Message::setField(const char *name, const char *value)
{
    if (name)
    {
        if (value)
        {
            std::vector<std::string> &values = p->fields[name];
            values.clear();
            values.push_back(value);
        }
        else
            p->fields.erase(name);
    }
}

void Message::addField(const char *name, const char *value)
{
    if (name && value)
        p->fields[name].push_back(value);
}

const char * Message::header(void) const
{
    std::ostringstream str;
    str.imbue(std::locale::classic());
    str << p->statusLine[0] << " " << p->statusLine[1];
    if (!p->statusLine[2].empty())
        str << " " << p->statusLine[2] << "\r\n";
    else
        str << "\r\n";

    str << "Content-Length: " << p->content.size() << "\r\n";

    for (const auto &i : p->fields)
    {
        if (strcasecmp(i.first.c_str(), "Content-Length") != 0)
        for (const auto &j : i.second)
            str << i.first << ": " << j << "\r\n";
    }

    str << "\r\n";
    p->header = str.str();

    return p->header.c_str();
}

const std::vector<uint8_t> & Message::content(void) const
{
    return p->content;
}

std::vector<uint8_t> & Message::content(void)
{
    return p->content;
}

void Message::clrResizeContent(int newLen)
{
    p->content.resize(static_cast<size_t>(newLen));
}

void * Message::clrContentPtr()
{
    return &(p->content[0]);
}

int Message::clrContentLen()
{
    return int(p->content.size());
}

} // End of namespaces
