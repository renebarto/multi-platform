#include "crypto/DES.h"

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include <sstream>

using namespace std;
using namespace Crypto;

// The algorithm is designed to encipher and decipher blocks of data consisting of 64 bits under control
// of a 64-bit key. Deciphering must be accomplished by using the same key as for enciphering, but
// with the schedule of addressing the key bits altered so that the deciphering process is the reverse of
// the enciphering process.

inline uint32_t PermuteBit(const uint8_t * data, int bitIndexIn, int bitIndexOut)
{
    return static_cast<uint32_t>(((data[bitIndexIn / 8] >> (7 - (bitIndexIn % 8))) & 0x01) << bitIndexOut);
}

inline uint8_t ShiftBitR(uint32_t data, int bitIndexIn, int bitIndexOut)
{
    return static_cast<uint8_t>(((data >> (31 - bitIndexIn)) & 0x00000001) << bitIndexOut);
}

inline uint32_t ShiftBitL(uint32_t data, int bitIndexIn, int bitIndexOut)
{
    return static_cast<uint32_t>(((data << bitIndexIn) & 0x80000000) >> bitIndexOut);
}

inline uint32_t RotateLeft28(uint32_t value, int numBits)
{
    return ((value << numBits) | (value >> (28-numBits))) & 0xfffffff0;
}

DES::DES()
    : _keyset()
{

}

void DES::Initialize(const DESKey & key, Direction direction)
{
    uint32_t C {};
    uint32_t D {};
    size_t keyIndex;
    const uint32_t key_rnd_shift[NumSubKeys] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};
    const uint32_t key_perm_c[28] = {56,48,40,32,24,16, 8, 0,57,49,41,33,25,17,
                                      9, 1,58,50,42,34,26,18,10, 2,59,51,43,35};
    const uint32_t key_perm_d[28] = {62,54,46,38,30,22,14, 6,61,53,45,37,29,21,
                                     13, 5,60,52,44,36,28,20,12, 4,27,19,11, 3};
    const uint32_t key_compression[48] = {13,16,10,23, 0, 4, 2,27,14, 5,20, 9,
                                          22,18,11, 3,25, 7,15, 6,26,19,12, 1,
                                          40,51,30,36,46,54,29,39,50,44,32,47,
                                          43,48,38,55,33,52,45,41,49,35,28,31};

    // Permuted Choice #1 (copy the key in, ignoring parity bits).
    // Create C and D key in bits 31..4
    for (int i = 0, j = 31; i < 28; ++i, --j)
        C |= PermuteBit(key,key_perm_c[i],j);
    for (int i = 0, j = 31; i < 28; ++i, --j)
        D |= PermuteBit(key,key_perm_d[i],j);

    // Generate the 16 subkeys.
    for (size_t i = 0; i < NumSubKeys; ++i)
    {
        C = RotateLeft28(C, key_rnd_shift[i]);
        D = RotateLeft28(D, key_rnd_shift[i]);

        // Decryption subkeys are reverse order of encryption subkeys so
        // generate them in reverse if the key schedule is for decryption useage.
        if (direction == Direction::Decrypt)
            keyIndex = 15 - i;
        else
            keyIndex = i;
        // Initialize the array
        // Permuted Choice #2
        for (size_t j = 0; j < SubKeySize; ++j)
            _keyset[keyIndex][j] = 0;
        for (int j = 0; j < 24; ++j)
            _keyset[keyIndex][j/8] |= ShiftBitR(C, key_compression[j], 7 - (j % 8));
        for (int j = 24; j < 48; ++j)
            _keyset[keyIndex][j/8] |= ShiftBitR(D, key_compression[j] - 28, 7 - (j % 8));
    }
}

void DES::Process(const uint8_t * dataIn, uint8_t * dataOut, size_t len)
{
    size_t offset = 0;
    DataBlock blockIn {};
    DataBlock blockOut {};
    while (offset < len)
    {
        size_t numBytes = (len - offset >= BlockSize) ? BlockSize : len - offset;
        memcpy(blockIn, dataIn + offset, numBytes);
        if (numBytes < BlockSize)
            memset(blockIn + numBytes, 0, BlockSize - numBytes);
        ProcessBlock(blockIn, blockOut);
        memcpy(dataOut + offset, blockOut, numBytes);
        offset += numBytes;
    }
}

