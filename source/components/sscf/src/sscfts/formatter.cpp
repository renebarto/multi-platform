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
#endif

#define SSCFTS1_FORMATTER_CPP
#include "sscfts1/formatter.h"
#include "sscfts1/bulkdata.h"
#include "sscf1/bulkchannel.h"
#include "sscf1/system.h"
#include "sscf/byteorder.h"
#include <cstring>
#include <stdexcept>

#if defined(_WIN32)
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
# include <cstdlib>
# undef min
# undef max
#elif defined(__GNUC__)
# include <clocale>
# include <cstdlib>
# include <climits>
# include <mutex>
#else
# include <codecvt>
# include <locale>
#endif

namespace SscfTs1 {
namespace Formatter {
	
//static const uint8_t uninitialized = 0xCC;
//static const size_t clearSize = 1048576; //!< Number of bytes to clear with uninitialized.

///////////////////////////////////////////////////////////////////////////////
// Byte-order conversion.
template<> int8_t		eswap<int8_t	>(int8_t		data) { return ::eswap(data); }
template<> uint8_t		eswap<uint8_t	>(uint8_t		data) { return ::eswap(data); }
template<> bool			eswap<bool		>(bool			data) { return ::eswap(data); }
template<> char			eswap<char		>(char			data) { return ::eswap(data); }
template<> int16_t		eswap<int16_t	>(int16_t		data) { return ::eswap(data); }
template<> uint16_t		eswap<uint16_t	>(uint16_t		data) { return ::eswap(data); }
template<> int32_t		eswap<int32_t	>(int32_t		data) { return ::eswap(data); }
template<> uint32_t		eswap<uint32_t	>(uint32_t		data) { return ::eswap(data); }
template<> int64_t		eswap<int64_t	>(int64_t		data) { return ::eswap(data); }
template<> uint64_t		eswap<uint64_t	>(uint64_t		data) { return ::eswap(data); }
template<> float		eswap<float		>(float			data) { return ::eswap(data); }
template<> double		eswap<double	>(double		data) { return ::eswap(data); }

#if (!defined(_MSC_VER) && defined(_LP64))
template<> signed long long		eswap<signed long long	>(signed long long   data) { return ::eswap(data); }
template<> unsigned long long	eswap<unsigned long long>(unsigned long long data) { return ::eswap(data); }
#endif

#if defined(__GNUC__)
static volatile bool utf8LocaleInitialized = false;
#endif

///////////////////////////////////////////////////////////////////////////////
// Struct
Struct::Struct(size_t size)
{
    memset(this, 0, size);
}

///////////////////////////////////////////////////////////////////////////////
// Serializer
Serializer::Serializer(std::vector<uint8_t> &buffer, Sscf1::BulkChannel *bulkChannel)
  : buffer(buffer),
	bulkChannel(bulkChannel),
	serializedBulkIds()
{
    if (buffer.empty())
        buffer.reserve(64);
}

Serializer::~Serializer()
{
}

Serializer & Serializer::operator<<(const BulkData &val)
{
	if (bulkChannel)
	{
		const Sscf1::BulkChannel::BulkId bulkId = bulkChannel->enqueue(val);
		serializedBulkIds.push_back(bulkId);

		return write(bulkId.data, sizeof(bulkId.data));
	}
	else
		Sscf1::system().fail("Serializing BulkData while BulkChannel not available.");
}

Serializer & Serializer::operator<<(int8_t		val) { return writeBasicType(val); }
Serializer & Serializer::operator<<(uint8_t		val) { return writeBasicType(val); }
Serializer & Serializer::operator<<(int16_t		val) { return writeBasicType(val); }
Serializer & Serializer::operator<<(uint16_t	val) { return writeBasicType(val); }
Serializer & Serializer::operator<<(int32_t		val) { return writeBasicType(val); }
Serializer & Serializer::operator<<(uint32_t	val) { return writeBasicType(val); }
Serializer & Serializer::operator<<(int64_t		val) { return writeBasicType(val); }
Serializer & Serializer::operator<<(uint64_t	val) { return writeBasicType(val); }
Serializer & Serializer::operator<<(bool		val) { return writeBasicType(val); }
Serializer & Serializer::operator<<(char		val) { return writeBasicType(val); }
Serializer & Serializer::operator<<(float		val) { return writeBasicType(val); }
Serializer & Serializer::operator<<(double		val) { return writeBasicType(val); }

#if (!defined(_MSC_VER) && defined(_LP64))
Serializer & Serializer::operator<<(signed long long	val) { return writeBasicType(val); }
Serializer & Serializer::operator<<(unsigned long long	val) { return writeBasicType(val); }
#endif

template <typename _type>
Serializer & Serializer::write(const _type *data, size_t size)
{
	if (size > 0)
	{
		size_t pos = buffer.size();
		buffer.resize(pos + (size * sizeof(*data)));
		for (size_t i = 0; i < size; i++)
		{
		    const _type result = eswap(data[i]);
		    std::memcpy(&buffer[pos + (i * sizeof(*data))], &result, sizeof(result));
		}
	}

    return *this;
}

template <>
Serializer & Serializer::write<uint8_t>(const uint8_t *data, size_t size)
{
	if (size > 0)
	{
		size_t pos = buffer.size();
		buffer.resize(pos + size);
		memcpy(&(buffer[pos]), data, size);
	}

    return *this;
}

template <> Serializer & Serializer::write<int8_t>(const int8_t *data, size_t size)
{
    return write<uint8_t>(reinterpret_cast<const uint8_t *>(data), size);
}

template <> Serializer & Serializer::write<char>(const char *data, size_t size)
{
    return write<uint8_t>(reinterpret_cast<const uint8_t *>(data), size);
}

template <typename _type> 
Serializer & Serializer::writeBasicType(_type val) 
{ 
    size_t pos = buffer.size();
    buffer.resize(pos + sizeof(val));

    const _type result = eswap(val);
    std::memcpy(&buffer[pos], &result, sizeof(result));

    return *this;
}

template Serializer & Serializer::writeBasicType<int8_t		>(int8_t	);
template Serializer & Serializer::writeBasicType<uint8_t	>(uint8_t	);
template Serializer & Serializer::writeBasicType<int16_t	>(int16_t	);
template Serializer & Serializer::writeBasicType<uint16_t	>(uint16_t	);
template Serializer & Serializer::writeBasicType<int32_t	>(int32_t	);
template Serializer & Serializer::writeBasicType<uint32_t	>(uint32_t	);
template Serializer & Serializer::writeBasicType<int64_t	>(int64_t	);
template Serializer & Serializer::writeBasicType<uint64_t	>(uint64_t	);

template Serializer & Serializer::writeBasicType<bool		>(bool		);
template Serializer & Serializer::writeBasicType<char		>(char		);
template Serializer & Serializer::writeBasicType<float		>(float		);
template Serializer & Serializer::writeBasicType<double		>(double	);

#if (!defined(_MSC_VER) && defined(_LP64))
template Serializer & Serializer::writeBasicType<signed long long	>(signed long long	);
template Serializer & Serializer::writeBasicType<unsigned long long	>(unsigned long long);
#endif

Serializer & Serializer::write(const std::string &val)
{
	writeBasicType(int32_t(val.length()));
	if (val.length() > 0)
		write(&(val[0]), val.length());

	return *this;
}

Serializer & Serializer::write(const std::wstring &val)
{
#if defined(_WIN32) // WideCharToMultiByte() is significantly faster than std::wstring_convert()
	if (val.size() > 0)
	{
		std::vector<char> buf;
		buf.resize(val.size() * 4);
		const int32_t len = ::WideCharToMultiByte(
			CP_UTF8, 0,
			&(val[0]), int(val.size()),
			&(buf[0]), int(buf.size()),
			NULL, NULL);

		return writeBasicType(len).write(&(buf[0]), static_cast<size_t>(len));
	}
	else
		return writeBasicType(int32_t(0));
#elif !defined(__GNUC__) // GCC does not support standard code conversion facets yet
	std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8;

	return write(utf8.to_bytes(val));
#else
    if (!utf8LocaleInitialized)
    {
        std::setlocale(LC_CTYPE, "en_US.UTF-8");
        utf8LocaleInitialized = true;
    }

    if (val.size() > 0)
    {
        std::vector<char> buf;
        buf.resize(val.size() * 4);
        const size_t len = ::wcstombs(&(buf[0]), val.c_str(), buf.size());
        if ((len != size_t(-1)) && (len < INT_MAX))
            return writeBasicType(int32_t(len)).write(&(buf[0]), len);
    }

    return writeBasicType(int32_t(0));
#endif
}

template <typename _type>
inline Serializer & Serializer::writeBasicArray(const _type *data, int32_t count)
{
	buffer.reserve(buffer.size() + sizeof(count) + (sizeof(*data) * count));

	return writeBasicType(count).write(data, static_cast<size_t>(count));
}

template <> Serializer & Serializer::writeArray<int8_t	 >(const int8_t		*data, int32_t count) { return writeBasicArray(data, count); }
template <> Serializer & Serializer::writeArray<uint8_t >(const uint8_t		*data, int32_t count) { return writeBasicArray(data, count); }
template <> Serializer & Serializer::writeArray<int16_t >(const int16_t		*data, int32_t count) { return writeBasicArray(data, count); }
template <> Serializer & Serializer::writeArray<uint16_t>(const uint16_t	*data, int32_t count) { return writeBasicArray(data, count); }
template <> Serializer & Serializer::writeArray<int32_t >(const int32_t		*data, int32_t count) { return writeBasicArray(data, count); }
template <> Serializer & Serializer::writeArray<uint32_t>(const uint32_t	*data, int32_t count) { return writeBasicArray(data, count); }
template <> Serializer & Serializer::writeArray<int64_t >(const int64_t		*data, int32_t count) { return writeBasicArray(data, count); }
template <> Serializer & Serializer::writeArray<uint64_t>(const uint64_t	*data, int32_t count) { return writeBasicArray(data, count); }
template <> Serializer & Serializer::writeArray<bool	 >(const bool		*data, int32_t count) { return writeBasicArray(data, count); }
template <> Serializer & Serializer::writeArray<char	 >(const char		*data, int32_t count) { return writeBasicArray(data, count); }
template <> Serializer & Serializer::writeArray<float	 >(const float		*data, int32_t count) { return writeBasicArray(data, count); }
template <> Serializer & Serializer::writeArray<double	 >(const double		*data, int32_t count) { return writeBasicArray(data, count); }

template <typename _container>
Serializer & Serializer::writeBasicContainer(const _container &data)
{
	if (!data.empty())
		return writeBasicArray(&(data[0]), int32_t(data.size()));
	else
		return writeBasicType(int32_t(0));
}

template <> Serializer & Serializer::writeContainer<std::vector<int8_t	>>(const std::vector<int8_t		> &data) { return writeBasicContainer(data); }
template <> Serializer & Serializer::writeContainer<std::vector<uint8_t	>>(const std::vector<uint8_t	> &data) { return writeBasicContainer(data); }
template <> Serializer & Serializer::writeContainer<std::vector<int16_t	>>(const std::vector<int16_t	> &data) { return writeBasicContainer(data); }
template <> Serializer & Serializer::writeContainer<std::vector<uint16_t>>(const std::vector<uint16_t	> &data) { return writeBasicContainer(data); }
template <> Serializer & Serializer::writeContainer<std::vector<int32_t	>>(const std::vector<int32_t	> &data) { return writeBasicContainer(data); }
template <> Serializer & Serializer::writeContainer<std::vector<uint32_t>>(const std::vector<uint32_t	> &data) { return writeBasicContainer(data); }
template <> Serializer & Serializer::writeContainer<std::vector<int64_t	>>(const std::vector<int64_t	> &data) { return writeBasicContainer(data); }
template <> Serializer & Serializer::writeContainer<std::vector<uint64_t>>(const std::vector<uint64_t	> &data) { return writeBasicContainer(data); }
template <> Serializer & Serializer::writeContainer<std::vector<char	>>(const std::vector<char		> &data) { return writeBasicContainer(data); }
template <> Serializer & Serializer::writeContainer<std::vector<float	>>(const std::vector<float		> &data) { return writeBasicContainer(data); }
template <> Serializer & Serializer::writeContainer<std::vector<double	>>(const std::vector<double		> &data) { return writeBasicContainer(data); }

template <>
Serializer & Serializer::writeContainer<std::vector<bool>>(const std::vector<bool> &data)
{
	buffer.reserve(buffer.size() + sizeof(int32_t) + (sizeof(bool) * data.size()));

	writeBasicType(int32_t(data.size()));
    for (const auto &i : data)
		writeBasicType(bool(i));

	return *this;
}

///////////////////////////////////////////////////////////////////////////////
// Deserializer
Deserializer::Deserializer(const std::vector<uint8_t> &buffer, Sscf1::BulkChannel *bulkChannel)
  : buffer(buffer), 
	readPos(0),
	bulkChannel(bulkChannel)
{
}

Deserializer::~Deserializer()
{
}

Deserializer & Deserializer::operator>>(BulkData &val)
{
	if (bulkChannel)
	{
		Sscf1::BulkChannel::BulkId bulkId;
		Deserializer &result = read(bulkId.data, sizeof(bulkId.data));
		val = bulkChannel->retrieve(bulkId);
		return result;
	}
	else
		Sscf1::system().fail("Serializing BulkData while BulkChannel not available.");
}

template <typename _type>
Deserializer & Deserializer::read(_type *data, size_t size)
{
	if (buffer.size() >= readPos + (size * sizeof(*data)))
	{
		for (size_t i = 0; i < size; i++)
		{
		    _type result;
		    std::memcpy(&result, &buffer[readPos], sizeof(result));
			
			data[i] = eswap(result);
			readPos += sizeof(result);
		}
	}
	else // Mark uninitialized.
		memset(data, uninitialized, std::min(size * sizeof(*data), clearSize));

	return *this;
}

template <>
Deserializer & Deserializer::read<uint8_t>(uint8_t *data, size_t size)
{
	if (buffer.size() >= readPos + size)
	{
		memcpy(data, &(buffer[readPos]), size);
		readPos += size;
	}
	else // Mark uninitialized.
		memset(data, uninitialized, std::min(size, clearSize));

	return *this;
}

template <> Deserializer & Deserializer::read<int8_t>(int8_t *data, size_t size)
{
    return read<uint8_t>(reinterpret_cast<uint8_t *>(data), size);
}

template <> Deserializer & Deserializer::read<char>(char *data, size_t size)
{
    return read<uint8_t>(reinterpret_cast<uint8_t *>(data), size);
}

template Deserializer & Deserializer::readBasicType<int8_t	>(int8_t	&);
template Deserializer & Deserializer::readBasicType<uint8_t	>(uint8_t	&);
template Deserializer & Deserializer::readBasicType<int16_t	>(int16_t	&);
template Deserializer & Deserializer::readBasicType<uint16_t>(uint16_t	&);
template Deserializer & Deserializer::readBasicType<int32_t	>(int32_t	&);
template Deserializer & Deserializer::readBasicType<uint32_t>(uint32_t	&);
template Deserializer & Deserializer::readBasicType<int64_t	>(int64_t	&);
template Deserializer & Deserializer::readBasicType<uint64_t>(uint64_t	&);

template Deserializer & Deserializer::readBasicType<bool	>(bool		&);
template Deserializer & Deserializer::readBasicType<char	>(char		&);
template Deserializer & Deserializer::readBasicType<float	>(float		&);
template Deserializer & Deserializer::readBasicType<double	>(double	&);

#if (!defined(_MSC_VER) && defined(_LP64))
template Deserializer & Deserializer::readBasicType<signed long long	>(signed long long      &);
template Deserializer & Deserializer::readBasicType<unsigned long long	>(unsigned long long	&);
#endif

Deserializer & Deserializer::read(std::string &val)
{
	int32_t len;
	readBasicType(len);
	if (len > 0)
	{
		val.resize(static_cast<size_t>(len));
		read(&(val[0]), val.length());
	}
	else
		val.clear();

	return *this;
}

Deserializer & Deserializer::read(std::wstring &val)
{
	std::string str;
	Deserializer &result = read(str);

#if defined(_WIN32) // MultiByteToWideChar() is significantly faster than std::wstring_convert()
	if (str.size() > 0)
	{
		val.resize(str.size());
		val.resize(static_cast<size_t>(::MultiByteToWideChar(
			CP_UTF8, 0,
			str.c_str(), int(str.size()),
			&(val[0]), int(val.size()))));
	}
    else
        val.clear();
#elif !defined(__GNUC__) // C++11 support, GCC does not support standard code conversion facets yet
    std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8;

	val = utf8.from_bytes(str);
#else
   if (!utf8LocaleInitialized)
   {
       std::setlocale(LC_CTYPE, "en_US.UTF-8");
       utf8LocaleInitialized = true;
   }

   if (str.size() > 0)
   {
       val.resize(str.size());
       val.resize(::mbstowcs(&(val[0]), str.c_str(), val.size()));
   }
   else
       val.clear();
#endif

	return result;
}

template <typename _type>
inline Deserializer & Deserializer::readBasicArray(_type *data, int32_t count)
{
    int32_t len;
	readBasicType(len);
	if (len > 0)
	{
		const int32_t c = std::max(0, std::min(count, len));
		read(data, static_cast<size_t>(c));
		if (c < count) // Mark rest
			// .
			memset(data + c, uninitialized, std::min((count - c) * sizeof(*data), clearSize));
	}
	else // Mark uninitialized.
		memset(data, uninitialized, std::min(count * sizeof(*data), clearSize));

	return *this;
}

template <> Deserializer & Deserializer::readArray<int8_t	>(int8_t	*data, int32_t count) { return readBasicArray(data, count); }
template <> Deserializer & Deserializer::readArray<uint8_t	>(uint8_t	*data, int32_t count) { return readBasicArray(data, count); }
template <> Deserializer & Deserializer::readArray<int16_t	>(int16_t	*data, int32_t count) { return readBasicArray(data, count); }
template <> Deserializer & Deserializer::readArray<uint16_t>(uint16_t	*data, int32_t count) { return readBasicArray(data, count); }
template <> Deserializer & Deserializer::readArray<int32_t	>(int32_t	*data, int32_t count) { return readBasicArray(data, count); }
template <> Deserializer & Deserializer::readArray<uint32_t>(uint32_t	*data, int32_t count) { return readBasicArray(data, count); }
template <> Deserializer & Deserializer::readArray<int64_t	>(int64_t	*data, int32_t count) { return readBasicArray(data, count); }
template <> Deserializer & Deserializer::readArray<uint64_t>(uint64_t	*data, int32_t count) { return readBasicArray(data, count); }
template <> Deserializer & Deserializer::readArray<bool	>(bool		*data, int32_t count) { return readBasicArray(data, count); }
template <> Deserializer & Deserializer::readArray<char	>(char		*data, int32_t count) { return readBasicArray(data, count); }
template <> Deserializer & Deserializer::readArray<float	>(float		*data, int32_t count) { return readBasicArray(data, count); }
template <> Deserializer & Deserializer::readArray<double	>(double	*data, int32_t count) { return readBasicArray(data, count); }

template <typename _type>
inline Deserializer & Deserializer::readBasicVector(std::vector<_type> &vector)
{
    int32_t len;
	readBasicType(len);
	if (len > 0)
	{
		vector.resize(static_cast<size_t>(std::max(0, len)));
		read(&(vector[0]), vector.size());
	}
	else
		vector.clear();

	return *this;
}

template <> Deserializer & Deserializer::readContainer<std::vector<int8_t  >>(std::vector<int8_t	> &data) { return readBasicVector(data); }
template <> Deserializer & Deserializer::readContainer<std::vector<uint8_t >>(std::vector<uint8_t	> &data) { return readBasicVector(data); }
template <> Deserializer & Deserializer::readContainer<std::vector<int16_t >>(std::vector<int16_t	> &data) { return readBasicVector(data); }
template <> Deserializer & Deserializer::readContainer<std::vector<uint16_t>>(std::vector<uint16_t	> &data) { return readBasicVector(data); }
template <> Deserializer & Deserializer::readContainer<std::vector<int32_t >>(std::vector<int32_t	> &data) { return readBasicVector(data); }
template <> Deserializer & Deserializer::readContainer<std::vector<uint32_t>>(std::vector<uint32_t	> &data) { return readBasicVector(data); }
template <> Deserializer & Deserializer::readContainer<std::vector<int64_t >>(std::vector<int64_t	> &data) { return readBasicVector(data); }
template <> Deserializer & Deserializer::readContainer<std::vector<uint64_t>>(std::vector<uint64_t	> &data) { return readBasicVector(data); }
template <> Deserializer & Deserializer::readContainer<std::vector<char    >>(std::vector<char		> &data) { return readBasicVector(data); }
template <> Deserializer & Deserializer::readContainer<std::vector<float   >>(std::vector<float		> &data) { return readBasicVector(data); }
template <> Deserializer & Deserializer::readContainer<std::vector<double  >>(std::vector<double	> &data) { return readBasicVector(data); }

template <>
Deserializer & Deserializer::readContainer<std::vector<bool>>(std::vector<bool> &data)
{
	int32_t len;
	readBasicType(len);
	if (len > 0)
	{
		data.resize(static_cast<size_t>(len));
		for (auto i : data)
		{
			bool val;
			readBasicType(val);
			i = val;
		}
	}
	else
		data.clear();

	return *this;
}

} } // End of namespaces
