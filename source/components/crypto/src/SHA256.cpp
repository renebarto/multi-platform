// SHA-224/256 computation
//

#include "crypto/SHA256.h"

#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;
using namespace Crypto;

static constexpr size_t NumWorkspace = 16;
static constexpr size_t NumWorkspaceMinusOne = NumWorkspace - 1;
static constexpr size_t NumRounds = 64;
struct Crypto::SHA256Base::WorkspaceBlock
{
    Word l[NumWorkspace];
};

static constexpr uint64_t Mod512Mask = 0x00000000000001FF;
static constexpr uint64_t Mod512_448 = 0x00000000000001C0;

const SHA256Base::Word SHA256Base::K[64] =
{
0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

SHA256Base::SHA256Base()
: _bitCount(), _buffer(), _state(), _workspace(), _block(new WorkspaceBlock)
{
}

SHA256Base::~SHA256Base()
{
    delete _block;
}

// Update the hash value
void SHA256Base::Process(const uint8_t * data, size_t len)
{
    uint32_t filledBlock = static_cast<uint32_t>((_bitCount >> 3) & BlockSizeMinusOne);
    // Calculate bytes mod 64 or bit mod 512

    _bitCount += (static_cast<uint64_t>(len) << 3);     // Add number of bits for this block
    // (we expect no more than 2^64 bits in total)

    uint32_t offset{};
    if ((filledBlock + len) > BlockSizeMinusOne)        // If total number of bytes crosses 64
        // (i.e. if we now have a complete 512 bit block)
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

void SHA256Base::Process(const OSAL::ByteArray & data)
{
    Process(data.Data(), data.Size());
}

void SHA256Base::Finalize()
{
    // Convert _bitCount into a 64 bit number
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

    memset(finalCount, 0, sizeof(finalCount));
}

void SHA256Base::DumpState()
{
    cout << "State: " << endl;
    for (size_t i = 0; i < StateSize; ++i)
    {
        cout << setw(2) << setfill('0') << i << " = " << hex << setw(8) << setfill('0') << uppercase << _state[i] << dec
             << endl;
    }
}

void SHA256Base::DumpState(size_t t, Word a, Word b, Word c, Word d, Word e, Word f, Word g, Word h)
{
    cout << "t = " << setw(2) << t << "  "
         << "A = " << hex << setw(8) << setfill('0') << uppercase << a << dec << " "
         << "B = " << hex << setw(8) << setfill('0') << uppercase << b << dec << " "
         << "C = " << hex << setw(8) << setfill('0') << uppercase << c << dec << " "
         << "D = " << hex << setw(8) << setfill('0') << uppercase << d << dec << " "
         << "E = " << hex << setw(8) << setfill('0') << uppercase << e << dec << " "
         << "F = " << hex << setw(8) << setfill('0') << uppercase << f << dec << " "
         << "G = " << hex << setw(8) << setfill('0') << uppercase << g << dec << " "
         << "H = " << hex << setw(8) << setfill('0') << uppercase << h << dec << endl;
}

void SHA256Base::DumpBlock(SHA256Base::WorkspaceBlock * block)
{
    cout << "W: " << endl;
    for (size_t i = 0; i < NumWorkspace; ++i)
    {
        cout << "W" << setw(2) << setfill('0') << i << " = " << hex << setw(8) << setfill('0') << uppercase
             << block->l[i] << dec << endl;
    }
}

inline SHA256Base::Word SHA256Base::ROTLEFT(SHA256Base::Word a, size_t b)
{
    return ((a) << (b)) | ((a) >> (SHA256Base::WordLength - (b)));
}

inline SHA256Base::Word SHA256Base::ROTRIGHT(SHA256Base::Word a, size_t b)
{
    return ((a) >> (b)) | ((a) << (SHA256Base::WordLength - (b)));
}

// Ch ( x , y , z )     = ( x ∧ y ) ⊕ ( ¬ x ∧ z )
inline SHA256Base::Word SHA256Base::CH(SHA256Base::Word x, SHA256Base::Word y, SHA256Base::Word z)
{
    return (((x) & (y)) ^ (~(x) & (z)));
}

// Maj ( x , y , z )    = ( x ∧ y ) ⊕ ( x ∧ z ) ⊕ ( y ∧ z )
inline SHA256Base::Word SHA256Base::MAJ(SHA256Base::Word x, SHA256Base::Word y, SHA256Base::Word z)
{
    return (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)));
}