void DES::Process(const OSAL::ByteArray & dataIn, OSAL::ByteArray & dataOut)
{
    dataOut.Size(dataIn.Size());
    Process(dataIn.Data(), dataOut.Data(), dataIn.Size());
}

void DES::Finalize()
{

}

OSAL::String DES::ToString() const
{
    basic_ostringstream<OSAL::Char> stream;
    stream << hex << uppercase;
    for (size_t i = 0; i < NumSubKeys; ++i)
    {
        for (size_t j = 0; j < SubKeySize; ++j)
        {
            stream << setw(2) << setfill('0') << int(_keyset[i][j]);
        }
        stream << endl;
    }
    stream << dec;
    return stream.str();
}

void DES::IP(uint32_t state[], const uint8_t dataIn[])
{
    // IP
    // 58, 50, 42, 34, 26, 18, 10, 2,
    // 60, 52, 44, 36, 28, 20, 12, 4,
    // 62, 54, 46, 38, 30, 22, 14, 6,
    // 64, 56, 48, 40, 32, 24, 16, 8,
    // 57, 49, 41, 33, 25, 17,  9, 1,
    // 59, 51, 43, 35, 27, 19, 11, 3,
    // 61, 53, 45, 37, 29, 21, 13, 5,
    // 63, 55, 47, 39, 31, 23, 15, 7

    state[0] = PermuteBit(dataIn,57,31) | PermuteBit(dataIn,49,30) | PermuteBit(dataIn,41,29) | PermuteBit(dataIn,33,28) |
               PermuteBit(dataIn,25,27) | PermuteBit(dataIn,17,26) | PermuteBit(dataIn, 9,25) | PermuteBit(dataIn, 1,24) |
               PermuteBit(dataIn,59,23) | PermuteBit(dataIn,51,22) | PermuteBit(dataIn,43,21) | PermuteBit(dataIn,35,20) |
               PermuteBit(dataIn,27,19) | PermuteBit(dataIn,19,18) | PermuteBit(dataIn,11,17) | PermuteBit(dataIn, 3,16) |
               PermuteBit(dataIn,61,15) | PermuteBit(dataIn,53,14) | PermuteBit(dataIn,45,13) | PermuteBit(dataIn,37,12) |
               PermuteBit(dataIn,29,11) | PermuteBit(dataIn,21,10) | PermuteBit(dataIn,13, 9) | PermuteBit(dataIn, 5, 8) |
               PermuteBit(dataIn,63, 7) | PermuteBit(dataIn,55, 6) | PermuteBit(dataIn,47, 5) | PermuteBit(dataIn,39, 4) |
               PermuteBit(dataIn,31, 3) | PermuteBit(dataIn,23, 2) | PermuteBit(dataIn,15, 1) | PermuteBit(dataIn, 7, 0);

    state[1] = PermuteBit(dataIn,56,31) | PermuteBit(dataIn,48,30) | PermuteBit(dataIn,40,29) | PermuteBit(dataIn,32,28) |
               PermuteBit(dataIn,24,27) | PermuteBit(dataIn,16,26) | PermuteBit(dataIn, 8,25) | PermuteBit(dataIn, 0,24) |
               PermuteBit(dataIn,58,23) | PermuteBit(dataIn,50,22) | PermuteBit(dataIn,42,21) | PermuteBit(dataIn,34,20) |
               PermuteBit(dataIn,26,19) | PermuteBit(dataIn,18,18) | PermuteBit(dataIn,10,17) | PermuteBit(dataIn, 2,16) |
               PermuteBit(dataIn,60,15) | PermuteBit(dataIn,52,14) | PermuteBit(dataIn,44,13) | PermuteBit(dataIn,36,12) |
               PermuteBit(dataIn,28,11) | PermuteBit(dataIn,20,10) | PermuteBit(dataIn,12, 9) | PermuteBit(dataIn, 4, 8) |
               PermuteBit(dataIn,62, 7) | PermuteBit(dataIn,54, 6) | PermuteBit(dataIn,46, 5) | PermuteBit(dataIn,38, 4) |
               PermuteBit(dataIn,30, 3) | PermuteBit(dataIn,22, 2) | PermuteBit(dataIn,14, 1) | PermuteBit(dataIn, 6, 0);
}

