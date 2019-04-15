/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifndef REMOTINGTS1_FORMATTER_H
#define REMOTINGTS1_FORMATTER_H

#include <array>
#include <cstring>
#include <type_traits>
#include <cstdint>
#include <list>
#include <set>
#include <string>
#include <vector>
#include "remoting1/bulkchannel.h"

namespace RemotingTs1 {

class BulkData;

namespace Formatter {

/*! Helper function that performs a byte-order conversion if needed.
 */
template <typename _type> _type eswap(_type);

/*! Helper class to ensure structs are initialized on creation.
 */
struct Struct
{
protected:
    explicit Struct(size_t size);
};

/*! Class to serialize objects to a buffer. To make custom serializable objects
    define an operator<< for this type in the namespace of the type:
    RemotingTs1::Formatter::Serializer & operator<<(RemotingTs1::Formatter::Serializer &, const MyClass &);
    Make sure binary serialization is done as newtork byteorder, the
    RemotingTs1::Formatter::eswap() function will swap the bytes if needed.
 */
class REMOTING_EXPORT Serializer final
{
public:
    Serializer(std::vector<uint8_t> &buffer, Remoting1::BulkChannel * = nullptr);
    ~Serializer();

    Serializer & operator<<(const BulkData &val);

    const std::vector<Remoting1::BulkChannel::BulkId> & bulkIds() const { return serializedBulkIds; }

    Serializer & operator<<(int8_t		val);
    Serializer & operator<<(uint8_t		val);
    Serializer & operator<<(int16_t		val);
    Serializer & operator<<(uint16_t	val);
    Serializer & operator<<(int32_t		val);
    Serializer & operator<<(uint32_t	val);
    Serializer & operator<<(int64_t		val);
    Serializer & operator<<(uint64_t	val);
    Serializer & operator<<(bool		val);
    Serializer & operator<<(char		val);
    Serializer & operator<<(float		val);
    Serializer & operator<<(double		val);

#if (!defined(_MSC_VER) && defined(_LP64))
    Serializer & operator<<(signed long long	val);
    Serializer & operator<<(unsigned long long	val);
#endif

    /*! The operator<< can be used to serialize data, for example:
        \code
            std::vector<uint8_t> buffer;
            {
                RemotingTs1::Formatter::Serializer serializer(buffer);

                int val1 = 1;
                float val2 = 5.7;
                uint64_t val3 = 12345678;

                serializer << val1 << val2 << val3;
            }
            {
                RemotingTs1::Formatter::Deserializer deserializer(buffer);

                int val1;
                float val2;
                uint64_t val3;

                deserializer >> val1 >> val2 >> val3;

                std::cout << val1 << val2 << val3 << std::endl;
            }
        \endcode
     */
    template <typename _type>
    Serializer & operator<<(const _type &val)
    {
#if !defined(__INTELLISENSE__) || defined(REMOTINGTS1_BUILD) // Hide internals from IntelliSense
        return EnumSelector<std::is_enum<_type>::value>::write(*this, val);
#endif // __INTELLISENSE__
    }

    /*! Writes a block of data, making sure each of the elements in data is in
        network byte order.
        \param	data	Pointer to the block of data.
        \param	size	Number of elements to write.
     */
    template <typename _type>
    Serializer & write(const _type *data, size_t size);

private:
#if !defined(__INTELLISENSE__) || defined(REMOTINGTS1_BUILD) // Hide internals from IntelliSense
///////////////////////////////////////////////////////////////////////////////
// Serialization of basic types.
    template <typename _type> Serializer & writeBasicType(_type val);

    Serializer & write(int8_t	val)	{ return writeBasicType(val); }
    Serializer & write(uint8_t	val)	{ return writeBasicType(val); }
    Serializer & write(int16_t	val)	{ return writeBasicType(val); }
    Serializer & write(uint16_t	val)	{ return writeBasicType(val); }
    Serializer & write(int32_t	val)	{ return writeBasicType(val); }
    Serializer & write(uint32_t	val)	{ return writeBasicType(val); }
    Serializer & write(int64_t	val)	{ return writeBasicType(val); }
    Serializer & write(uint64_t	val)	{ return writeBasicType(val); }

