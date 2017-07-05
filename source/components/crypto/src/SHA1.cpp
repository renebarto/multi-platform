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

union Crypto::SHA1WorkspaceBlock
{
    uint8_t c[64];
    uint32_t l[16];
};

// Rotate x bits to the left
#define ROL32(value, bits) (((value)<<(bits))|((value)>>(32-(bits))))

#ifdef LITTLE_ENDIAN
#define SHABLK0(i) (_block->l[i] = \
            (ROL32(_block->l[i],24) & 0xFF00FF00) | (ROL32(_block->l[i],8) & 0x00FF00FF))
#else
#define SHABLK0(i) (_block->l[i])
#endif

#define SHABLK(i) (_block->l[i&15] = ROL32(_block->l[(i+13)&15] ^ _block->l[(i+8)&15] \
        ^ _block->l[(i+2)&15] ^ _block->l[i&15],1))

// SHA-1 rounds
#define R0(v,w,x,y,z,i) { z+=((w&(x^y))^y)+SHABLK0(i)+0x5A827999+ROL32(v,5); w=ROL32(w,30); }
#define R1(v,w,x,y,z,i) { z+=((w&(x^y))^y)+SHABLK(i)+0x5A827999+ROL32(v,5); w=ROL32(w,30); }
#define R2(v,w,x,y,z,i) { z+=(w^x^y)+SHABLK(i)+0x6ED9EBA1+ROL32(v,5); w=ROL32(w,30); }
#define R3(v,w,x,y,z,i) { z+=(((w|x)&y)|(w&x))+SHABLK(i)+0x8F1BBCDC+ROL32(v,5); w=ROL32(w,30); }
#define R4(v,w,x,y,z,i) { z+=(w^x^y)+SHABLK(i)+0xCA62C1D6+ROL32(v,5); w=ROL32(w,30); }

SHA1::SHA1()
    : _state()
    , _count()
    , _buffer()
    , _digest()
    , _workspace()
    , _block(reinterpret_cast<SHA1WorkspaceBlock *>(_workspace))
{
    Reset();
}

void SHA1::Reset()
{
    // SHA1 initialization constants
    _state[0] = 0x67452301;
    _state[1] = 0xEFCDAB89;
    _state[2] = 0x98BADCFE;
    _state[3] = 0x10325476;
    _state[4] = 0xC3D2E1F0;

    _count[0] = 0;
    _count[1] = 0;

    memset(_digest, 0, sizeof(_digest));
}

// Update the hash value
void SHA1::Process(const uint8_t *data, size_t len)
{
    uint32_t i = 0, j = 0;

    j = (_count[0] >> 3) & 63;

    _count[0] += (len << 3);
    if (_count[0] < (len << 3))
        _count[1]++;

    _count[1] += (len >> 29);

    if ((j + len) > 63)
    {
        memcpy(&_buffer[j], data, (i = 64 - j));
        Transform(_state, _buffer);

        for (; i+63 < len; i += 64)
        {
            Transform(_state, &data[i]);
        }

        j = 0;
    }
    else i = 0;

    memcpy(&_buffer[j], &data[i], len - i);
}