//  { 256 }
// ∑    (x )            = ROTR 2 (x) ⊕ ROTR 13 (x) ⊕ ROTR 22 (x)
//  0
inline SHA256Base::Word SHA256Base::EP0(SHA256Base::Word x)
{
    return (ROTRIGHT(x, 2) ^ ROTRIGHT(x, 13) ^ ROTRIGHT(x, 22));
}

//  { 256 }
// ∑    (x )            = ROTR 6 (x) ⊕ ROTR 11 (x) ⊕ ROTR 25 (x)
//  1
inline SHA256Base::Word SHA256Base::EP1(SHA256Base::Word x)
{
    return (ROTRIGHT(x, 6) ^ ROTRIGHT(x, 11) ^ ROTRIGHT(x, 25));
}

//  { 256 }
// σ   ( x )            = ROTR 7 (x) ⊕ ROTR 18 (x) ⊕ SHR 3 (x)
//  0
inline SHA256Base::Word SHA256Base::SIG0(SHA256Base::Word x)
{
    return (ROTRIGHT(x, 7) ^ ROTRIGHT(x, 18) ^ ((x) >> 3));
}

//  { 256 }
// σ   ( x )            = ROTR 17 (x) ⊕ ROTR 19 (x) ⊕ SHR 10 (x)
//  1
inline SHA256Base::Word SHA256Base::SIG1(SHA256Base::Word x)
{
    return (ROTRIGHT(x, 17) ^ ROTRIGHT(x, 19) ^ ((x) >> 10));
}

// Wt = Mt                                                  0 <= t <= 15
#ifdef LITTLE_ENDIAN

// Swap bytes to for BIG ENDIAN value
inline SHA256Base::Word SHA256Base::SHABLK0(SHA256Base::WorkspaceBlock * block, size_t t)
{
    return ((ROTLEFT(block->l[t], 24) & 0xFF00FF00) | (ROTLEFT(block->l[t], 8) & 0x00FF00FF));
}

#else
inline SHA256Base::Word SHA256Base::SHABLK0(SHA256Base::WorkspaceBlock * block, size_t i)
{
    return block->l[i];
}
#endif

//                                                          16 <= t <= 79
// s = t & 0x0000000F
//        {256}                         {256}
// W s = σ     ( W t − 2 ) + W t − 7 + σ     ( W t − 15 ) + W t − 16
//        1                             0
inline SHA256Base::Word SHA256Base::SHABLK(SHA256Base::WorkspaceBlock * block, size_t t)
{
    return (SIG1(block->l[(t + 14) & NumWorkspaceMinusOne]) + block->l[(t + 9) & NumWorkspaceMinusOne] +
            SIG0(block->l[(t + 1) & NumWorkspaceMinusOne]) + block->l[t & NumWorkspaceMinusOne]);
}

inline void SHA256Base::Round0(SHA256Base::WorkspaceBlock * block,
                               SHA256Base::Word & a, SHA256Base::Word & b, SHA256Base::Word & c, SHA256Base::Word & d,
                               SHA256Base::Word & e, SHA256Base::Word & f, SHA256Base::Word & g, SHA256Base::Word & h,
                               size_t t)
{
    block->l[t] = SHABLK0(block, t);
//    if (t == 0)
//        DumpBlock(block);
    Word T1 = h + EP1(e) + CH(e, f, g) + K[t] + block->l[t];
    Word T2 = EP0(a) + MAJ(a, b, c);
    h = g;
    g = f;
    f = e;
    e = d + T1;
    d = c;
    c = b;
    b = a;
    a = T1 + T2;
//    DumpState(t, a, b, c, d, e, f, g, h);
}

inline void SHA256Base::Round1(SHA256Base::WorkspaceBlock * block,
                               SHA256Base::Word & a, SHA256Base::Word & b, SHA256Base::Word & c, SHA256Base::Word & d,
                               SHA256Base::Word & e, SHA256Base::Word & f, SHA256Base::Word & g, SHA256Base::Word & h,
                               size_t t)
{
    block->l[t & NumWorkspaceMinusOne] = SHABLK(block, t);
    Word T1 = h + EP1(e) + CH(e, f, g) + K[t] + block->l[t & NumWorkspaceMinusOne];
    Word T2 = EP0(a) + MAJ(a, b, c);
    h = g;
    g = f;
    f = e;
    e = d + T1;
    d = c;
    c = b;
    b = a;
    a = T1 + T2;
//    DumpState(t, a, b, c, d, e, f, g, h);
}