void DES::IPInv(const uint32_t state[], uint8_t out[])
{
    // IP-1
    // 40,  8, 48, 16, 56, 24, 64, 32,
    // 39,  7, 47, 15, 55, 23, 63, 31,
    // 38,  6, 46, 14, 54, 22, 62, 30,
    // 37,  5, 45, 13, 53, 21, 61, 29,
    // 36,  4, 44, 12, 52, 20, 60, 28,
    // 35,  3, 43, 11, 51, 19, 59, 27,
    // 34,  2, 42, 10, 50, 18, 58, 26,
    // 33,  1, 41,  9, 49, 17, 57, 25};

    out[0] = ShiftBitR(state[1], 7,7) | ShiftBitR(state[0], 7,6) | ShiftBitR(state[1],15,5) | ShiftBitR(state[0],15,4) |
             ShiftBitR(state[1],23,3) | ShiftBitR(state[0],23,2) | ShiftBitR(state[1],31,1) | ShiftBitR(state[0],31,0);

    out[1] = ShiftBitR(state[1], 6,7) | ShiftBitR(state[0], 6,6) | ShiftBitR(state[1],14,5) | ShiftBitR(state[0],14,4) |
             ShiftBitR(state[1],22,3) | ShiftBitR(state[0],22,2) | ShiftBitR(state[1],30,1) | ShiftBitR(state[0],30,0);

    out[2] = ShiftBitR(state[1], 5,7) | ShiftBitR(state[0], 5,6) | ShiftBitR(state[1],13,5) | ShiftBitR(state[0],13,4) |
             ShiftBitR(state[1],21,3) | ShiftBitR(state[0],21,2) | ShiftBitR(state[1],29,1) | ShiftBitR(state[0],29,0);

    out[3] = ShiftBitR(state[1], 4,7) | ShiftBitR(state[0], 4,6) | ShiftBitR(state[1],12,5) | ShiftBitR(state[0],12,4) |
             ShiftBitR(state[1],20,3) | ShiftBitR(state[0],20,2) | ShiftBitR(state[1],28,1) | ShiftBitR(state[0],28,0);

    out[4] = ShiftBitR(state[1], 3,7) | ShiftBitR(state[0], 3,6) | ShiftBitR(state[1],11,5) | ShiftBitR(state[0],11,4) |
             ShiftBitR(state[1],19,3) | ShiftBitR(state[0],19,2) | ShiftBitR(state[1],27,1) | ShiftBitR(state[0],27,0);

    out[5] = ShiftBitR(state[1], 2,7) | ShiftBitR(state[0], 2,6) | ShiftBitR(state[1],10,5) | ShiftBitR(state[0],10,4) |
             ShiftBitR(state[1],18,3) | ShiftBitR(state[0],18,2) | ShiftBitR(state[1],26,1) | ShiftBitR(state[0],26,0);

    out[6] = ShiftBitR(state[1], 1,7) | ShiftBitR(state[0], 1,6) | ShiftBitR(state[1], 9,5) | ShiftBitR(state[0],9,4) |
             ShiftBitR(state[1],17,3) | ShiftBitR(state[0],17,2) | ShiftBitR(state[1],25,1) | ShiftBitR(state[0],25,0);

    out[7] = ShiftBitR(state[1], 0,7) | ShiftBitR(state[0], 0,6) | ShiftBitR(state[1], 8,5) | ShiftBitR(state[0], 8,4) |
             ShiftBitR(state[1],16,3) | ShiftBitR(state[0],16,2) | ShiftBitR(state[1],24,1) | ShiftBitR(state[0],24,0);
}

