// MD5 computation

#include "crypto/MD5.h"

#include <sstream>

using namespace std;
using namespace Crypto;

static constexpr uint64_t Mod512Mask = 0x00000000000001FF;
static constexpr uint64_t Mod512_448 = 0x00000000000001C0;
static constexpr size_t NumWorkspace = 16;
static constexpr size_t NumWorkspaceMinusOne = NumWorkspace - 1;
static constexpr size_t NumRounds = 64;

union Crypto::MD5::WorkspaceBlock
{
    uint8_t c[MD5::BlockSize];
    Word l[16];
};

const MD5::Word MD5::K[MD5::BlockSize] =
{
    0XD76AA478, 0XE8C7B756, 0X242070DB, 0XC1BDCEEE,
    0XF57C0FAF, 0X4787C62A, 0XA8304613, 0XFD469501,
    0X698098D8, 0X8B44F7AF, 0XFFFF5BB1, 0X895CD7BE,
    0X6B901122, 0XFD987193, 0XA679438E, 0X49B40821,
    0XF61E2562, 0XC040B340, 0X265E5A51, 0XE9B6C7AA,
    0XD62F105D, 0X02441453, 0XD8A1E681, 0XE7D3FBC8,
    0X21E1CDE6, 0XC33707D6, 0XF4D50D87, 0X455A14ED,
    0XA9E3E905, 0XFCEFA3F8, 0X676F02D9, 0X8D2A4C8A,
    0XFFFA3942, 0X8771F681, 0X6D9D6122, 0XFDE5380C,
    0XA4BEEA44, 0X4BDECFA9, 0XF6BB4B60, 0XBEBFBC70,
    0X289B7EC6, 0XEAA127FA, 0XD4EF3085, 0X04881D05,
    0XD9D4D039, 0XE6DB99E5, 0X1FA27CF8, 0XC4AC5665,
    0XF4292244, 0X432AFF97, 0XAB9423A7, 0XFC93A039,
    0X655B59C3, 0X8F0CCC92, 0XFFEFF47D, 0X85845DD1,
    0X6FA87E4F, 0XFE2CE6E0, 0XA3014314, 0X4E0811A1,
    0XF7537E82, 0XBD3AF235, 0X2AD7D2BB, 0XEB86D391,
};

MD5::MD5()
    : _digest()
    , _buffer()
    , _bitCount()
    , _state()
    , _workspace()
    , _block(reinterpret_cast<WorkspaceBlock *>(_workspace))
{
    Initialize();
}

void MD5::Initialize()
{
    _bitCount = 0;
    _state[0] = 0x67452301;
    _state[1] = 0xEFCDAB89;
    _state[2] = 0x98BADCFE;
    _state[3] = 0x10325476;
}

// Update the hash value
void MD5::Process(const uint8_t *data, size_t len)
{
    uint32_t filledBlock = static_cast<uint32_t>((_bitCount >> 3) & BlockSizeMinusOne);
    // Calculate bytes mod 64 or bit mod 512

    _bitCount += (static_cast<uint64_t>(len) << 3);         // Add number of bits for this block
    // (we expect no more than 2^64 bits in total)

    uint32_t offset {};
    if ((filledBlock + len) > BlockSizeMinusOne)            // If total number of bytes crosses 64 (i.e. if we now have a complete 512 bit block)
    {
        uint32_t spaceLeft = static_cast<uint32_t>(BlockSize - filledBlock);
        // Space left in block
        memcpy(&_buffer[filledBlock], data, spaceLeft);
        // Fill up block;
        Transform(_buffer);
        // Calculate
        offset += spaceLeft;

        while (offset + BlockSizeMinusOne < len)
        {
            Transform(&data[offset]);
            offset += BlockSize;
        }

        filledBlock = 0;
    }

    memcpy(&_buffer[filledBlock], &data[offset], len - offset);
}

void MD5::Process(const Core::ByteArray & data)
{
    Process(data.Data(), data.Size());
}