void SHA256Base::Transform(const uint8_t buffer[BlockSize])
{
    memcpy(_block, buffer, BlockSize);

//    DumpState();

    // Copy state[] to working vars
    Word a = _state[0];
    Word b = _state[1];
    Word c = _state[2];
    Word d = _state[3];
    Word e = _state[4];
    Word f = _state[5];
    Word g = _state[6];
    Word h = _state[7];

    for (size_t t = 0; t < NumWorkspace; ++t)
        Round0(_block, a, b, c, d, e, f, g, h, t);
    for (size_t t = NumWorkspace; t < NumRounds; ++t)
        Round1(_block, a, b, c, d, e, f, g, h, t);

    // Add the working vars back into state[]
    _state[0] += a;
    _state[1] += b;
    _state[2] += c;
    _state[3] += d;
    _state[4] += e;
    _state[5] += f;
    _state[6] += g;
    _state[7] += h;

//    DumpState();
}

SHA224::SHA224()
: _digest()
{
    Initialize();
}

void SHA224::Initialize()
{
    // SHA224 initialization constants
    _state[0] = 0xc1059ed8;
    _state[1] = 0x367cd507;
    _state[2] = 0x3070dd17;
    _state[3] = 0xf70e5939;
    _state[4] = 0xffc00b31;
    _state[5] = 0x68581511;
    _state[6] = 0x64f98fa7;
    _state[7] = 0xbefa4fa4;
    _bitCount = 0;

    memset(_digest, 0, sizeof(_digest));
}

void SHA224::Finalize()
{
    SHA256Base::Finalize();
    for (size_t i = 0; i < DigestSize; i++)
    {
        // Convert 16 32 bit integers to digest in BIG ENDIAN form
        _digest[i] = static_cast<uint8_t>((_state[i >> 2] >> ((3 - (i & 3)) * 8)) & 255);
    }

    memset(_buffer, 0, BlockSize);
    memset(_state, 0, sizeof(_state));
    _bitCount = 0;
}

OSAL::ByteArray SHA224::GetDigest() const
{
    OSAL::ByteArray digest(_digest, sizeof(_digest));
    return digest;
}

OSAL::String SHA224::ToString() const
{
    basic_ostringstream<OSAL::Char> stream;

    for (size_t i = 0; i < sizeof(_digest); ++i)
    {
        stream << uppercase << hex << setw(2) << setfill(_('0')) << static_cast<int>(_digest[i]);
    }
    return stream.str();
}

SHA256::SHA256()
: _digest()
{
    Initialize();
}

void SHA256::Initialize()
{
    // SHA256 initialization constants
    _state[0] = 0x6a09e667;
    _state[1] = 0xbb67ae85;
    _state[2] = 0x3c6ef372;
    _state[3] = 0xa54ff53a;
    _state[4] = 0x510e527f;
    _state[5] = 0x9b05688c;
    _state[6] = 0x1f83d9ab;
    _state[7] = 0x5be0cd19;

    _bitCount = 0;

    memset(_digest, 0, sizeof(_digest));
}

void SHA256::Finalize()
{
    SHA256Base::Finalize();
    for (size_t i = 0; i < DigestSize; i++)
    {
        // Convert 16 32 bit integers to digest in BIG ENDIAN form
        _digest[i] = static_cast<uint8_t>((_state[i >> 2] >> ((3 - (i & 3)) * 8)) & 255);
    }

    memset(_buffer, 0, BlockSize);
    memset(_state, 0, sizeof(_state));
    _bitCount = 0;
}

OSAL::ByteArray SHA256::GetDigest() const
{
    OSAL::ByteArray digest(_digest, sizeof(_digest));

    return digest;
}

OSAL::String SHA256::ToString() const
{
    basic_ostringstream<OSAL::Char> stream;

    for (size_t i = 0; i < sizeof(_digest); ++i)
    {
        stream << uppercase << hex << setw(2) << setfill(_('0')) << static_cast<int>(_digest[i]);
    }
    return stream.str();
}
