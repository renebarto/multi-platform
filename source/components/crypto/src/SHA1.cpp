// SHA-1 computation

// 100% free public domain implementation of the SHA-1
// algorithm by Dominik Reichl <dominik.reichl@t-online.de>
//
//
// === Test Vectors (from FIPS PUB 180-1) ===
//
// SHA1("abc") =
// A9993E36 4706816A BA3E2571 7850C26C 9CD0D89D
//
// SHA1("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq") =
// 84983E44 1C3BD26E BAAE4AA1 F95129E5 E54670F1
//
// SHA1(A million repetitions of "a") =
// 34AA973C D4C4DAA4 F61EEB2B DBAD2731 6534016F

#include "crypto/SHA1.h"

#include <sstream>

using namespace std;
using namespace Crypto;

union Crypto::SHA1::WorkspaceBlock
{
    uint8_t c[SHA1::BlockSize];
    Word l[16];
};

// Rotate x bits to the left
#define ROTL(value, bits) (((value)<<(bits))|((value)>>(WordLength-(bits))))

// Wt = Mt                                                  0 <= t <= 15
#ifdef LITTLE_ENDIAN
// Swap bytes to for BIG ENDIAN value
#define SHABLK0(i) (_block->l[i] = \
            (ROTL(_block->l[i],24) & 0xFF00FF00) | (ROTL(_block->l[i],8) & 0x00FF00FF))
#else
#define SHABLK0(i) (_block->l[i])
#endif

//                                                          16 <= t <= 79
// s = t & 0x0000000F
// W s = ROTL 1 ( W ( s + 13 ) ∧ MASK ⊕ W ( s + 8 ) ∧ MASK ⊕ W ( s + 2 ) ∧ MASK ⊕ W s )
#define SHABLK(i) (_block->l[i&15] = \
            (ROTL(_block->l[(i+13)&15] ^ _block->l[(i+8)&15] ^ _block->l[(i+2)&15] ^ _block->l[i&15],1)))

// SHA-1 rounds
const SHA1::Word SHA1::K0 = 0x5A827999;
const SHA1::Word SHA1::K1 = 0x6ED9EBA1;
const SHA1::Word SHA1::K2 = 0x8F1BBCDC;
const SHA1::Word SHA1::K3 = 0xCA62C1D6;

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
#define R0(v,w,x,y,z,i) { z+=((w&(x^y))^y)+SHABLK0(i)+K0+ROTL(v,5); w=ROTL(w,30); }
#define R1(v,w,x,y,z,i) { z+=((w&(x^y))^y)+SHABLK(i)+K0+ROTL(v,5); w=ROTL(w,30); }
// Parity(x, y, z)=x ⊕ y ⊕ z                    20 ≤ t ≤ 39
#define R2(v,w,x,y,z,i) { z+=(w^x^y)+SHABLK(i)+K1+ROTL(v,5); w=ROTL(w,30); }
// Maj(x, y, z)=(x ∧ y) ⊕ (x ∧ z) ⊕ (y ∧ z)     40 ≤ t ≤ 59
#define R3(v,w,x,y,z,i) { z+=(((w|x)&y)|(w&x))+SHABLK(i)+K2+ROTL(v,5); w=ROTL(w,30); }
// Parity(x, y, z)=x ⊕ y ⊕ z                    60 ≤ t ≤ 79
#define R4(v,w,x,y,z,i) { z+=(w^x^y)+SHABLK(i)+K3+ROTL(v,5); w=ROTL(w,30); }

SHA1::SHA1()
    : _state()
    , _bitCount()
    , _buffer()
    , _digest()
    , _workspace()
    , _block(reinterpret_cast<WorkspaceBlock *>(_workspace))
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
void SHA1::Process(const uint8_t *data, size_t len)
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

void SHA1::Process(const Core::ByteArray & data)
{
    Process(data.Data(), data.Size());
}