    Serializer & write(bool		val)	{ return writeBasicType(val); }
    Serializer & write(char		val)	{ return writeBasicType(val); }
    Serializer & write(float	val)	{ return writeBasicType(val); }
    Serializer & write(double	val)	{ return writeBasicType(val); }

#if (!defined(_MSC_VER) && defined(_LP64))
    Serializer & write(signed long long		val)	{ return writeBasicType(val); }
    Serializer & write(unsigned long long	val)	{ return writeBasicType(val); }
#else
    // The 32-bit "long" type is not supported to prevent portability issues.
    Serializer & write(signed long			val, bool not_supported);
    Serializer & write(unsigned long		val, bool not_supported);
#endif

    // The "long double" and "wchar_t" types are not supported to prevent portability issues.
    Serializer & write(long double			val, bool not_supported);
#if !defined(_MSC_VER) || defined(_NATIVE_WCHAR_T_DEFINED)
    Serializer & write(wchar_t				val, bool not_supported);
#endif

///////////////////////////////////////////////////////////////////////////////
// Serialization of strings.
    Serializer & write(const std::string &val);

    // all wstrings are converted to/from UTF8 to deal with big-endian and
    // little-endian systems, and systems with 16 bit and 32 bit unicode.
    Serializer & write(const std::wstring &val);

///////////////////////////////////////////////////////////////////////////////
// Serialization of containers and arrays.
    template <typename _type>
    Serializer & writeBasicArray(const _type *data, int32_t count);

    template <typename _type>
    Serializer & writeArray(const _type *data, int32_t count)
    {
        *this << count;
        for (int32_t i=0; i<count; i++)
            *this << data[i];

        return *this;
    }

    template <typename _container>
    Serializer & writeBasicContainer(const _container &data);

    template <typename _container>
    Serializer & writeContainer(const _container &data)
    {
        *this << int32_t(data.size());
        for (const auto &i : data)
            *this << i;

        return *this;
    }

    template <typename _type, int32_t _count>
    Serializer & write(const _type (& data)[_count]) { return writeArray(data, _count); }

    template <typename _type, int32_t _count>
    Serializer & write(const std::array<_type, _count> &data) { return writeArray(data.data(), _count); }

