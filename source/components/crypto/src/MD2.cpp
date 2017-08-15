// MD2 computation

#include "crypto/MD2.h"

#include <iomanip>
#include <iostream>

using namespace std;
using namespace Crypto;

static constexpr size_t NumWorkspace = 16;
static constexpr size_t NumWorkspaceMinusOne = NumWorkspace - 1;
static constexpr size_t NumRounds = 18;

static const uint8_t s[256] =
{
    41, 46, 67, 201, 162, 216, 124, 1, 61, 54, 84, 161, 236, 240, 6,
    19, 98, 167, 5, 243, 192, 199, 115, 140, 152, 147, 43, 217, 188,
    76, 130, 202, 30, 155, 87, 60, 253, 212, 224, 22, 103, 66, 111, 24,
    138, 23, 229, 18, 190, 78, 196, 214, 218, 158, 222, 73, 160, 251,
    245, 142, 187, 47, 238, 122, 169, 104, 121, 145, 21, 178, 7, 63,
    148, 194, 16, 137, 11, 34, 95, 33, 128, 127, 93, 154, 90, 144, 50,
    39, 53, 62, 204, 231, 191, 247, 151, 3, 255, 25, 48, 179, 72, 165,
    181, 209, 215, 94, 146, 42, 172, 86, 170, 198, 79, 184, 56, 210,
    150, 164, 125, 182, 118, 252, 107, 226, 156, 116, 4, 241, 69, 157,
    112, 89, 100, 113, 135, 32, 134, 91, 207, 101, 230, 45, 168, 2, 27,
    96, 37, 173, 174, 176, 185, 246, 28, 70, 97, 105, 52, 64, 126, 15,
    85, 71, 163, 35, 221, 81, 175, 58, 195, 92, 249, 206, 186, 197,
    234, 38, 44, 83, 13, 110, 133, 40, 132, 9, 211, 223, 205, 244, 65,
    129, 77, 82, 106, 220, 55, 200, 108, 193, 171, 250, 36, 225, 123,
    8, 12, 189, 177, 74, 120, 136, 149, 139, 227, 99, 232, 109, 233,
    203, 213, 254, 59, 0, 29, 57, 242, 239, 183, 14, 102, 88, 208, 228,
    166, 119, 114, 248, 235, 117, 75, 10, 49, 68, 80, 180, 143, 237,
    31, 26, 219, 153, 141, 51, 159, 17, 131, 20
};

MD2::MD2()
    : _digest()
    , _buffer()
    , _byteCount()
    , _state()
    , _checksum()
{
    Initialize();
}

void MD2::Initialize()
{
    _byteCount = 0;

    memset(_state, 0, sizeof(_state));
    memset(_checksum, 0, sizeof(_checksum));
    memset(_digest, 0, sizeof(_digest));
}

// Update the hash value
void MD2::Process(const uint8_t *data, size_t len)
{
    uint32_t filledBlock = static_cast<uint32_t>(_byteCount & BlockSizeMinusOne);
    // Calculate bytes mod 64 or bit mod 512

    _byteCount += len;          // Add number of bytes for this block
                                // (we expect no more than 2^64 bytes in total)

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

void MD2::Process(const OSAL::ByteArray & data)
{
    Process(data.Data(), data.Size());
}

void MD2::Finalize()
{
    uint8_t block[BlockSize];
    size_t padSize = BlockSize - (_byteCount & BlockSizeMinusOne);

    size_t offset = 0;
    while (offset < padSize)
        block[offset++] = static_cast<uint8_t>(padSize);

    Process(block, padSize);
    Process(_checksum, sizeof(_checksum));
    for (size_t i = 0; i < DigestSize; i++)
    {
        _digest[i] = _state[i];
    }

    memset(_buffer, 0, BlockSize);
    memset(_state, 0, sizeof(_state));
    memset(_checksum, 0, sizeof(_checksum));
    _byteCount = 0;
}

OSAL::ByteArray MD2::GetDigest() const
{
    OSAL::ByteArray digest(_digest, sizeof(_digest));
    return digest;
}

OSAL::String MD2::ToString() const
{
    basic_ostringstream<OSAL::Char> stream;

    for (size_t i = 0; i < sizeof(_digest); ++i)
    {
        stream << uppercase << hex << setw(2) << setfill('0') << static_cast<int>(_digest[i]);
    }
    return stream.str();
}

void MD2::DumpState()
{
    cout << "State: " << endl;
    for (size_t i = 0; i < StateSize; ++i)
    {
        cout << setw(2) << setfill('0') << i << " = " << hex << setw(2) << setfill('0') << uppercase << int(_state[i]) << dec
             << endl;
    }
}

void MD2::DumpBlock(const uint8_t buffer[BlockSize])
{
    cout << "W: " << endl;
    for (size_t i = 0; i < BlockSize; ++i)
    {
        cout << "W" << setw(2) << setfill('0') << i << " = " << hex << setw(2) << setfill('0') << uppercase
             << int(buffer[i]) << dec << endl;
    }
}

void MD2::Transform(const uint8_t buffer[BlockSize])
{
    for (size_t i = 0; i < BlockSize; ++i)
    {
        _state[i + 16] = buffer[i];
        _state[i + 32] = (_state[i + 16] ^ _state[i]);
    }
    uint8_t t = 0;

    for (size_t j = 0; j < NumRounds; ++j)
    {
        for (size_t i = 0; i < StateSize; ++i)
        {
             _state[i] ^= s[t];
            t = _state[i];
        }
        t = static_cast<uint8_t>((t + j) & 0xFF);
    }

    t = _checksum[BlockSizeMinusOne];
    for (size_t i = 0; i < BlockSize; ++i)
    {
        uint8_t c = buffer[i];
        _checksum[i] ^= s[c ^ t];
        t = _checksum[i];
    }
}