// This macro converts a 6 bit block with the S-Box row defined as the first and last
// bits to a 6 bit block with the row defined by the first two bits.
#define SBOXBIT(a) (((a) & 0x20) | (((a) & 0x1f) >> 1) | (((a) & 0x01) << 4))

/**************************** VARIABLES *****************************/
static const uint8_t sbox1[64] =
{
    14,  4,  13,  1,   2, 15,  11,  8,   3, 10,   6, 12,   5,  9,   0,  7,
     0, 15,   7,  4,  14,  2,  13,  1,  10,  6,  12, 11,   9,  5,   3,  8,
     4,  1,  14,  8,  13,  6,   2, 11,  15, 12,   9,  7,   3, 10,   5,  0,
    15, 12,   8,  2,   4,  9,   1,  7,   5, 11,   3, 14,  10,  0,   6, 13
};
static const uint8_t sbox2[64] =
{
    15,  1,   8, 14,   6, 11,   3,  4,   9,  7,   2, 13,  12,  0,   5, 10,
     3, 13,   4,  7,  15,  2,   8, 14,  12,  0,   1, 10,   6,  9,  11,  5,
     0, 14,   7, 11,  10,  4,  13,  1,   5,  8,  12,  6,   9,  3,   2, 15,
    13,  8,  10,  1,   3, 15,   4,  2,  11,  6,   7, 12,   0,  5,  14,  9
};
static const uint8_t sbox3[64] =
{
    10,  0,   9, 14,   6,  3,  15,  5,   1, 13,  12,  7,  11,  4,   2,  8,
    13,  7,   0,  9,   3,  4,   6, 10,   2,  8,   5, 14,  12, 11,  15,  1,
    13,  6,   4,  9,   8, 15,   3,  0,  11,  1,   2, 12,   5, 10,  14,  7,
     1, 10,  13,  0,   6,  9,   8,  7,   4, 15,  14,  3,  11,  5,   2, 12
};
static const uint8_t sbox4[64] =
{
     7, 13,  14,  3,   0,  6,   9, 10,   1,  2,   8,  5,  11, 12,   4, 15,
    13,  8,  11,  5,   6, 15,   0,  3,   4,  7,   2, 12,   1, 10,  14,  9,
    10,  6,   9,  0,  12, 11,   7, 13,  15,  1,   3, 14,   5,  2,   8,  4,
     3, 15,   0,  6,  10,  1,  13,  8,   9,  4,   5, 11,  12,  7,   2, 14
};
static const uint8_t sbox5[64] =
{
     2, 12,   4,  1,   7, 10,  11,  6,   8,  5,   3, 15,  13,  0,  14,  9,
    14, 11,   2, 12,   4,  7,  13,  1,   5,  0,  15, 10,   3,  9,   8,  6,
     4,  2,   1, 11,  10, 13,   7,  8,  15,  9,  12,  5,   6,  3,   0, 14,
    11,  8,  12,  7,   1, 14,   2, 13,   6, 15,   0,  9,  10,  4,   5,  3
};
static const uint8_t sbox6[64] =
{
    12,  1,  10, 15,   9,  2,   6,  8,   0, 13,   3,  4,  14,  7,   5, 11,
    10, 15,   4,  2,   7, 12,   9,  5,   6,  1,  13, 14,   0, 11,   3,  8,
     9, 14,  15,  5,   2,  8,  12,  3,   7,  0,   4, 10,   1, 13,  11,  6,
     4,  3,   2, 12,   9,  5,  15, 10,  11, 14,   1,  7,   6,  0,   8, 13
};
static const uint8_t sbox7[64] =
{
     4, 11,   2, 14,  15,  0,   8, 13,   3, 12,   9,  7,   5, 10,   6,  1,
    13,  0,  11,  7,   4,  9,   1, 10,  14,  3,   5, 12,   2, 15,   8,  6,
     1,  4,  11, 13,  12,  3,   7, 14,  10, 15,   6,  8,   0,  5,   9,  2,
     6, 11,  13,  8,   1,  4,  10,  7,   9,  5,   0, 15,  14,  2,   3, 12
};
static const uint8_t sbox8[64] =
{
    13,  2,   8,  4,   6, 15,  11,  1,  10,  9,   3, 14,   5,  0,  12,  7,
     1, 15,  13,  8,  10,  3,   7,  4,  12,  5,   6, 11,   0, 14,   9,  2,
     7, 11,   4,  1,   9, 12,  14,  2,   0,  6,  10, 13,  15,  3,   5,  8,
     2,  1,  14,  7,   4, 10,   8, 13,  15, 12,   9,  0,   3,  5,   6, 11
};