    template <typename _type> 
    Serializer & write(const std::vector<_type> &data) { return writeContainer(data); }
    template <typename _type> 
    Serializer & write(const std::list<_type> &data) { return writeContainer(data); }
    template <typename _type> 
    Serializer & write(const std::set<_type> &data) { return writeContainer(data); }

///////////////////////////////////////////////////////////////////////////////
// Serialization of enums.
    template <bool _enum>
    struct EnumSelector
    {
        template <typename _type>
        static Serializer & write(Serializer &s, const _type &val)
        {
            // The compiler gives an "cannot convert parameter 1 from ... to 'int8_t'"
            // error here if there is no serializer specified for the ... type.
            // Refer to the Doxygen documentation in this file on how to write
            // custom serializers.
            return s.write(val);
        }
    };

private:
    Serializer(const Serializer &);
    Serializer & operator=(const Serializer &);

private:
    std::vector<uint8_t> &buffer;
    Remoting1::BulkChannel * const bulkChannel;
    std::vector<Remoting1::BulkChannel::BulkId> serializedBulkIds;
#endif // __INTELLISENSE__
};

#if !defined(__INTELLISENSE__) || defined(REMOTINGTS1_BUILD) // Hide internals from IntelliSense

// Optimized specializations
template <> Serializer & Serializer::write<uint8_t>(const uint8_t *data, size_t size);
template <> Serializer & Serializer::write<int8_t>(const int8_t *data, size_t size);
template <> Serializer & Serializer::write<char  >(const char   *data, size_t size);

template <> Serializer & Serializer::writeArray<int8_t	>(const int8_t	*data, int32_t count);
template <> Serializer & Serializer::writeArray<uint8_t	>(const uint8_t	*data, int32_t count);
template <> Serializer & Serializer::writeArray<int16_t	>(const int16_t	*data, int32_t count);
template <> Serializer & Serializer::writeArray<uint16_t>(const uint16_t*data, int32_t count);
template <> Serializer & Serializer::writeArray<int32_t	>(const int32_t	*data, int32_t count);
template <> Serializer & Serializer::writeArray<uint32_t>(const uint32_t*data, int32_t count);
template <> Serializer & Serializer::writeArray<int64_t	>(const int64_t	*data, int32_t count);
template <> Serializer & Serializer::writeArray<uint64_t>(const uint64_t*data, int32_t count);
template <> Serializer & Serializer::writeArray<bool	>(const bool	*data, int32_t count);
template <> Serializer & Serializer::writeArray<char	>(const char	*data, int32_t count);
template <> Serializer & Serializer::writeArray<float	>(const float	*data, int32_t count);
template <> Serializer & Serializer::writeArray<double	>(const double	*data, int32_t count);

template <> Serializer & Serializer::writeContainer<std::vector<int8_t  >>(const std::vector<int8_t		> &data);
template <> Serializer & Serializer::writeContainer<std::vector<uint8_t	>>(const std::vector<uint8_t	> &data);
template <> Serializer & Serializer::writeContainer<std::vector<int16_t	>>(const std::vector<int16_t	> &data);
template <> Serializer & Serializer::writeContainer<std::vector<uint16_t>>(const std::vector<uint16_t	> &data);
template <> Serializer & Serializer::writeContainer<std::vector<int32_t	>>(const std::vector<int32_t	> &data);
template <> Serializer & Serializer::writeContainer<std::vector<uint32_t>>(const std::vector<uint32_t	> &data);
template <> Serializer & Serializer::writeContainer<std::vector<int64_t	>>(const std::vector<int64_t	> &data);
template <> Serializer & Serializer::writeContainer<std::vector<uint64_t>>(const std::vector<uint64_t	> &data);
template <> Serializer & Serializer::writeContainer<std::vector<char    >>(const std::vector<char		> &data);
template <> Serializer & Serializer::writeContainer<std::vector<float   >>(const std::vector<float		> &data);
template <> Serializer & Serializer::writeContainer<std::vector<double  >>(const std::vector<double		> &data);

// Special version as std::vector<bool> is a bit array and needs to be treated slightly different.
template <> Serializer & Serializer::writeContainer<std::vector<bool>>(const std::vector<bool> &);

// This specialization will be used for enums.
template <> 
struct Serializer::EnumSelector<true>  
{ 
    template <typename _type>
    static Serializer & write(Serializer &s, const _type &val)
    {
        return s.writeBasicType((typename std::underlying_type<_type>::type)val);
    }
};

#endif // __INTELLISENSE__

/*! Class to deserialize objects from a buffer. To make custom deserializable objects
    define an operator>> for this type in the namespace of the type:
    RemotingTs1::Formatter::Deserializer & operator>>(RemotingTs1::Formatter::Deserializer &, MyClass &);
    Make sure binary serialization is done as newtork byteorder, the
    RemotingTs1::Formatter::eswap() function will swap the bytes if needed.
 */
class REMOTING_EXPORT Deserializer final
{
public:
    Deserializer(const std::vector<uint8_t> &buffer, Remoting1::BulkChannel * = nullptr);
    ~Deserializer();

    Deserializer & operator>>(BulkData &val);

    /*! The operator>> can be used to deserialize data, for example:
        \code
            std::vector<uint8_t> buffer;
            {
                ::RemotingTs1::Formatter::Serializer serializer(buffer);

                int val1 = 1;
                float val2 = 5.7;
                uint64_t val3 = 12345678;

                serializer << val1 << val2 << val3;
            }
            {
                ::RemotingTs1::Formatter::Deserializer deserializer(buffer);

                int val1;
                float val2;
                uint64_t val3;

                deserializer >> val1 >> val2 >> val3;

                std::cout << val1 << val2 << val3 << std::endl;
            }
        \endcode
     */
    template <typename _type>
    Deserializer & operator>>(_type &val)
    {
#if !defined(__INTELLISENSE__) || defined(REMOTINGTS1_BUILD) // Hide internals from IntelliSense
        return EnumSelector<std::is_enum<_type>::value>::read(*this, val);
#endif // __INTELLISENSE__
    }

