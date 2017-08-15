// SHA-1 computation

#include "crypto/SHA1.h"

#include <sstream>

using namespace std;
using namespace Crypto;

static constexpr size_t NumWorkspace = 16;
static constexpr size_t NumWorkspaceMinusOne = NumWorkspace - 1;
union Crypto::SHA1::WorkspaceBlock
{
    uint8_t c[SHA1::BlockSize];
    Word l[NumWorkspace];
};

static constexpr uint64_t Mod512Mask = 0x00000000000001FF;
static constexpr uint64_t Mod512_448 = 0x00000000000001C0;

// SHA-1 rounds
const SHA1::Word SHA1::K0 = 0x5A827999;
const SHA1::Word SHA1::K1 = 0x6ED9EBA1;
const SHA1::Word SHA1::K2 = 0x8F1BBCDC;
const SHA1::Word SHA1::K3 = 0xCA62C1D6;

SHA1::SHA1()
: _state(), _bitCount(), _buffer(), _digest(), _workspace(), _block(reinterpret_cast<WorkspaceBlock *>(_workspace))
{
    Initialize();
}

void SHA1::Initialize()
{
    // SHA1 initialization constants
    _state[0] = 0x67452301;
    _state[1] = 0xEFCDAB89;
    _state[2] = 0x98BADCFE;
    _state[3] = 0x10325476;
    _state[4] = 0xC3D2E1F0;

    _bitCount = 0;

    memset(_digest, 0, sizeof(_digest));
}

