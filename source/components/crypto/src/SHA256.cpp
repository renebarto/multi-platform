// SHA-1 computation

// 100% free public domain implementation of the SHA-1
// algorithm by Dominik Reichl <dominik.reichl@t-online.de>
//
//
// === Test Vectors (from FIPS PUB 180-1) ===
//
// SHA256("abc") =
// A9993E36 4706816A BA3E2571 7850C26C 9CD0D89D 00000000 00000000 00000000
//
// SHA256("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq") =
// 84983E44 1C3BD26E BAAE4AA1 F95129E5 E54670F1 00000000 00000000 00000000
//
// SHA256(A million repetitions of "a") =
// 34AA973C D4C4DAA4 F61EEB2B DBAD2731 6534016F 00000000 00000000 00000000

#include "crypto/SHA256.h"

#include <sstream>

using namespace std;
using namespace Crypto;

union Crypto::SHA256::WorkspaceBlock
{
    uint8_t c[SHA256::BlockSize];
    uint32_t l[16];
};

// Rotate x bits to the left
#define ROTL(value, bits) (((value)<<(bits))|((value)>>(SHA256::WordLength-(bits))))
#define ROTR(value, bits) (((value)>>(bits))|((value)<<(SHA256::WordLength-(bits))))
#define SHR(value, bits) ((value)>>(bits))

#ifdef LITTLE_ENDIAN
// Swap bytes to for BIG ENDIAN value
#define SHABLK0(i) (_block->l[i] = \
            (ROTL(_block->l[i],24) & 0xFF00FF00) | (ROTL(_block->l[i],8) & 0x00FF00FF))
#else
#define SHABLK0(i) (_block->l[i])
#endif

#define SHABLK(i) (_block->l[i&15] = \
            (ROTL(_block->l[(i+13)&15] ^ _block->l[(i+8)&15] ^ _block->l[(i+2)&15] ^ _block->l[i&15],1)))

// SHA-1 rounds
const SHA256::Word SHA256::K[64] =
{
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

static inline SHA256::Word F1(SHA256::Word x, SHA256::Word y, SHA256::Word z)
{
    return (x&(y^z))^z;
}
static inline SHA256::Word F2(SHA256::Word x, SHA256::Word y, SHA256::Word z)
{
    return ((x|y)&z)|(x&y);
}
static inline SHA256::Word F3(SHA256::Word x, SHA256::Word y, SHA256::Word z)
{
    return ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22);
}
static inline SHA256::Word F4(SHA256::Word x, SHA256::Word y, SHA256::Word z)
{
    return ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25);
}
static inline SHA256::Word F5(SHA256::Word x, SHA256::Word y, SHA256::Word z)
{
    return ROTR(x, 7) ^ ROTR(x, 18) ^ SHR(x, 3);
}
static inline SHA256::Word F6(SHA256::Word x, SHA256::Word y, SHA256::Word z)
{
    return ROTR(x, 17) ^ ROTR(x, 19) ^ SHR(x, 10);
}

SHA256::Word K0 = 0;
SHA256::Word K1 = 0;
SHA256::Word K2 = 0;
SHA256::Word K3 = 0;

#define R0(v,w,x,y,z,i) { z+=((w&(x^y))^y)+SHABLK0(i)+K0+ROTL(v,5); w=ROTL(w,30); }
#define R1(v,w,x,y,z,i) { z+=((w&(x^y))^y)+SHABLK(i)+K0+ROTL(v,5); w=ROTL(w,30); }
#define R2(v,w,x,y,z,i) { z+=(w^x^y)+SHABLK(i)+K1+ROTL(v,5); w=ROTL(w,30); }
#define R3(v,w,x,y,z,i) { z+=(((w|x)&y)|(w&x))+SHABLK(i)+K2+ROTL(v,5); w=ROTL(w,30); }
#define R4(v,w,x,y,z,i) { z+=(w^x^y)+SHABLK(i)+K3+ROTL(v,5); w=ROTL(w,30); }

SHA256::SHA256()
    : _state()
    , _bitCount()
    , _buffer()
    , _digest()
    , _workspace()
    , _block(reinterpret_cast<WorkspaceBlock *>(_workspace))
{
    Initialize();
}

void SHA256::Initialize()
{
    // SHA256 initialization constants
    _state[0] = 0x67452301;
    _state[1] = 0xEFCDAB89;
    _state[2] = 0x98BADCFE;
    _state[3] = 0x10325476;
    _state[4] = 0xC3D2E1F0;

    _bitCount = 0;

    memset(_digest, 0, sizeof(_digest));
}

// Update the hash value
void SHA256::Process(const uint8_t *data, size_t len)
{
    uint32_t filledBlock = static_cast<uint32_t>((_bitCount >> 3) & BlockSizeMinusOne);
                                                        // Calculate bytes mod 64 or bit mod 512

    _bitCount += (static_cast<uint64_t>(len) << 3);     // Add number of bits for this block
    // (we expect no more than 2^64 bits in total)

    uint32_t offset {};
    if ((filledBlock + len) > BlockSizeMinusOne)        // If total number of bytes crosses 64
                                                        // (i.e. if we now have a complete 512 bit block)
    {
        uint32_t spaceLeft = static_cast<uint32_t>(BlockSize - filledBlock);
                                                        // Space left in block
        memcpy(&_buffer[filledBlock], data, spaceLeft);
        // Fill up block;
        Transform(_state, _buffer);
        // Calculate
        offset += spaceLeft;

        while (offset + BlockSizeMinusOne < len)
        {
            Transform(_state, &data[offset]);
            offset += BlockSize;
        }

        filledBlock = 0;
    }

    memcpy(&_buffer[filledBlock], &data[offset], len - offset);
}

void SHA256::Process(const Core::ByteArray & data)
{
    Process(data.Data(), data.Size());
}

void SHA256::Finalize()
{
    uint32_t i = 0;
    uint8_t finalCount[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    for (i = 0; i < sizeof(finalCount); i++)
        // Endian independent
        finalCount[i] = static_cast<uint8_t>((_bitCount >> ((7 - (i & 7)) * 8) ) & 255);

    Process(reinterpret_cast<const uint8_t *>("\x80"), 1);

    while ((_bitCount & 504) != 448)
        Process(reinterpret_cast<const uint8_t *>("\x00"), 1);

    Process(finalCount, 8); // Cause a SHA256Transform()

    for (i = 0; i < 20; i++)
    {
        // Convert 5 32 bit integers to 20 bytes in BIG ENDIAN form
        _digest[i] = static_cast<uint8_t>((_state[i >> 2] >> ((3 - (i & 3)) * 8) ) & 255);
    }

    memset(_buffer, 0, BlockSize);
    memset(_state, 0, 20);
    _bitCount = 0;
    memset(finalCount, 0, 8);

    Transform(_state, _buffer);
}

Core::ByteArray SHA256::GetDigest() const
{
    Core::ByteArray digest(_digest, sizeof(_digest));
    return digest;
}

void SHA256::Transform(uint32_t state[5], const uint8_t buffer[BlockSize])
{
    memcpy(_block, buffer, sizeof(WorkspaceBlock::c));

    // Copy state[] to working vars
    uint32_t a = state[0];
    uint32_t b = state[1];
    uint32_t c = state[2];
    uint32_t d = state[3];
    uint32_t e = state[4];

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

OSAL::String SHA256::ToString() const
{
    basic_ostringstream<OSAL::Char> stream;

    for (size_t i = 0; i < sizeof(_digest); ++i)
    {
        stream << uppercase << hex << setw(2) << setfill('0') << static_cast<int>(_digest[i]);
    }
    return stream.str();
}