    /*! Reads a block of data, making sure each of the elements in data is in
        host byte order.
        \param	data	Pointer to the destination.
        \param	size	Number of elements to read.
     */
    template <typename _type>
    Deserializer & read(_type *data, size_t size);

private:
#if !defined(__INTELLISENSE__) || defined(REMOTINGTS1_BUILD) // Hide internals from IntelliSense
///////////////////////////////////////////////////////////////////////////////
// Serialization of basic types.
    template <typename _type> Deserializer & readBasicType(_type &val);

    Deserializer & read(int8_t		&val)	{ return readBasicType(val); }
    Deserializer & read(uint8_t		&val)	{ return readBasicType(val); }
    Deserializer & read(int16_t		&val)	{ return readBasicType(val); }
    Deserializer & read(uint16_t	&val)	{ return readBasicType(val); }
    Deserializer & read(int32_t		&val)	{ return readBasicType(val); }
    Deserializer & read(uint32_t	&val)	{ return readBasicType(val); }
    Deserializer & read(int64_t		&val)	{ return readBasicType(val); }
    Deserializer & read(uint64_t	&val)	{ return readBasicType(val); }

    Deserializer & read(bool		&val)	{ return readBasicType(val); }
    Deserializer & read(char		&val)	{ return readBasicType(val); }
    Deserializer & read(float		&val)	{ return readBasicType(val); }
    Deserializer & read(double		&val)	{ return readBasicType(val); }

#if (!defined(_MSC_VER) && defined(_LP64))
    Deserializer & read(signed long long	&val)	{ return readBasicType(val); }
    Deserializer & read(unsigned long long	&val)	{ return readBasicType(val); }
#else
    // The 32-bit "long" type is not supported to prevent portability issues.
    Deserializer & read(signed long		&, bool not_supported);
    Deserializer & read(unsigned long	&, bool not_supported);
#endif

    // The "long double" and "wchar_t" types are not supported to prevent portability issues.
    Deserializer & read(long double		&, bool not_supported);
#if !defined(_MSC_VER) || defined(_NATIVE_WCHAR_T_DEFINED)
    Deserializer & read(wchar_t			&, bool not_supported);
#endif

///////////////////////////////////////////////////////////////////////////////
// Serialization of strings.
    Deserializer & read(std::string &val);

    // all wstrings are converted to/from UTF8 to deal with big-endian and
    // little-endian systems, and systems with 16 bit and 32 bit unicode.
    Deserializer & read(std::wstring &val);

///////////////////////////////////////////////////////////////////////////////
// Serialization of containers and arrays.
    template <typename _type>
    Deserializer & readBasicArray(_type *data, int32_t count);

    template <typename _type>
    Deserializer & readArray(_type *data, int32_t count)
    {
        int32_t len;
        readBasicType(len);
        if (len >= 0)
        for (int32_t i = 0; (i < len) && (i < count); i++)
            *this >> data[i];

        return *this;
    }

    template <typename _type>
    inline Deserializer & readBasicVector(std::vector<_type> &);

    template <typename _container>
    Deserializer & readContainer(_container &data)
    {
        int32_t len;
        readBasicType(len);
        if (len >= 0)
        {
            data.resize(len);
            for (auto &i : data)
                *this >> i;
        }

        return *this;
    }

    template <typename _type, int32_t _count> Deserializer & read(_type (& data)[_count]) { return readArray(data, _count); }
    template <typename _type, int32_t _count> Deserializer & read(std::array<_type, _count> &data) { return readArray(data.data(), _count); }
    template <typename _type> Deserializer & read(std::vector<_type> &data) { return readContainer(data); }
    template <typename _type> Deserializer & read(std::list<_type> &data) { return readContainer(data); }