uint32_t DES::F(uint32_t state, const SubKey & key)
{
    uint8_t lrgstate[6]; //,i;

    // Expansion Permutation
    uint32_t t1 = ShiftBitL(state,31, 0) | ((state & 0xf0000000) >>  1) | ShiftBitL(state, 4, 5) |
                  ShiftBitL(state, 3, 6) | ((state & 0x0f000000) >>  3) | ShiftBitL(state, 8,11) |
                  ShiftBitL(state, 7,12) | ((state & 0x00f00000) >>  5) | ShiftBitL(state,12,17) |
                  ShiftBitL(state,11,18) | ((state & 0x000f0000) >>  7) | ShiftBitL(state,16,23);

    uint32_t t2 = ShiftBitL(state,15, 0) | ((state & 0x0000f000) << 15) | ShiftBitL(state,20, 5) |
                  ShiftBitL(state,19, 6) | ((state & 0x00000f00) << 13) | ShiftBitL(state,24,11) |
                  ShiftBitL(state,23,12) | ((state & 0x000000f0) << 11) | ShiftBitL(state,28,17) |
                  ShiftBitL(state,27,18) | ((state & 0x0000000f) <<  9) | ShiftBitL(state, 0,23);

    lrgstate[0] = static_cast<uint8_t>((t1 >> 24) & 0x000000ff);
    lrgstate[1] = static_cast<uint8_t>((t1 >> 16) & 0x000000ff);
    lrgstate[2] = static_cast<uint8_t>((t1 >> 8) & 0x000000ff);
    lrgstate[3] = static_cast<uint8_t>((t2 >> 24) & 0x000000ff);
    lrgstate[4] = static_cast<uint8_t>((t2 >> 16) & 0x000000ff);
    lrgstate[5] = static_cast<uint8_t>((t2 >> 8) & 0x000000ff);

    // Key XOR
    lrgstate[0] ^= key[0];
    lrgstate[1] ^= key[1];
    lrgstate[2] ^= key[2];
    lrgstate[3] ^= key[3];
    lrgstate[4] ^= key[4];
    lrgstate[5] ^= key[5];

    // S-Box Permutation
    state = (sbox1[SBOXBIT(lrgstate[0] >> 2)] << 28) |
            (sbox2[SBOXBIT(((lrgstate[0] & 0x03) << 4) | (lrgstate[1] >> 4))] << 24) |
            (sbox3[SBOXBIT(((lrgstate[1] & 0x0f) << 2) | (lrgstate[2] >> 6))] << 20) |
            (sbox4[SBOXBIT(lrgstate[2] & 0x3f)] << 16) |
            (sbox5[SBOXBIT(lrgstate[3] >> 2)] << 12) |
            (sbox6[SBOXBIT(((lrgstate[3] & 0x03) << 4) | (lrgstate[4] >> 4))] << 8) |
            (sbox7[SBOXBIT(((lrgstate[4] & 0x0f) << 2) | (lrgstate[5] >> 6))] << 4) |
            sbox8[SBOXBIT(lrgstate[5] & 0x3f)];

    // P-Box Permutation
    state = ShiftBitL(state,15,0) | ShiftBitL(state,6,1) | ShiftBitL(state,19,2) |
            ShiftBitL(state,20,3) | ShiftBitL(state,28,4) | ShiftBitL(state,11,5) |
            ShiftBitL(state,27,6) | ShiftBitL(state,16,7) | ShiftBitL(state,0,8) |
            ShiftBitL(state,14,9) | ShiftBitL(state,22,10) | ShiftBitL(state,25,11) |
            ShiftBitL(state,4,12) | ShiftBitL(state,17,13) | ShiftBitL(state,30,14) |
            ShiftBitL(state,9,15) | ShiftBitL(state,1,16) | ShiftBitL(state,7,17) |
            ShiftBitL(state,23,18) | ShiftBitL(state,13,19) | ShiftBitL(state,31,20) |
            ShiftBitL(state,26,21) | ShiftBitL(state,2,22) | ShiftBitL(state,8,23) |
            ShiftBitL(state,18,24) | ShiftBitL(state,12,25) | ShiftBitL(state,29,26) |
            ShiftBitL(state,5,27) | ShiftBitL(state,21,28) | ShiftBitL(state,10,29) |
            ShiftBitL(state,3,30) | ShiftBitL(state,24,31);

    // Return the final state value
    return(state);
}

