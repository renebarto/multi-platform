// SHA-384/512 computation
//

#include "crypto/SHA512.h"

#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;
using namespace Crypto;

static constexpr size_t NumWorkspace = 16;
static constexpr size_t NumWorkspaceMinusOne = NumWorkspace - 1;
static constexpr size_t NumRounds = 80;
struct Crypto::SHA512Base::WorkspaceBlock
{
    Word l[NumWorkspace];
};

static constexpr uint64_t Mod1024Mask = 0x00000000000003FF;
static constexpr uint64_t Mod1024_896 = 0x0000000000000380;

const SHA512Base::Word SHA512Base::K[80] =
{
UL64(0x428A2F98D728AE22), UL64(0x7137449123EF65CD),
UL64(0xB5C0FBCFEC4D3B2F), UL64(0xE9B5DBA58189DBBC),
UL64(0x3956C25BF348B538), UL64(0x59F111F1B605D019),
UL64(0x923F82A4AF194F9B), UL64(0xAB1C5ED5DA6D8118),
UL64(0xD807AA98A3030242), UL64(0x12835B0145706FBE),
UL64(0x243185BE4EE4B28C), UL64(0x550C7DC3D5FFB4E2),
UL64(0x72BE5D74F27B896F), UL64(0x80DEB1FE3B1696B1),
UL64(0x9BDC06A725C71235), UL64(0xC19BF174CF692694),
UL64(0xE49B69C19EF14AD2), UL64(0xEFBE4786384F25E3),
UL64(0x0FC19DC68B8CD5B5), UL64(0x240CA1CC77AC9C65),
UL64(0x2DE92C6F592B0275), UL64(0x4A7484AA6EA6E483),
UL64(0x5CB0A9DCBD41FBD4), UL64(0x76F988DA831153B5),
UL64(0x983E5152EE66DFAB), UL64(0xA831C66D2DB43210),
UL64(0xB00327C898FB213F), UL64(0xBF597FC7BEEF0EE4),
UL64(0xC6E00BF33DA88FC2), UL64(0xD5A79147930AA725),
UL64(0x06CA6351E003826F), UL64(0x142929670A0E6E70),
UL64(0x27B70A8546D22FFC), UL64(0x2E1B21385C26C926),
UL64(0x4D2C6DFC5AC42AED), UL64(0x53380D139D95B3DF),
UL64(0x650A73548BAF63DE), UL64(0x766A0ABB3C77B2A8),
UL64(0x81C2C92E47EDAEE6), UL64(0x92722C851482353B),
UL64(0xA2BFE8A14CF10364), UL64(0xA81A664BBC423001),
UL64(0xC24B8B70D0F89791), UL64(0xC76C51A30654BE30),
UL64(0xD192E819D6EF5218), UL64(0xD69906245565A910),
UL64(0xF40E35855771202A), UL64(0x106AA07032BBD1B8),
UL64(0x19A4C116B8D2D0C8), UL64(0x1E376C085141AB53),
UL64(0x2748774CDF8EEB99), UL64(0x34B0BCB5E19B48A8),
UL64(0x391C0CB3C5C95A63), UL64(0x4ED8AA4AE3418ACB),
UL64(0x5B9CCA4F7763E373), UL64(0x682E6FF3D6B2B8A3),
UL64(0x748F82EE5DEFB2FC), UL64(0x78A5636F43172F60),
UL64(0x84C87814A1F0AB72), UL64(0x8CC702081A6439EC),
UL64(0x90BEFFFA23631E28), UL64(0xA4506CEBDE82BDE9),
UL64(0xBEF9A3F7B2C67915), UL64(0xC67178F2E372532B),
UL64(0xCA273ECEEA26619C), UL64(0xD186B8C721C0C207),
UL64(0xEADA7DD6CDE0EB1E), UL64(0xF57D4F7FEE6ED178),
UL64(0x06F067AA72176FBA), UL64(0x0A637DC5A2C898A6),
UL64(0x113F9804BEF90DAE), UL64(0x1B710B35131C471B),
UL64(0x28DB77F523047D84), UL64(0x32CAAB7B40C72493),
UL64(0x3C9EBE0A15C9BEBC), UL64(0x431D67C49C100D4C),
UL64(0x4CC5D4BECB3E42B6), UL64(0x597F299CFC657E2A),
UL64(0x5FCB6FAB3AD6FAEC), UL64(0x6C44198C4A475817)
};