    template <typename _type> Deserializer & read(std::set<_type> &data) 
    {
        int32_t len;
        readBasicType(len);

        for (int32_t i = 0; i < len; i++)
        {
            _type value;
            *this >> value;
            data.insert(value);
        }

        return *this;
    }

///////////////////////////////////////////////////////////////////////////////
// Serialization of enums.
    template <bool _enum>
    struct EnumSelector
    {
        template <typename _type>
        static Deserializer & read(Deserializer &d, _type &val)
        {
            return d.read(val);
        }
    };

private:
    Deserializer(const Deserializer &);
    Deserializer & operator=(const Deserializer &);

private:
    const std::vector<uint8_t> &buffer;
    size_t readPos;
    Remoting1::BulkChannel * const bulkChannel;
#endif // __INTELLISENSE__
};

#if !defined(__INTELLISENSE__) || defined(REMOTINGTS1_BUILD) // Hide internals from IntelliSense

// Optimized specializations
template <> Deserializer & Deserializer::read<uint8_t	>(uint8_t	*data, size_t size);
template <> Deserializer & Deserializer::read<int8_t	>(int8_t	*data, size_t size);
template <> Deserializer & Deserializer::read<char		>(char		*data, size_t size);

template <> Deserializer & Deserializer::readArray<int8_t	>(int8_t	*data, int32_t count);
template <> Deserializer & Deserializer::readArray<uint8_t	>(uint8_t	*data, int32_t count);
template <> Deserializer & Deserializer::readArray<int16_t	>(int16_t	*data, int32_t count);
template <> Deserializer & Deserializer::readArray<uint16_t	>(uint16_t	*data, int32_t count);
template <> Deserializer & Deserializer::readArray<int32_t	>(int32_t	*data, int32_t count);
template <> Deserializer & Deserializer::readArray<uint32_t	>(uint32_t	*data, int32_t count);
template <> Deserializer & Deserializer::readArray<int64_t	>(int64_t	*data, int32_t count);
template <> Deserializer & Deserializer::readArray<uint64_t	>(uint64_t	*data, int32_t count);
template <> Deserializer & Deserializer::readArray<bool		>(bool		*data, int32_t count);
template <> Deserializer & Deserializer::readArray<char		>(char		*data, int32_t count);
template <> Deserializer & Deserializer::readArray<float	>(float		*data, int32_t count);
template <> Deserializer & Deserializer::readArray<double	>(double	*data, int32_t count);

template <> Deserializer & Deserializer::readContainer<std::vector<int8_t	>>(std::vector<int8_t	> &data);
template <> Deserializer & Deserializer::readContainer<std::vector<uint8_t	>>(std::vector<uint8_t	> &data);
template <> Deserializer & Deserializer::readContainer<std::vector<int16_t	>>(std::vector<int16_t	> &data);
template <> Deserializer & Deserializer::readContainer<std::vector<uint16_t	>>(std::vector<uint16_t	> &data);
template <> Deserializer & Deserializer::readContainer<std::vector<int32_t	>>(std::vector<int32_t	> &data);
template <> Deserializer & Deserializer::readContainer<std::vector<uint32_t	>>(std::vector<uint32_t	> &data);
template <> Deserializer & Deserializer::readContainer<std::vector<int64_t	>>(std::vector<int64_t	> &data);
template <> Deserializer & Deserializer::readContainer<std::vector<uint64_t	>>(std::vector<uint64_t	> &data);
template <> Deserializer & Deserializer::readContainer<std::vector<char		>>(std::vector<char		> &data);
template <> Deserializer & Deserializer::readContainer<std::vector<float	>>(std::vector<float	> &data);
template <> Deserializer & Deserializer::readContainer<std::vector<double	>>(std::vector<double	> &data);

// Special version as std::vector<bool> is a bit array and needs to be treated slightly different.
template <> Deserializer & Deserializer::readContainer< std::vector<bool> >(std::vector<bool> &);

// This specialization will be used for enums.
template <> 
struct Deserializer::EnumSelector<true>  
{ 
    template <typename _type>
    static Deserializer & read(Deserializer &d, _type &val)
    {
        typename std::underlying_type<_type>::type v;
        Deserializer &result = d.readBasicType(v);
        val = _type(v);
        return result;
    }
};

constexpr uint8_t uninitialized = 0xCC;
constexpr size_t clearSize = 1048576; //!< Number of bytes to clear with uninitialized.

template <typename _type>
Deserializer & Deserializer::readBasicType(_type &val)
{
    if (buffer.size() >= readPos + sizeof(val))
    {
        _type result;
        std::memcpy(&result, &buffer[readPos], sizeof(result));

        val = eswap(result);
        readPos += sizeof(result);
    }
    else // Mark uninitialized.
        memset(&val, uninitialized, std::min(sizeof(val), clearSize));

    return *this;
}

#endif // __INTELLISENSE__

} } // End of namespaces

#endif