void SHA1::Finalize()
{
    uint32_t i = 0;
    uint8_t finalCount[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    for (i = 0; i < sizeof(finalCount); i++)
        // Endian independent
        finalCount[i] = static_cast<uint8_t>((_count[(i >= 4 ? 0 : 1)] >> ((3 - (i & 3)) * 8) ) & 255);

    Process(reinterpret_cast<const uint8_t *>("\200"), 1);

    while ((_count[0] & 504) != 448)
        Process(reinterpret_cast<const uint8_t *>("\0"), 1);

    Process(finalCount, 8); // Cause a SHA1Transform()

    for (i = 0; i < 20; i++)
    {
        _digest[i] = static_cast<uint8_t>((_state[i >> 2] >> ((3 - (i & 3)) * 8) ) & 255);
    }

    memset(_buffer, 0, 64);
    memset(_state, 0, 20);
    memset(_count, 0, 8);
    memset(finalCount, 0, 8);

    Transform(_state, _buffer);
}

Core::ByteArray SHA1::GetDigest()
{
    Core::ByteArray digest(_digest, sizeof(_digest));
    return digest;
}

void SHA1::Transform(uint32_t state[5], const uint8_t buffer[64])
{
    uint32_t a = 0, b = 0, c = 0, d = 0, e = 0;

    memcpy(_block, buffer, 64);

    // Copy state[] to working vars
    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];

    // 4 rounds of 20 operations each. Loop unrolled.
    R0(a,b,c,d,e, 0); R0(e,a,b,c,d, 1); R0(d,e,a,b,c, 2); R0(c,d,e,a,b, 3);
    R0(b,c,d,e,a, 4); R0(a,b,c,d,e, 5); R0(e,a,b,c,d, 6); R0(d,e,a,b,c, 7);
    R0(c,d,e,a,b, 8); R0(b,c,d,e,a, 9); R0(a,b,c,d,e,10); R0(e,a,b,c,d,11);
    R0(d,e,a,b,c,12); R0(c,d,e,a,b,13); R0(b,c,d,e,a,14); R0(a,b,c,d,e,15);
    R1(e,a,b,c,d,16); R1(d,e,a,b,c,17); R1(c,d,e,a,b,18); R1(b,c,d,e,a,19);
    R2(a,b,c,d,e,20); R2(e,a,b,c,d,21); R2(d,e,a,b,c,22); R2(c,d,e,a,b,23);
    R2(b,c,d,e,a,24); R2(a,b,c,d,e,25); R2(e,a,b,c,d,26); R2(d,e,a,b,c,27);
    R2(c,d,e,a,b,28); R2(b,c,d,e,a,29); R2(a,b,c,d,e,30); R2(e,a,b,c,d,31);
    R2(d,e,a,b,c,32); R2(c,d,e,a,b,33); R2(b,c,d,e,a,34); R2(a,b,c,d,e,35);
    R2(e,a,b,c,d,36); R2(d,e,a,b,c,37); R2(c,d,e,a,b,38); R2(b,c,d,e,a,39);
    R3(a,b,c,d,e,40); R3(e,a,b,c,d,41); R3(d,e,a,b,c,42); R3(c,d,e,a,b,43);
    R3(b,c,d,e,a,44); R3(a,b,c,d,e,45); R3(e,a,b,c,d,46); R3(d,e,a,b,c,47);
    R3(c,d,e,a,b,48); R3(b,c,d,e,a,49); R3(a,b,c,d,e,50); R3(e,a,b,c,d,51);
    R3(d,e,a,b,c,52); R3(c,d,e,a,b,53); R3(b,c,d,e,a,54); R3(a,b,c,d,e,55);
    R3(e,a,b,c,d,56); R3(d,e,a,b,c,57); R3(c,d,e,a,b,58); R3(b,c,d,e,a,59);
    R4(a,b,c,d,e,60); R4(e,a,b,c,d,61); R4(d,e,a,b,c,62); R4(c,d,e,a,b,63);
    R4(b,c,d,e,a,64); R4(a,b,c,d,e,65); R4(e,a,b,c,d,66); R4(d,e,a,b,c,67);
    R4(c,d,e,a,b,68); R4(b,c,d,e,a,69); R4(a,b,c,d,e,70); R4(e,a,b,c,d,71);
    R4(d,e,a,b,c,72); R4(c,d,e,a,b,73); R4(b,c,d,e,a,74); R4(a,b,c,d,e,75);
    R4(e,a,b,c,d,76); R4(d,e,a,b,c,77); R4(c,d,e,a,b,78); R4(b,c,d,e,a,79);

    // Add the working vars back into state[]
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
}

OSAL::String SHA1::ToString()
{
    basic_ostringstream<OSAL::Char> stream;

    for (size_t i = 0; i < sizeof(_digest); ++i)
    {
        stream << uppercase << hex << setw(2) << setfill('0') << static_cast<int>(_digest[i]);
    }
    return stream.str();
}