SHA512Base::SHA512Base()
: _bitCountL(), _bitCountH(), _buffer(), _state(), _workspace(), _block(reinterpret_cast<WorkspaceBlock *>(_workspace))
{
}

// Update the hash value
void SHA512Base::Process(const uint8_t * data, size_t len)
{
    uint32_t filledBlock = static_cast<uint32_t>((_bitCountL >> 3) & BlockSizeMinusOne);
    // Calculate bytes mod 64 or bit mod 512

    _bitCountH += (((_bitCountL >> 3) + static_cast<uint64_t>(len)) >> 61);
    _bitCountL += (static_cast<uint64_t>(len) << 3);
    // Add number of bits for this block
    // (we expect no more than 2^128 bits in total)

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

void SHA512Base::Process(const OSAL::ByteArray & data)
{
    Process(data.Data(), data.Size());
}

void SHA512Base::Finalize()
{
    // Convert _bitCount into a 128 bit number
    uint8_t finalCount[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    // Endian independent
    for (size_t i = 0; i < sizeof(finalCount) / 2; i++)
        finalCount[i] = static_cast<uint8_t>((_bitCountH >> ((7 - (i & 7)) * 8)) & 255);
    for (size_t i = sizeof(finalCount) / 2; i < sizeof(finalCount); i++)
        finalCount[i] = static_cast<uint8_t>((_bitCountL >> ((7 - (i & 7)) * 8)) & 255);

    // Suppose that the length of the message, M, is l bits. Append the bit “1” to the end of the
    // message, followed by k zero bits, where k is the smallest, non-negative solution to the equation
    // l + 1 + k ≡ 448 mod 512 . Then append the 64-bit block that is equal to the number l expressed
    // using a binary representation.
    // A "1" bit followed by 7 "0" bits
    Process(reinterpret_cast<const uint8_t *>("\x80"), 1);

    // Fill up to 896 bits with "0" bits
    while ((_bitCountL & Mod1024Mask) != Mod1024_896)
        Process(reinterpret_cast<const uint8_t *>("\x00"), 1);

    // Add l = _bitCount as 64 bit integer
    Process(finalCount, sizeof(finalCount));

    memset(finalCount, 0, sizeof(finalCount));
}

void SHA512Base::DumpState()
{
    cout << "State: " << endl;
    for (size_t i = 0; i < StateSize; ++i)
    {
        cout << setw(2) << setfill('0') << i << " = " << hex << setw(16) << setfill('0') << uppercase << _state[i]
             << dec << endl;
    }
}

void SHA512Base::DumpState(size_t t, Word a, Word b, Word c, Word d, Word e, Word f, Word g, Word h)
{
    cout << "t = " << setw(2) << t << "  "
         << "A = " << hex << setw(16) << setfill('0') << uppercase << a << dec << " "
         << "B = " << hex << setw(16) << setfill('0') << uppercase << b << dec << " "
         << "C = " << hex << setw(16) << setfill('0') << uppercase << c << dec << " "
         << "D = " << hex << setw(16) << setfill('0') << uppercase << d << dec << " "
         << "E = " << hex << setw(16) << setfill('0') << uppercase << e << dec << " "
         << "F = " << hex << setw(16) << setfill('0') << uppercase << f << dec << " "
         << "G = " << hex << setw(16) << setfill('0') << uppercase << g << dec << " "
         << "H = " << hex << setw(16) << setfill('0') << uppercase << h << dec << endl;
}

void SHA512Base::DumpBlock(SHA512Base::WorkspaceBlock * block)
{
    cout << "W: " << endl;
    for (size_t i = 0; i < NumWorkspace; ++i)
    {
        cout << "W" << setw(2) << setfill('0') << i << " = " << hex << setw(16) << setfill('0') << uppercase
             << block->l[i] << dec << endl;
    }
}

inline SHA512Base::Word SHA512Base::ROTLEFT(SHA512Base::Word a, size_t b)
{
    return ((a) << (b)) | ((a) >> (SHA512Base::WordLength - (b)));
}

inline SHA512Base::Word SHA512Base::ROTRIGHT(SHA512Base::Word a, size_t b)
{
    return ((a) >> (b)) | ((a) << (SHA512Base::WordLength - (b)));
}

// Ch ( x , y , z )     = ( x ∧ y ) ⊕ ( ¬ x ∧ z )
inline SHA512Base::Word SHA512Base::CH(SHA512Base::Word x, SHA512Base::Word y, SHA512Base::Word z)
{
    return (((x) & (y)) ^ (~(x) & (z)));
}

// Maj ( x , y , z )    = ( x ∧ y ) ⊕ ( x ∧ z ) ⊕ ( y ∧ z )
inline SHA512Base::Word SHA512Base::MAJ(SHA512Base::Word x, SHA512Base::Word y, SHA512Base::Word z)
{
    return (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)));
}