void MD5::Finalize()
{
    // For _bitCount into a 64 bit number
    uint8_t finalCount[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    // Suppose that the length of the message, M, is l bits. Append the bit “1” to the end of the
    // message, followed by k zero bits, where k is the smallest, non-negative solution to the equation
    // l + 1 + k ≡ 448 mod 512 . Then append the 64-bit block that is equal to the number l expressed
    // using a binary representation.
    // A "1" bit followed by 7 "0" bits
    Process(reinterpret_cast<const uint8_t *>("\x80"), 1);

    // Fill up to 448 bits with "0" bits
    while ((_bitCount & Mod512Mask) != Mod512_448)
        Process(reinterpret_cast<const uint8_t *>("\x00"), 1);

    for (size_t i = 0; i < sizeof(finalCount); i++)
        // Endian independent
        finalCount[i] = static_cast<uint8_t>((_bitCount >> ((7 - (i & 7)) * 8) ) & 255);

    // Add l = _bitCount as 64 bit integer
    Process(finalCount, sizeof(finalCount));

    for (size_t i = 0; i < DigestSize; i++)
    {
        // Convert 4 32 bit integers to 16 bytes in BIG ENDIAN form
        _digest[i] = static_cast<uint8_t>((_state[i >> 2] >> ((3 - (i & 3)) * 8) ) & 255);
    }

    memset(_buffer, 0, BlockSize);
    memset(_state, 0, sizeof(_state));
    _bitCount = 0;
    memset(finalCount, 0, sizeof(finalCount));
}

Core::ByteArray MD5::GetDigest() const
{
    Core::ByteArray digest(_digest, sizeof(_digest));
    return digest;
}

OSAL::String MD5::ToString() const
{
    basic_ostringstream<OSAL::Char> stream;

    for (size_t i = 0; i < sizeof(_digest); ++i)
    {
        stream << uppercase << hex << setw(2) << setfill('0') << static_cast<int>(_digest[i]);
    }
    return stream.str();
}

MD5::Word MD5::F(MD5::Word x, MD5::Word y, MD5::Word z)
{
    return (x & y) | (~x & z);
}

MD5::Word MD5::G(MD5::Word x, MD5::Word y, MD5::Word z)
{
    return (x & z) | (y & ~z);
}

MD5::Word MD5::H(MD5::Word x, MD5::Word y, MD5::Word z)
{
    return x ^ y ^ z;
}

MD5::Word MD5::I(MD5::Word x, MD5::Word y, MD5::Word z)
{
    return y ^ (x | ~z);
}

// Rotate x bits to the left
inline MD5::Word MD5::ROTL(MD5::Word value, size_t bits)
{
    return (((value)<<(bits))|((value)>>(MD5::WordLength-(bits))));
}

// Wt = Mt                                                  0 <= t <= 15
#ifdef LITTLE_ENDIAN
// Swap bytes to for BIG ENDIAN value
inline MD5::Word MD5::GetData(const WorkspaceBlock * block, size_t i)
{
    return (ROTL(block->l[i], 24) & 0xFF00FF00) | (ROTL(block->l[i], 8) & 0x00FF00FF);
}
#else
inline SHA1::Word SHABLK0(SHA1::WorkspaceBlock & block, size_t i)
{
    return block->l[i];
}
#endif

void MD5::Transform(const uint8_t buffer[BlockSize])
{
    memcpy(_block, buffer, sizeof(WorkspaceBlock::c));

    // Copy state[] to working vars
    Word a = _state[0];
    Word b = _state[1];
    Word c = _state[2];
    Word d = _state[3];
    Word e = _state[4];

    for (size_t i = 0; i < NumWorkspace; ++i)
    {
        _block->l[i] = GetData(_block, i);
    }
    for (size_t i = 0; i < NumRounds; ++i)
    {

    }
    // Add the working vars back into state[]
    _state[0] += a;
    _state[1] += b;
    _state[2] += c;
    _state[3] += d;
    _state[4] += e;
}