// Update the hash value
void SHA1::Process(const uint8_t * data, size_t len)
{
    uint32_t filledBlock = static_cast<uint32_t>((_bitCount >> 3) & BlockSizeMinusOne);
    // Calculate bytes mod 64 or bit mod 512

    _bitCount += (static_cast<uint64_t>(len) << 3);         // Add number of bits for this block
    // (we expect no more than 2^64 bits in total)

    uint32_t offset{};
    if ((filledBlock + len) >
        BlockSizeMinusOne)            // If total number of bytes crosses 64 (i.e. if we now have a complete 512 bit block)
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

void SHA1::Process(const OSAL::ByteArray & data)
{
    Process(data.Data(), data.Size());
}

void SHA1::Finalize()
{
    // For _bitCount into a 64 bit number
    uint8_t finalCount[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    for (size_t i = 0; i < sizeof(finalCount); i++)
        // Endian independent
        finalCount[i] = static_cast<uint8_t>((_bitCount >> ((7 - (i & 7)) * 8)) & 255);

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
        // Convert 5 32 bit integers to 20 bytes in BIG ENDIAN form
        _digest[i] = static_cast<uint8_t>((_state[i >> 2] >> ((3 - (i & 3)) * 8)) & 255);
    }

    memset(_buffer, 0, BlockSize);
    memset(_state, 0, sizeof(_state));
    _bitCount = 0;
    memset(finalCount, 0, sizeof(finalCount));
}

OSAL::ByteArray SHA1::GetDigest() const
{
    OSAL::ByteArray digest(_digest, sizeof(_digest));
    return digest;
}

// Rotate x bits to the left
inline SHA1::Word SHA1::ROTL(SHA1::Word value, size_t bits)
{
    return (((value) << (bits)) | ((value) >> (SHA1::WordLength - (bits))));
}

// Wt = Mt                                                  0 <= t <= 15
#ifdef LITTLE_ENDIAN

// Swap bytes to for BIG ENDIAN value
inline SHA1::Word SHA1::SHABLK0(SHA1::WorkspaceBlock * block, size_t i)
{
    block->l[i] = (ROTL(block->l[i], 24) & 0xFF00FF00) | (ROTL(block->l[i], 8) & 0x00FF00FF);
    return block->l[i];
}

#else
inline SHA1::Word SHA1::SHABLK0(SHA1::WorkspaceBlock * block, size_t i)
{
    return block->l[i];
}
#endif

//                                                          16 <= t <= 79
// s = t & 0x0000000F
// W s = ROTL 1 ( W ( s + 13 ) ∧ MASK ⊕ W ( s + 8 ) ∧ MASK ⊕ W ( s + 2 ) ∧ MASK ⊕ W s )
inline SHA1::Word SHA1::SHABLK(SHA1::WorkspaceBlock * block, size_t i)
{
    block->l[i & NumWorkspaceMinusOne] = (ROTL(
    block->l[(i + 13) & NumWorkspaceMinusOne] ^ block->l[(i + 8) & NumWorkspaceMinusOne] ^
    block->l[(i + 2) & NumWorkspaceMinusOne] ^ block->l[i & NumWorkspaceMinusOne], 1));
    return block->l[i & NumWorkspaceMinusOne];
}

// Ch(x, y, z)=(x ∧ y) ⊕ ( ¬ x ∧ z)             0 ≤ t ≤ 19
// First set is for uninitialized data
// T = ROTL 5 ( a ) + f t ( b , c , d ) + e + K t + W t
// e = d
// d = c
// c = ROTL 30 ( b )
// b = a
// a = T
// Variable rotation is done while calling the macro, and variables take the initial place for:
// a : v, b : w, c : x, d : y, e : z
inline void SHA1::Round0(SHA1::WorkspaceBlock * block, SHA1::Word & v, SHA1::Word & w, SHA1::Word & x, SHA1::Word & y,
                         SHA1::Word & z, size_t i)
{
    z += ((w & (x ^ y)) ^ y) + SHABLK0(block, i) + SHA1::K0 + ROTL(v, 5);
    w = ROTL(w, 30);
}

inline void SHA1::Round1(SHA1::WorkspaceBlock * block, SHA1::Word & v, SHA1::Word & w, SHA1::Word & x, SHA1::Word & y,
                         SHA1::Word & z, size_t i)
{
    z += ((w & (x ^ y)) ^ y) + SHABLK(block, i) + SHA1::K0 + ROTL(v, 5);
    w = ROTL(w, 30);
}

// Parity(x, y, z)=x ⊕ y ⊕ z                    20 ≤ t ≤ 39
inline void SHA1::Round2(SHA1::WorkspaceBlock * block, SHA1::Word & v, SHA1::Word & w, SHA1::Word & x, SHA1::Word & y,
                         SHA1::Word & z, size_t i)
{
    z += (w ^ x ^ y) + SHABLK(block, i) + SHA1::K1 + ROTL(v, 5);
    w = ROTL(w, 30);
}

// Maj(x, y, z)=(x ∧ y) ⊕ (x ∧ z) ⊕ (y ∧ z)     40 ≤ t ≤ 59
inline void SHA1::Round3(SHA1::WorkspaceBlock * block, SHA1::Word & v, SHA1::Word & w, SHA1::Word & x, SHA1::Word & y,
                         SHA1::Word & z, size_t i)
{
    z += (((w | x) & y) | (w & x)) + SHABLK(block, i) + SHA1::K2 + ROTL(v, 5);
    w = ROTL(w, 30);
}

// Parity(x, y, z)=x ⊕ y ⊕ z                    60 ≤ t ≤ 79
inline void SHA1::Round4(SHA1::WorkspaceBlock * block, SHA1::Word & v, SHA1::Word & w, SHA1::Word & x, SHA1::Word & y,
                         SHA1::Word & z, size_t i)
{
    z += (w ^ x ^ y) + SHABLK(block, i) + SHA1::K3 + ROTL(v, 5);
    w = ROTL(w, 30);
}

void SHA1::Transform(const uint8_t buffer[BlockSize])
{
    memcpy(_block, buffer, sizeof(WorkspaceBlock::c));

    // Copy state[] to working vars
    Word a = _state[0];
    Word b = _state[1];
    Word c = _state[2];
    Word d = _state[3];
    Word e = _state[4];

    // 4 rounds of 20 operations each. Loop unrolled.
    // 0 <= t <= 19
    Round0(_block, a, b, c, d, e, 0);
    Round0(_block, e, a, b, c, d, 1);
    Round0(_block, d, e, a, b, c, 2);
    Round0(_block, c, d, e, a, b, 3);
    Round0(_block, b, c, d, e, a, 4);
    Round0(_block, a, b, c, d, e, 5);
    Round0(_block, e, a, b, c, d, 6);
    Round0(_block, d, e, a, b, c, 7);
    Round0(_block, c, d, e, a, b, 8);
    Round0(_block, b, c, d, e, a, 9);
    Round0(_block, a, b, c, d, e, 10);
    Round0(_block, e, a, b, c, d, 11);
    Round0(_block, d, e, a, b, c, 12);
    Round0(_block, c, d, e, a, b, 13);
    Round0(_block, b, c, d, e, a, 14);
    Round0(_block, a, b, c, d, e, 15);
    Round1(_block, e, a, b, c, d, 16);
    Round1(_block, d, e, a, b, c, 17);
    Round1(_block, c, d, e, a, b, 18);
    Round1(_block, b, c, d, e, a, 19);

    // 20 <= t <= 39
    Round2(_block, a, b, c, d, e, 20);
    Round2(_block, e, a, b, c, d, 21);
    Round2(_block, d, e, a, b, c, 22);
    Round2(_block, c, d, e, a, b, 23);
    Round2(_block, b, c, d, e, a, 24);
    Round2(_block, a, b, c, d, e, 25);
    Round2(_block, e, a, b, c, d, 26);
    Round2(_block, d, e, a, b, c, 27);
    Round2(_block, c, d, e, a, b, 28);
    Round2(_block, b, c, d, e, a, 29);
    Round2(_block, a, b, c, d, e, 30);
    Round2(_block, e, a, b, c, d, 31);
    Round2(_block, d, e, a, b, c, 32);
    Round2(_block, c, d, e, a, b, 33);
    Round2(_block, b, c, d, e, a, 34);
    Round2(_block, a, b, c, d, e, 35);
    Round2(_block, e, a, b, c, d, 36);
    Round2(_block, d, e, a, b, c, 37);
    Round2(_block, c, d, e, a, b, 38);
    Round2(_block, b, c, d, e, a, 39);

    // 40 <= t <= 59
    Round3(_block, a, b, c, d, e, 40);
    Round3(_block, e, a, b, c, d, 41);
    Round3(_block, d, e, a, b, c, 42);
    Round3(_block, c, d, e, a, b, 43);
    Round3(_block, b, c, d, e, a, 44);
    Round3(_block, a, b, c, d, e, 45);
    Round3(_block, e, a, b, c, d, 46);
    Round3(_block, d, e, a, b, c, 47);
    Round3(_block, c, d, e, a, b, 48);
    Round3(_block, b, c, d, e, a, 49);
    Round3(_block, a, b, c, d, e, 50);
    Round3(_block, e, a, b, c, d, 51);
    Round3(_block, d, e, a, b, c, 52);
    Round3(_block, c, d, e, a, b, 53);
    Round3(_block, b, c, d, e, a, 54);
    Round3(_block, a, b, c, d, e, 55);
    Round3(_block, e, a, b, c, d, 56);
    Round3(_block, d, e, a, b, c, 57);
    Round3(_block, c, d, e, a, b, 58);
    Round3(_block, b, c, d, e, a, 59);

    // 60 <= t <= 79
    Round4(_block, a, b, c, d, e, 60);
    Round4(_block, e, a, b, c, d, 61);
    Round4(_block, d, e, a, b, c, 62);
    Round4(_block, c, d, e, a, b, 63);
    Round4(_block, b, c, d, e, a, 64);
    Round4(_block, a, b, c, d, e, 65);
    Round4(_block, e, a, b, c, d, 66);
    Round4(_block, d, e, a, b, c, 67);
    Round4(_block, c, d, e, a, b, 68);
    Round4(_block, b, c, d, e, a, 69);
    Round4(_block, a, b, c, d, e, 70);
    Round4(_block, e, a, b, c, d, 71);
    Round4(_block, d, e, a, b, c, 72);
    Round4(_block, c, d, e, a, b, 73);
    Round4(_block, b, c, d, e, a, 74);
    Round4(_block, a, b, c, d, e, 75);
    Round4(_block, e, a, b, c, d, 76);
    Round4(_block, d, e, a, b, c, 77);
    Round4(_block, c, d, e, a, b, 78);
    Round4(_block, b, c, d, e, a, 79);

    // Add the working vars back into state[]
    _state[0] += a;
    _state[1] += b;
    _state[2] += c;
    _state[3] += d;
    _state[4] += e;
}

OSAL::String SHA1::ToString() const
{
    basic_ostringstream<OSAL::Char> stream;

    for (size_t i = 0; i < sizeof(_digest); ++i)
    {
        stream << uppercase << hex << setw(2) << setfill('0') << static_cast<int>(_digest[i]);
    }
    return stream.str();
}