//  { 512 }
// ∑    (x )            = ROTR 28 (x) ⊕ ROTR 34 (x) ⊕ ROTR 39 (x)
//  0
inline SHA512Base::Word SHA512Base::EP0(SHA512Base::Word x)
{
    return (ROTRIGHT(x, 28) ^ ROTRIGHT(x, 34) ^ ROTRIGHT(x, 39));
}

//  { 512 }
// ∑    (x )            = ROTR 14 (x) ⊕ ROTR 18 (x) ⊕ ROTR 41 (x)
//  1
inline SHA512Base::Word SHA512Base::EP1(SHA512Base::Word x)
{
    return (ROTRIGHT(x, 14) ^ ROTRIGHT(x, 18) ^ ROTRIGHT(x, 41));
}

//  { 512 }
// σ   ( x )            = ROTR 1 (x) ⊕ ROTR 8 (x) ⊕ SHR 7 (x)
//  0
inline SHA512Base::Word SHA512Base::SIG0(SHA512Base::Word x)
{
    return (ROTRIGHT(x, 1) ^ ROTRIGHT(x, 8) ^ ((x) >> 7));
}

//  { 512 }
// σ   ( x )            = ROTR 19 (x) ⊕ ROTR 61 (x) ⊕ SHR 6 (x)
//  1
inline SHA512Base::Word SHA512Base::SIG1(SHA512Base::Word x)
{
    return (ROTRIGHT(x, 19) ^ ROTRIGHT(x, 61) ^ ((x) >> 6));
}

// Wt = Mt                                                  0 <= t <= 15
#ifdef LITTLE_ENDIAN

// Swap bytes to for BIG ENDIAN value
inline SHA512Base::Word SHA512Base::SHABLK0(SHA512Base::WorkspaceBlock * block, size_t t)
{
    return ((ROTLEFT(block->l[t], 8) & 0x000000FF000000FF) |
            (ROTLEFT(block->l[t], 24) & 0x0000FF000000FF00) |
            (ROTLEFT(block->l[t], 40) & 0x00FF000000FF0000) |
            (ROTLEFT(block->l[t], 56) & 0xFF000000FF000000));
}

#else
inline SHA512Base::Word SHA512Base::SHABLK0(SHA512Base::WorkspaceBlock * block, size_t i)
{
    return block->l[i];
}
#endif

//                                                          16 <= t <= 79
// s = t & 0x0000000F
//        {512}                         {512}
// W s = σ     ( W t − 2 ) + W t − 7 + σ     ( W t − 15 ) + W t − 16
//        1                             0
inline SHA512Base::Word SHA512Base::SHABLK(SHA512Base::WorkspaceBlock * block, size_t t)
{
    return (SIG1(block->l[(t + 14) & NumWorkspaceMinusOne]) + block->l[(t + 9) & NumWorkspaceMinusOne] +
            SIG0(block->l[(t + 1) & NumWorkspaceMinusOne]) + block->l[t & NumWorkspaceMinusOne]);
}