void SHA1::Finalize()
{
    // For _bitCount into a 64 bit number
    uint8_t finalCount[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    for (size_t i = 0; i < sizeof(finalCount); i++)
        // Endian independent
        finalCount[i] = static_cast<uint8_t>((_bitCount >> ((7 - (i & 7)) * 8) ) & 255);

    // Suppose that the length of the message, M, is l bits. Append the bit “1” to the end of the
    // message, followed by k zero bits, where k is the smallest, non-negative solution to the equation
    // l + 1 + k ≡ 448 mod 512 . Then append the 64-bit block that is equal to the number l expressed
    // using a binary representation.
    // A "1" bit followed by 7 "0" bits
    Process(reinterpret_cast<const uint8_t *>("\x80"), 1);

    // Fill up to 448 bits with "0" bits
    while ((_bitCount & 504) != 448)
        Process(reinterpret_cast<const uint8_t *>("\x00"), 1);

    // Add l = _bitCount as 64 bit integer
    Process(finalCount, sizeof(finalCount));

    for (size_t i = 0; i < DigestSize; i++)
    {
        // Convert 5 32 bit integers to 20 bytes in BIG ENDIAN form
        _digest[i] = static_cast<uint8_t>((_state[i >> 2] >> ((3 - (i & 3)) * 8) ) & 255);
    }

    memset(_buffer, 0, BlockSize);
    memset(_state, 0, sizeof(_state));
    _bitCount = 0;
    memset(finalCount, 0, sizeof(finalCount));

    Transform(_buffer);
}

Core::ByteArray SHA1::GetDigest() const
{
    Core::ByteArray digest(_digest, sizeof(_digest));
    return digest;
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
    R0(a,b,c,d,e, 0); R0(e,a,b,c,d, 1); R0(d,e,a,b,c, 2); R0(c,d,e,a,b, 3);
    R0(b,c,d,e,a, 4); R0(a,b,c,d,e, 5); R0(e,a,b,c,d, 6); R0(d,e,a,b,c, 7);
    R0(c,d,e,a,b, 8); R0(b,c,d,e,a, 9); R0(a,b,c,d,e,10); R0(e,a,b,c,d,11);
    R0(d,e,a,b,c,12); R0(c,d,e,a,b,13); R0(b,c,d,e,a,14); R0(a,b,c,d,e,15);
    R1(e,a,b,c,d,16); R1(d,e,a,b,c,17); R1(c,d,e,a,b,18); R1(b,c,d,e,a,19);

    // 20 <= t <= 39
    R2(a,b,c,d,e,20); R2(e,a,b,c,d,21); R2(d,e,a,b,c,22); R2(c,d,e,a,b,23);
    R2(b,c,d,e,a,24); R2(a,b,c,d,e,25); R2(e,a,b,c,d,26); R2(d,e,a,b,c,27);
    R2(c,d,e,a,b,28); R2(b,c,d,e,a,29); R2(a,b,c,d,e,30); R2(e,a,b,c,d,31);
    R2(d,e,a,b,c,32); R2(c,d,e,a,b,33); R2(b,c,d,e,a,34); R2(a,b,c,d,e,35);
    R2(e,a,b,c,d,36); R2(d,e,a,b,c,37); R2(c,d,e,a,b,38); R2(b,c,d,e,a,39);

    // 40 <= t <= 59
    R3(a,b,c,d,e,40); R3(e,a,b,c,d,41); R3(d,e,a,b,c,42); R3(c,d,e,a,b,43);
    R3(b,c,d,e,a,44); R3(a,b,c,d,e,45); R3(e,a,b,c,d,46); R3(d,e,a,b,c,47);
    R3(c,d,e,a,b,48); R3(b,c,d,e,a,49); R3(a,b,c,d,e,50); R3(e,a,b,c,d,51);
    R3(d,e,a,b,c,52); R3(c,d,e,a,b,53); R3(b,c,d,e,a,54); R3(a,b,c,d,e,55);
    R3(e,a,b,c,d,56); R3(d,e,a,b,c,57); R3(c,d,e,a,b,58); R3(b,c,d,e,a,59);

    // 60 <= t <= 79
    R4(a,b,c,d,e,60); R4(e,a,b,c,d,61); R4(d,e,a,b,c,62); R4(c,d,e,a,b,63);
    R4(b,c,d,e,a,64); R4(a,b,c,d,e,65); R4(e,a,b,c,d,66); R4(d,e,a,b,c,67);
    R4(c,d,e,a,b,68); R4(b,c,d,e,a,69); R4(a,b,c,d,e,70); R4(e,a,b,c,d,71);
    R4(d,e,a,b,c,72); R4(c,d,e,a,b,73); R4(b,c,d,e,a,74); R4(a,b,c,d,e,75);
    R4(e,a,b,c,d,76); R4(d,e,a,b,c,77); R4(c,d,e,a,b,78); R4(b,c,d,e,a,79);

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