#pragma once

#include <chrono>
#include <sstream>
#include <osal/Strings.h>
#if !defined(WIN_MSVC)
#include <osal/linux/DomainSocketAddress.h>
#endif
#include <osal/IPV4Address.h>
#include <osal/IPV6Address.h>
#include <osal/MACAddress.h>
#include <core/exports.h>
//#include <byteswap.h>

namespace Core
{

// forward declarations
class ByteArray;

namespace Util
{

CORE_EXPORT size_t NextPowerOfTwo(size_t value);

//template<class T> T BitsToBytes(T numBits) {
//    return (numBits + 7) / 8;
//}
//template<class T> T SwapBytes(T value);
//template<class T> T Clip(T value, T min, T max)
//{
//    return std::min(max, std::max(min, value));
//}
timespec MilliSecondsToTimespec(int ms);
//timespec ConvertTimeOffsetToTimeSpec(int offsetMS);
//timespec ConvertDurationToTimeSpec(std::chrono::milliseconds duration);
//timespec ConvertDurationToTimeSpec(std::chrono::microseconds duration);
//timespec ConvertDurationToTimeSpec(std::chrono::nanoseconds duration);
CORE_EXPORT void Sleep(int sleepMS);
//int KBHit(void);
//
//int NumDigits(int64_t);
//
//void ZeroMemory(void * destination, int size);
//void ZeroMemory(void * destination, int offset, int size);
//void CopyMemory(void * destination, const void * source, int size);
//void CopyMemory(void * destination, int offsetDestination, const void * source, int size);
//void CopyMemory(void * destination, const void * source, int offsetSource, int size);
//void CopyMemory(void * destination, int offsetDestination, const void * source, int offsetSource, int size);
//
//void DumpToStream(std::ostream & stream, const ByteArray & data);
//void DumpToStream(std::ostream & stream, const uint8_t * data, int length);
//
//std::string Base64Encode(const ByteArray & data);
//ByteArray Base64Decode(const std::string & dataBase64);
//ByteArray Base64Decode(const char * dataBase64);
//
//bool TryParse(const std::string & text, bool & value);
//bool TryParse(const std::string & text, int8_t & value, int base = 10);
//bool TryParse(const std::string & text, uint8_t & value, int base = 10);
//bool TryParse(const std::string & text, int16_t & value, int base = 10);
//bool TryParse(const std::string & text, uint16_t & value, int base = 10);
//bool TryParse(const std::string & text, int32_t & value, int base = 10);
//bool TryParse(const std::string & text, uint32_t & value, int base = 10);
//bool TryParse(const std::string & text, int64_t & value, int base = 10);
//bool TryParse(const std::string & text, uint64_t & value, int base = 10);
//bool TryParse(const std::string & text, float & value);
//bool TryParse(const std::string & text, double & value);
//inline bool TryParse(const std::string & text, std::string & value)
//{
//    value = text;
//    return true;
//}
//std::string ToString(bool value);
//std::string ToString(uint8_t value, int base = 10);
//std::string ToString(int16_t value, int base = 10);
//std::string ToString(uint16_t value, int base = 10);
//std::string ToString(int32_t value, int base = 10);
//std::string ToString(uint32_t value, int base = 10);
//std::string ToString(int64_t value, int base = 10);
//std::string ToString(uint64_t value, int base = 10);
//std::string ToString(float value, int precision = 16);
//std::string ToString(double value, int precision = 16);
//std::string ToString(std::string value, bool quote = true);
//template <class T> std::string ToString(T value)
//{
//    std::ostringstream stream;
//
//    stream << value;
//
//    return stream.str();
//}
//
//bool CompareFiles(const char * pathA, const char * pathB);
//bool CompareFiles(const std::string & pathA, const std::string & pathB);
//bool CompareTextFiles(const char * pathA, const char * pathB);
//bool CompareTextFiles(const std::string & pathA, const std::string & pathB);
//bool CompareLogFiles(const char * pathA, const char * pathB);
//bool CompareLogFiles(const std::string & pathA, const std::string & pathB);
//
//bool Compare(double expected, double actual, double epsilonAbs, double epsilonRel);
//bool Compare(float expected, float actual, float epsilonAbs, float epsilonRel);
//
template<class T>
bool Compare(T const * expected, int offsetExpected,
             T const * actual, int offsetActual, int numSamples)
{
    if ((offsetExpected < 0) || (offsetActual < 0) || (numSamples < 0))
    {
        return false;
    }
    for (int i = 0; i < numSamples; i++)
    {
        if (expected[i + offsetExpected] != actual[i + offsetActual])
            return false;
    }
    return true;
}

template<class T>
bool Compare(T const * expected, T const * actual, int numSamples)
{
    return Compare(expected, 0, actual, 0, numSamples);
}

//template<class T>
//typename std::enable_if<std::is_floating_point<T>::value, bool>::type
//Compare(T const * expected, int offsetExpected,
//        T const * actual, int offsetActual, int numSamples,
//        T epsilonAbs, T epsilonRel)
//{
//    if ((offsetExpected < 0) || (offsetActual < 0) || (numSamples < 0))
//    {
//        return false;
//    }
//    for (int i = 0; i < numSamples; i++)
//    {
//        if (!Compare(expected[i + offsetExpected],
//                     actual[i + offsetActual],
//                     epsilonAbs, epsilonRel))
//            return false;
//    }
//    return true;
//}
//
//template<class T>
//typename std::enable_if<std::is_floating_point<T>::value, bool>::type
//Compare(T const * expected, T const * actual, int numSamples,
//        T epsilonAbs, T epsilonRel)
//{
//    return Compare(expected, 0, actual, 0, numSamples, epsilonAbs, epsilonRel);
//}
//
//template<class T>
//bool Compare(T const * const * expected, int offsetExpectedRows, int offsetExpectedCols,
//             T const * const * actual, int offsetActualRows, int offsetActualCols,
//             int numRows, int numCols)
//{
//    if ((offsetExpectedRows < 0) || (offsetExpectedCols < 0) ||
//        (offsetActualRows < 0) || (offsetActualCols < 0) ||
//        (numRows < 0) || (numCols < 0))
//    {
//        return false;
//    }
//    for (int i = 0; i < numRows; i++)
//    {
//        for (int j = 0; j < numCols; j++)
//        {
//            if (expected[i + offsetExpectedRows][j + offsetExpectedCols] !=
//                actual[i + offsetActualRows][j + offsetActualCols])
//                return false;
//        }
//    }
//    return true;
//}
//
//template<class T>
//bool Compare(T const * const * expected, T const * const * actual, int numRows, int numCols)
//{
//    return Compare(expected, 0, 0, actual, 0, 0, numRows, numCols);
//}
//
//template<class T>
//typename std::enable_if<std::is_floating_point<T>::value, bool>::type
//Compare(T const * const * expected, int offsetExpectedRows, int offsetExpectedCols,
//        T const * const * actual, int offsetActualRows, int offsetActualCols,
//        int numRows, int numCols,
//        T epsilonAbs, T epsilonRel)
//{
//    if ((offsetExpectedRows < 0) || (offsetExpectedCols < 0) ||
//        (offsetActualRows < 0) || (offsetActualCols < 0) ||
//        (numRows < 0) || (numCols < 0))
//    {
//        return false;
//    }
//    for (int i = 0; i < numRows; i++)
//    {
//        for (int j = 0; j < numCols; j++)
//        {
//            if (!Compare(expected[i + offsetExpectedRows][j + offsetExpectedCols],
//                         actual[i + offsetActualRows][j + offsetActualCols],
//                         epsilonAbs, epsilonRel))
//                return false;
//        }
//    }
//    return true;
//}
//
//template<class T>
//typename std::enable_if<std::is_floating_point<T>::value, bool>::type
//Compare(T const * const * expected, T const * const * actual,
//        int numRows, int numCols,
//        T epsilonAbs, T epsilonRel)
//{
//    return Compare(expected, 0, 0, actual, 0, 0, numRows, numCols, epsilonAbs, epsilonRel);
//}
//
//template<> inline int8_t SwapBytes<int8_t>(int8_t value)
//{
//    return value;
//}
//template<> inline uint8_t SwapBytes<uint8_t>(uint8_t value)
//{
//    return value;
//}
//template<> inline int16_t SwapBytes<int16_t>(int16_t value)
//{
//    return (int16_t)bswap_16(value);
//}
//template<> inline uint16_t SwapBytes<uint16_t>(uint16_t value)
//{
//    return bswap_16(value);
//}
//template<> inline int32_t SwapBytes<int32_t>(int32_t value)
//{
//    return (int32_t)bswap_32(value);
//}
//template<> inline uint32_t SwapBytes<uint32_t>(uint32_t value)
//{
//    return bswap_32(value);
//}
//template<> inline int64_t SwapBytes<int64_t>(int64_t value)
//{
//    return (int64_t)bswap_64(value);
//}
//template<> inline uint64_t SwapBytes<uint64_t>(uint64_t value)
//{
//    return bswap_64(value);
//}
//
//std::streambuf & serialize_out(std::streambuf & sb, void * p, int size);
//std::streambuf & serialize_in(std::streambuf & sb, void * p, int size);

#if !defined(WIN_MSVC)
inline bool TryParse(const OSAL::String & text, OSAL::Network::DomainSocketAddress & ipAddress)
{
    return OSAL::Network::DomainSocketAddress::TryParse(text, ipAddress);
}
#endif

inline bool TryParse(const OSAL::String & text, OSAL::Network::IPV4Address & ipAddress)
{
    return OSAL::Network::IPV4Address::TryParse(text, ipAddress);
}

inline bool TryParse(const OSAL::String & text, OSAL::Network::IPV6Address & ipAddress)
{
    return OSAL::Network::IPV6Address::TryParse(text, ipAddress);
}

inline bool TryParse(const OSAL::String & text, OSAL::Network::MACAddress & ipAddress)
{
    return OSAL::Network::MACAddress::TryParse(text, ipAddress);
}

} // namespace Util
} // namespace Core

