#pragma once
/*-----------------------------------------------------------------------------
| Copyright: Copyright(c) 2014, Philips Medical Systems Nederland B.V.       |
| Author:    A.J. Admiraal, iXR Innovation                                   |
-----------------------------------------------------------------------------*/

#include <cstdint>

#if defined(__unix__)
# include <endian.h>
#elif defined(__VXWORKS__)
# define __LITTLE_ENDIAN	1234
# define __BYTE_ORDER       __LITTLE_ENDIAN // Assumption: VxWorks is always little-endian
#elif defined(_WIN32)
// These intrinsics are used to do byteswapping, in release mode these are
// replaced by the "bswap" instruction by the compiler.
unsigned short __cdecl _byteswap_ushort(unsigned short);
unsigned long __cdecl _byteswap_ulong(unsigned long);
unsigned __int64 __cdecl _byteswap_uint64(unsigned __int64);
#endif

#if defined(__GNUC__) && ((__GNUC__ < 4) || ((__GNUC__ == 4) && ((__GNUC_MINOR__ < 2) || (__GNUC_MINOR__ == 7))))
inline static uint16_t		__builtin_bswap16(uint16_t data) { return ((data & 0x00FF) << 8) | ((data & 0xFF00) >> 8); }
inline static uint32_t		__builtin_bswap32(uint32_t data) { return ((data & 0x000000FF) << 24) | ((data & 0x0000FF00) << 8) | ((data & 0x00FF0000) >> 8) | ((data & 0xFF000000) >> 24); }
inline static uint64_t		__builtin_bswap64(uint64_t data) { return ((data & 0x00000000000000FFULL) << 56) | ((data & 0x000000000000FF00ULL) << 40) | ((data & 0x0000000000FF0000ULL) << 24) | ((data & 0x00000000FF000000ULL) << 8) | ((data & 0x000000FF00000000ULL) >> 8) | ((data & 0x0000FF0000000000ULL) >> 24) | ((data & 0x00FF000000000000ULL) >> 40) | ((data & 0xFF00000000000000ULL) >> 56); }
#endif

static inline int8_t	eswap(int8_t		data) { return data; }
static inline uint8_t	eswap(uint8_t		data) { return data; }
static inline bool		eswap(bool			data) { return data; }
static inline char		eswap(char			data) { return data; }

#if (defined(__BYTE_ORDER) && (__BYTE_ORDER == __BIG_ENDIAN))
static inline int16_t	eswap(int16_t		data) { return data; }
static inline uint16_t	eswap(uint16_t		data) { return data; }
static inline int32_t	eswap(int32_t		data) { return data; }
static inline uint32_t	eswap(uint32_t		data) { return data; }
static inline int64_t	eswap(int64_t		data) { return data; }
static inline uint64_t	eswap(uint64_t		data) { return data; }
static inline wchar_t	eswap(wchar_t		data) { return data; }
static inline float		eswap(float			data) { return data; }
static inline double	eswap(double		data) { return data; }
#elif (defined(__GNUC__) && (defined(__BYTE_ORDER) && (__BYTE_ORDER == __LITTLE_ENDIAN)))
static inline int16_t	eswap(int16_t		data) { return int16_t(__builtin_bswap16(data)); }
static inline uint16_t	eswap(uint16_t		data) { return uint16_t(__builtin_bswap16(data)); }
static inline int32_t	eswap(int32_t		data) { return int32_t(__builtin_bswap32(data)); }
static inline uint32_t	eswap(uint32_t		data) { return uint32_t(__builtin_bswap32(data)); }
static inline int64_t	eswap(int64_t		data) { return int64_t(__builtin_bswap64(data)); }
static inline uint64_t	eswap(uint64_t		data) { return uint64_t(__builtin_bswap64(data)); }
static inline wchar_t	eswap(wchar_t		data) { return wchar_t(__builtin_bswap32(data)); }
static inline float		eswap(float			data) { union { uint32_t dw; float  v; } e; e.v = data; e.dw = eswap(e.dw); return e.v; }
static inline double	eswap(double		data) { union { uint64_t qw; double v; } e; e.v = data; e.qw = eswap(e.qw); return e.v; }
#elif (defined(__GNUC__) && (defined(__BYTE_ORDER) && (__BYTE_ORDER == __PDP_ENDIAN)))
static inline int16_t	eswap(int16_t		data) { return int16_t(__builtin_bswap16(data)); }
static inline uint16_t	eswap(uint16_t		data) { return uint16_t(__builtin_bswap16(data)); }
static inline int32_t	eswap(int32_t		data) { union { uint16_t w[2]; int32_t  v; } e; e.v = data; for (auto &i : e.w) i = eswap(i); return e.v; }
static inline uint32_t	eswap(uint32_t		data) { union { uint16_t w[2]; uint32_t v; } e; e.v = data; for (auto &i : e.w) i = eswap(i); return e.v; }
static inline int64_t	eswap(int64_t		data) { union { uint16_t w[4]; int64_t  v; } e; e.v = data; for (auto &i : e.w) i = eswap(i); return e.v; }
static inline uint64_t	eswap(uint64_t		data) { union { uint16_t w[4]; uint64_t v; } e; e.v = data; for (auto &i : e.w) i = eswap(i); return e.v; }
static inline wchar_t	eswap(wchar_t		data) { return wchar_t(eswap(uint32_t(data))); }
static inline float		eswap(float			data) { union { uint32_t dw; float  v; } e; e.v = data; e.dw = eswap(e.dw); return e.v; }
static inline double	eswap(double		data) { union { uint64_t qw; double v; } e; e.v = data; e.qw = eswap(e.qw); return e.v; }
#elif defined(_WIN32) // Assumption: Microsoft is always little-endian
static inline int16_t	eswap(int16_t		data) { return static_cast<int16_t>(_byteswap_ushort(static_cast<uint16_t>(data))); }
static inline uint16_t	eswap(uint16_t		data) { return uint16_t(_byteswap_ushort(data)); }
static inline int32_t	eswap(int32_t		data) { return static_cast<int32_t>(_byteswap_ulong(static_cast<uint32_t>(data))); }
static inline uint32_t	eswap(uint32_t		data) { return uint32_t(_byteswap_ulong(data)); }
static inline int64_t	eswap(int64_t		data) { return static_cast<int64_t>(_byteswap_uint64(static_cast<uint64_t>(data))); }
static inline uint64_t	eswap(uint64_t		data) { return uint64_t(_byteswap_uint64(data)); }
#ifdef _NATIVE_WCHAR_T_DEFINED
static inline wchar_t	eswap(wchar_t		data) { return wchar_t(_byteswap_ushort(data)); }
#endif
static inline float		eswap(float			data) { union { uint32_t dw; float  v; } e; e.v = data; e.dw = eswap(e.dw); return e.v; }
static inline double	eswap(double		data) { union { uint64_t qw; double v; } e; e.v = data; e.qw = eswap(e.qw); return e.v; }
#else
# error Unsupported platform.
#endif

#if (!defined(_WIN32) && defined(_LP64))
static inline signed long long		eswap(signed long long   data) { return (signed long long)  eswap(int64_t(data)); }
static inline unsigned long long	eswap(unsigned long long data) { return (unsigned long long)eswap(uint64_t(data)); }
#endif