void DES::ProcessBlock(const DataBlock dataIn, DataBlock dataOut)
{
    uint32_t state[2];

    IP(state, dataIn);

    for (int idx = 0; idx < 15; ++idx)
    {
        uint32_t temp = state[1];
        state[1] = F(state[1], _keyset[idx]) ^ state[0];
        state[0] = temp;
    }
    // Perform the final loop manually as it doesn't switch sides
    state[0] = F(state[1], _keyset[15]) ^ state[0];

    IPInv(state, dataOut);
}

TripleDES::TripleDES()
    : _des1()
    , _des2()
    , _des3()
{

}

void TripleDES::Initialize(const TripleDESKey & key, Direction direction)
{
    DESKey key1 {};
    DESKey key2 {};
    DESKey key3 {};
    if (direction == Direction::Encrypt)
    {
        memcpy(key1, &key[0], DESKeySize);
        memcpy(key2, &key[DESKeySize], DESKeySize);
        memcpy(key3, &key[2 * DESKeySize], DESKeySize);
        _des1.Initialize(key1, Direction::Encrypt);
        _des2.Initialize(key2, Direction::Decrypt);
        _des3.Initialize(key3, Direction::Encrypt);
    }
    else
    {
        memcpy(key1, &key[2 * DESKeySize], DESKeySize);
        memcpy(key2, &key[DESKeySize], DESKeySize);
        memcpy(key3, &key[0], DESKeySize);
        _des1.Initialize(key1, Direction::Decrypt);
        _des2.Initialize(key2, Direction::Encrypt);
        _des3.Initialize(key3, Direction::Decrypt);
    }
}

void TripleDES::Process(const uint8_t * dataIn, uint8_t * dataOut, size_t len)
{
    size_t offset = 0;
    DataBlock blockIn {};
    DataBlock blockOut {};
    while (offset < len)
    {
        size_t numBytes = (len - offset >= BlockSize) ? BlockSize : len - offset;
        memcpy(blockIn, dataIn + offset, numBytes);
        if (numBytes < BlockSize)
            memset(blockIn + numBytes, 0, BlockSize - numBytes);
        ProcessBlock(blockIn, blockOut);
        memcpy(dataOut + offset, blockOut, numBytes);
        offset += numBytes;
    }
}

void TripleDES::Process(const OSAL::ByteArray & dataIn, OSAL::ByteArray & dataOut)
{
    dataOut.Size(dataIn.Size());
    Process(dataIn.Data(), dataOut.Data(), dataIn.Size());
}

void TripleDES::Finalize()
{

}

OSAL::String TripleDES::ToString() const
{
    basic_ostringstream<OSAL::Char> stream;
    stream << "BlockCipher 1: " << _des1.ToString() << endl;
    stream << "BlockCipher 2: " << _des2.ToString() << endl;
    stream << "BlockCipher 3: " << _des3.ToString() << endl;
    return stream.str();
}

void TripleDES::ProcessBlock(const DataBlock dataIn, DataBlock dataOut)
{
    DataBlock dataTemp1;
    DataBlock dataTemp2;
    _des1.ProcessBlock(dataIn, dataTemp1);
    _des2.ProcessBlock(dataTemp1, dataTemp2);
    _des3.ProcessBlock(dataTemp2, dataOut);
}
