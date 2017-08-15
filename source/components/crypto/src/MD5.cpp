// MD5 computation

#include "crypto/MD5.h"

#include <iomanip>
#include <iostream>

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
    _state[0] = 0x67452301;
    _state[1] = 0xEFCDAB89;
    _state[2] = 0x98BADCFE;
    _state[3] = 0x10325476;

    _bitCount = 0;

    memset(_digest, 0, sizeof(_digest));
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

void MD5::Process(const OSAL::ByteArray & data)
{
    Process(data.Data(), data.Size());
}

void MD5::Finalize()
{
    // For _bitCount into a 64 bit number
    uint8_t finalCount[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    for (size_t i = 0; i < sizeof(finalCount); i++)
        // Endian independent
        finalCount[i] = static_cast<uint8_t>((_bitCount >> ((i & 7) * 8) ) & 255);

    // Suppose that the length of the message, M, is l bits. Append the bit “1” to the end of the
    // message, followed by k zero bits, where k is the smallest, non-negative solution to the equation
    // l + 1 + k ≡ 448 mod 512 . Then append the 64-bit block that is equal to the number l expressed
    // using a binary representation.
    // A "1" bit followed by 7 "0" bits
    Process(reinterpret_cast<const uint8_t *>("\x80"), 1);

    // Fill up to 448 bits with "0" bits
    while ((_bitCount & Mod512Mask) != Mod512_448)
        Process(reinterpret_cast<const uint8_t *>("\x00"), 1);

    // Add l = _bitCount as 64 bit integer
    Process(finalCount, sizeof(finalCount));

    for (size_t i = 0; i < DigestSize; i++)
    {
        // Convert 4 32 bit integers to 16 bytes in LITTLE ENDIAN form
        _digest[i] = static_cast<uint8_t>((_state[i >> 2] >> ((i & 3) * 8) ) & 255);
    }

    memset(_buffer, 0, BlockSize);
    memset(_state, 0, sizeof(_state));
    _bitCount = 0;
    memset(finalCount, 0, sizeof(finalCount));
}

OSAL::ByteArray MD5::GetDigest() const
{
    OSAL::ByteArray digest(_digest, sizeof(_digest));
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

void MD5::DumpState()
{
    cout << "State: " << endl;
    for (size_t i = 0; i < StateSize; ++i)
    {
        cout << setw(2) << setfill('0') << i << " = " << hex << setw(8) << setfill('0') << uppercase << _state[i] << dec
             << endl;
    }
}

void MD5::DumpState(size_t t, Word a, Word b, Word c, Word d)
{
    cout << "t = " << setw(2) << t << "  "
         << "A = " << hex << setw(8) << setfill('0') << uppercase << a << dec << " "
         << "B = " << hex << setw(8) << setfill('0') << uppercase << b << dec << " "
         << "C = " << hex << setw(8) << setfill('0') << uppercase << c << dec << " "
         << "D = " << hex << setw(8) << setfill('0') << uppercase << d << dec << endl;
}

void MD5::DumpBlock(MD5::WorkspaceBlock * block)
{
    cout << "W: " << endl;
    for (size_t i = 0; i < NumWorkspace; ++i)
    {
        cout << "W" << setw(2) << setfill('0') << i << " = " << hex << setw(8) << setfill('0') << uppercase
             << block->l[i] << dec << endl;
    }
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

void MD5::FF(MD5::Word & a, MD5::Word b, MD5::Word c, MD5::Word d, MD5::Word m, size_t s, MD5::Word t)
{
    a += F(b,c,d) + m + t;
    a = b + ROTLEFT(a,s);
}
void MD5::GG(MD5::Word & a, MD5::Word b, MD5::Word c, MD5::Word d, MD5::Word m, size_t s, MD5::Word t)
{
    a += G(b,c,d) + m + t;
    a = b + ROTLEFT(a,s);
}
void MD5::HH(MD5::Word & a, MD5::Word b, MD5::Word c, MD5::Word d, MD5::Word m, size_t s, MD5::Word t)
{
    a += H(b,c,d) + m + t;
    a = b + ROTLEFT(a,s);
}
void MD5::II(MD5::Word & a, MD5::Word b, MD5::Word c, MD5::Word d, MD5::Word m, size_t s, MD5::Word t)
{
    a += I(b,c,d) + m + t;
    a = b + ROTLEFT(a,s);
}

// Rotate x bits to the left
inline MD5::Word MD5::ROTLEFT(MD5::Word value, size_t bits)
{
    return (((value)<<(bits))|((value)>>(MD5::WordLength-(bits))));
}

// Wt = Mt                                                  0 <= t <= 15
#ifdef LITTLE_ENDIAN
// Swap bytes to for BIG ENDIAN value
inline MD5::Word MD5::GetData(const WorkspaceBlock * block, size_t i)
{
    return block->l[i];
}
#else
inline MD5::Word MD5::GetData(const WorkspaceBlock * block, size_t i)
{
    return (ROTLEFT(block->l[i], 24) & 0xFF00FF00) | (ROTLEFT(block->l[i], 8) & 0x00FF00FF);
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

    for (size_t i = 0; i < NumWorkspace; ++i)
    {
        _block->l[i] = GetData(_block, i);
    }

    FF(a,b,c,d,_block->l[0],  7,0xd76aa478);
    FF(d,a,b,c,_block->l[1], 12,0xe8c7b756);
    FF(c,d,a,b,_block->l[2], 17,0x242070db);
    FF(b,c,d,a,_block->l[3], 22,0xc1bdceee);
    FF(a,b,c,d,_block->l[4],  7,0xf57c0faf);
    FF(d,a,b,c,_block->l[5], 12,0x4787c62a);
    FF(c,d,a,b,_block->l[6], 17,0xa8304613);
    FF(b,c,d,a,_block->l[7], 22,0xfd469501);
    FF(a,b,c,d,_block->l[8],  7,0x698098d8);
    FF(d,a,b,c,_block->l[9], 12,0x8b44f7af);
    FF(c,d,a,b,_block->l[10],17,0xffff5bb1);
    FF(b,c,d,a,_block->l[11],22,0x895cd7be);
    FF(a,b,c,d,_block->l[12], 7,0x6b901122);
    FF(d,a,b,c,_block->l[13],12,0xfd987193);
    FF(c,d,a,b,_block->l[14],17,0xa679438e);
    FF(b,c,d,a,_block->l[15],22,0x49b40821);

    GG(a,b,c,d,_block->l[1],  5,0xf61e2562);
    GG(d,a,b,c,_block->l[6],  9,0xc040b340);
    GG(c,d,a,b,_block->l[11],14,0x265e5a51);
    GG(b,c,d,a,_block->l[0], 20,0xe9b6c7aa);
    GG(a,b,c,d,_block->l[5],  5,0xd62f105d);
    GG(d,a,b,c,_block->l[10], 9,0x02441453);
    GG(c,d,a,b,_block->l[15],14,0xd8a1e681);
    GG(b,c,d,a,_block->l[4], 20,0xe7d3fbc8);
    GG(a,b,c,d,_block->l[9],  5,0x21e1cde6);
    GG(d,a,b,c,_block->l[14], 9,0xc33707d6);
    GG(c,d,a,b,_block->l[3], 14,0xf4d50d87);
    GG(b,c,d,a,_block->l[8], 20,0x455a14ed);
    GG(a,b,c,d,_block->l[13], 5,0xa9e3e905);
    GG(d,a,b,c,_block->l[2],  9,0xfcefa3f8);
    GG(c,d,a,b,_block->l[7], 14,0x676f02d9);
    GG(b,c,d,a,_block->l[12],20,0x8d2a4c8a);

    HH(a,b,c,d,_block->l[5],  4,0xfffa3942);
    HH(d,a,b,c,_block->l[8], 11,0x8771f681);
    HH(c,d,a,b,_block->l[11],16,0x6d9d6122);
    HH(b,c,d,a,_block->l[14],23,0xfde5380c);
    HH(a,b,c,d,_block->l[1],  4,0xa4beea44);
    HH(d,a,b,c,_block->l[4], 11,0x4bdecfa9);
    HH(c,d,a,b,_block->l[7], 16,0xf6bb4b60);
    HH(b,c,d,a,_block->l[10],23,0xbebfbc70);
    HH(a,b,c,d,_block->l[13], 4,0x289b7ec6);
    HH(d,a,b,c,_block->l[0], 11,0xeaa127fa);
    HH(c,d,a,b,_block->l[3], 16,0xd4ef3085);
    HH(b,c,d,a,_block->l[6], 23,0x04881d05);
    HH(a,b,c,d,_block->l[9],  4,0xd9d4d039);
    HH(d,a,b,c,_block->l[12],11,0xe6db99e5);
    HH(c,d,a,b,_block->l[15],16,0x1fa27cf8);
    HH(b,c,d,a,_block->l[2], 23,0xc4ac5665);

    II(a,b,c,d,_block->l[0],  6,0xf4292244);
    II(d,a,b,c,_block->l[7], 10,0x432aff97);
    II(c,d,a,b,_block->l[14],15,0xab9423a7);
    II(b,c,d,a,_block->l[5], 21,0xfc93a039);
    II(a,b,c,d,_block->l[12], 6,0x655b59c3);
    II(d,a,b,c,_block->l[3], 10,0x8f0ccc92);
    II(c,d,a,b,_block->l[10],15,0xffeff47d);
    II(b,c,d,a,_block->l[1], 21,0x85845dd1);
    II(a,b,c,d,_block->l[8],  6,0x6fa87e4f);
    II(d,a,b,c,_block->l[15],10,0xfe2ce6e0);
    II(c,d,a,b,_block->l[6], 15,0xa3014314);
    II(b,c,d,a,_block->l[13],21,0x4e0811a1);
    II(a,b,c,d,_block->l[4],  6,0xf7537e82);
    II(d,a,b,c,_block->l[11],10,0xbd3af235);
    II(c,d,a,b,_block->l[2], 15,0x2ad7d2bb);
    II(b,c,d,a,_block->l[9], 21,0xeb86d391);

    // Add the working vars back into state[]
    _state[0] += a;
    _state[1] += b;
    _state[2] += c;
    _state[3] += d;
}