inline void SHA512Base::Round0(SHA512Base::WorkspaceBlock * block,
                               SHA512Base::Word & a, SHA512Base::Word & b, SHA512Base::Word & c, SHA512Base::Word & d,
                               SHA512Base::Word & e, SHA512Base::Word & f, SHA512Base::Word & g, SHA512Base::Word & h,
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

inline void SHA512Base::Round1(SHA512Base::WorkspaceBlock * block,
                               SHA512Base::Word & a, SHA512Base::Word & b, SHA512Base::Word & c, SHA512Base::Word & d,
                               SHA512Base::Word & e, SHA512Base::Word & f, SHA512Base::Word & g, SHA512Base::Word & h,
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

void SHA512Base::Transform(const uint8_t buffer[BlockSize])
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

SHA384::SHA384()
: _digest()
{
    Initialize();
}

void SHA384::Initialize()
{
    // SHA384 initialization constants
    _state[0] = UL64(0xCBBB9D5DC1059ED8);
    _state[1] = UL64(0x629A292A367CD507);
    _state[2] = UL64(0x9159015A3070DD17);
    _state[3] = UL64(0x152FECD8F70E5939);
    _state[4] = UL64(0x67332667FFC00B31);
    _state[5] = UL64(0x8EB44A8768581511);
    _state[6] = UL64(0xDB0C2E0D64F98FA7);
    _state[7] = UL64(0x47B5481DBEFA4FA4);

    _bitCountL = _bitCountH = 0;

    memset(_digest, 0, sizeof(_digest));
}

void SHA384::Finalize()
{
    SHA512Base::Finalize();
    for (size_t i = 0; i < DigestSize; i++)
    {
        // Convert 16 64 bit integers to digest in BIG ENDIAN form
        _digest[i] = static_cast<uint8_t>((_state[i >> 3] >> ((7 - (i & 7)) * 8)) & 255);
    }

    memset(_buffer, 0, BlockSize);
    memset(_state, 0, sizeof(_state));
    _bitCountL = _bitCountH = 0;
}

OSAL::ByteArray SHA384::GetDigest() const
{
    OSAL::ByteArray digest(_digest, sizeof(_digest));
    return digest;
}

OSAL::String SHA384::ToString() const
{
    basic_ostringstream<OSAL::Char> stream;

    for (size_t i = 0; i < sizeof(_digest); ++i)
    {
        stream << uppercase << hex << setw(2) << setfill('0') << static_cast<int>(_digest[i]);
    }
    return stream.str();
}

SHA512::SHA512()
: _digest()
{
    Initialize();
}

void SHA512::Initialize()
{
    // SHA512 initialization constants
    _state[0] = UL64(0x6A09E667F3BCC908);
    _state[1] = UL64(0xBB67AE8584CAA73B);
    _state[2] = UL64(0x3C6EF372FE94F82B);
    _state[3] = UL64(0xA54FF53A5F1D36F1);
    _state[4] = UL64(0x510E527FADE682D1);
    _state[5] = UL64(0x9B05688C2B3E6C1F);
    _state[6] = UL64(0x1F83D9ABFB41BD6B);
    _state[7] = UL64(0x5BE0CD19137E2179);

    _bitCountL = _bitCountH = 0;

    memset(_digest, 0, sizeof(_digest));
}

void SHA512::Finalize()
{
    SHA512Base::Finalize();
    for (size_t i = 0; i < DigestSize; i++)
    {
        // Convert 16 64 bit integers to digest in BIG ENDIAN form
        _digest[i] = static_cast<uint8_t>((_state[i >> 3] >> ((7 - (i & 7)) * 8)) & 255);
    }

    memset(_buffer, 0, BlockSize);
    memset(_state, 0, sizeof(_state));
    _bitCountL = _bitCountH = 0;
}

OSAL::ByteArray SHA512::GetDigest() const
{
    OSAL::ByteArray digest(_digest, sizeof(_digest));
    return digest;
}

OSAL::String SHA512::ToString() const
{
    basic_ostringstream<OSAL::Char> stream;

    for (size_t i = 0; i < sizeof(_digest); ++i)
    {
        stream << uppercase << hex << setw(2) << setfill('0') << static_cast<int>(_digest[i]);
    }
    return stream.str();
}

SHA512_224::SHA512_224()
: _digest()
{
    Initialize();
}

void SHA512_224::Initialize()
{
    // SHA512_224 initialization constants
    _state[0] = 0x8C3D37C819544DA2;
    _state[1] = 0x73E1996689DCD4D6;
    _state[2] = 0x1DFAB7AE32FF9C82;
    _state[3] = 0x679DD514582F9FCF;
    _state[4] = 0x0F6D2B697BD44DA8;
    _state[5] = 0x77E36F7304C48942;
    _state[6] = 0x3F9D85A86A1D36C8;
    _state[7] = 0x1112E6AD91D692A1;

    _bitCountL = _bitCountH = 0;

    memset(_digest, 0, sizeof(_digest));
}

void SHA512_224::Finalize()
{
    SHA512Base::Finalize();
    for (size_t i = 0; i < DigestSize; i++)
    {
        // Convert 16 64 bit integers to digest in BIG ENDIAN form
        _digest[i] = static_cast<uint8_t>((_state[i >> 3] >> ((7 - (i & 7)) * 8)) & 255);
    }

    memset(_buffer, 0, BlockSize);
    memset(_state, 0, sizeof(_state));
    _bitCountL = _bitCountH = 0;
}

OSAL::ByteArray SHA512_224::GetDigest() const
{
    OSAL::ByteArray digest(_digest, sizeof(_digest));
    return digest;
}

OSAL::String SHA512_224::ToString() const
{
    basic_ostringstream<OSAL::Char> stream;

    for (size_t i = 0; i < sizeof(_digest); ++i)
    {
        stream << uppercase << hex << setw(2) << setfill('0') << static_cast<int>(_digest[i]);
    }
    return stream.str();
}

SHA512_256::SHA512_256()
: _digest()
{
    Initialize();
}

void SHA512_256::Initialize()
{
    // SHA512_256 initialization constants
    _state[0] = 0x22312194FC2BF72C;
    _state[1] = 0x9F555FA3C84C64C2;
    _state[2] = 0x2393B86B6F53B151;
    _state[3] = 0x963877195940EABD;
    _state[4] = 0x96283EE2A88EFFE3;
    _state[5] = 0xBE5E1E2553863992;
    _state[6] = 0x2B0199FC2C85B8AA;
    _state[7] = 0x0EB72DDC81C52CA2;

    _bitCountL = _bitCountH = 0;

    memset(_digest, 0, sizeof(_digest));
}

void SHA512_256::Finalize()
{
    SHA512Base::Finalize();
    for (size_t i = 0; i < DigestSize; i++)
    {
        // Convert 16 64 bit integers to digest in BIG ENDIAN form
        _digest[i] = static_cast<uint8_t>((_state[i >> 3] >> ((7 - (i & 7)) * 8)) & 255);
    }

    memset(_buffer, 0, BlockSize);
    memset(_state, 0, sizeof(_state));
    _bitCountL = _bitCountH = 0;
}

OSAL::ByteArray SHA512_256::GetDigest() const
{
    OSAL::ByteArray digest(_digest, sizeof(_digest));
    return digest;
}

OSAL::String SHA512_256::ToString() const
{
    basic_ostringstream<OSAL::Char> stream;

    for (size_t i = 0; i < sizeof(_digest); ++i)
    {
        stream << uppercase << hex << setw(2) << setfill('0') << static_cast<int>(_digest[i]);
    }
    return stream.str();
}
