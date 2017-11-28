#include "core/Util.h"

#include <bitset>
#include <climits>
#include <cmath>
#include <cstring>
#include <core/Core.h>
#include <core/serialization/SerializationImpl.h>
//#include "core/DefaultLogger.h"
//#include "core/String.h"

using namespace std;
using namespace Core;

size_t Util::NextPowerOfTwo(size_t value)
{
    size_t rest = value;
    size_t numberOfOnes = 0;
    size_t power = 0;
    while (rest != 0)
    {
        power++;
        if ((rest & 0x01) != 0)
            numberOfOnes++;
        rest >>= 1;
    }
    return static_cast<size_t>((numberOfOnes != 1) ? 1 << power : 1 << (power - 1));
}

OSAL::Time::timespec Util::MilliSecondsToTimespec(int ms)
{
    OSAL::Time::timespec time;
    time.tv_sec = ms / MilliSecondsPerSecond;
    time.tv_nsec = (ms % MilliSecondsPerSecond) * NanoSecondsPerMilliSecond;
    return time;
}

//timespec Util::ConvertTimeOffsetToTimeSpec(int offsetMS)
//{
//    timespec time;
//    clock_gettime(CLOCK_REALTIME, &time);
//    time.tv_nsec += (offsetMS % 1000) * 1000000;
//    time.tv_sec += offsetMS / 1000;
//    time.tv_sec += time.tv_nsec / 1000000000;
//    time.tv_nsec %= 1000000000;
//    return time;
//}
//
//timespec Util::ConvertDurationToTimeSpec(std::chrono::milliseconds duration)
//{
//    timespec time;
//    time.tv_sec = duration.count() / 1000;
//    time.tv_nsec = (duration.count() % 1000) * 1000000;
//    return time;
//}
//
//timespec Util::ConvertDurationToTimeSpec(std::chrono::microseconds duration)
//{
//    timespec time;
//    time.tv_sec = duration.count() / 1000000;
//    time.tv_nsec = (duration.count() % 1000000) * 1000;
//    return time;
//}
//
//timespec Util::ConvertDurationToTimeSpec(std::chrono::nanoseconds duration)
//{
//    timespec time;
//    time.tv_sec = duration.count() / 1000000000;
//    time.tv_nsec = duration.count() % 1000000000;
//    return time;
//}
//
void Util::Sleep(int sleepMS)
{
    OSAL::Time::timespec sleepTime = MilliSecondsToTimespec(sleepMS);
    int errorCode;
    do
    {
        errorCode = OSAL::Time::nanosleep(&sleepTime, &sleepTime);
        if (errorCode == -1)
            errorCode = errno;
    }
    while (errorCode == EINTR);
    OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
}

//int Util::KBHit()
//{
//    int character;
//    struct termios orig_term_attr;
//    struct termios new_term_attr;
//
//    /* set the terminal to raw mode */
//    tcgetattr(fileno(stdin), &orig_term_attr);
//    memcpy(&new_term_attr, &orig_term_attr, sizeof(struct termios));
//    new_term_attr.c_lflag &= ~(ECHO|ICANON);
//    new_term_attr.c_cc[VTIME] = 0;
//    new_term_attr.c_cc[VMIN] = 0;
//    tcsetattr(fileno(stdin), TCSANOW, &new_term_attr);
//
//    /* read a character from the stdin stream without blocking */
//    /*   returns EOF (-1) if no character is available */
//    character = fgetc(stdin);
//
//    /* restore the original terminal attributes */
//    tcsetattr(fileno(stdin), TCSANOW, &orig_term_attr);
//
//    return character;
//}
//
//int Util::NumDigits(int64_t n) {
//    return (n != 0) ? static_cast<int>(std::log10(std::abs(static_cast<double>(n)))) + 1 : 1;
//}
//
//void Util::ZeroMemory(void * destination, int size)
//{
//    ZeroMemory(destination, 0, size);
//}
//void Util::ZeroMemory(void * destination, int offset, int size)
//{
//    memset((uint8_t *)destination + offset, 0, size);
//}
//
//void Util::CopyMemory(void * destination, const void * source, int size)
//{
//    CopyMemory(destination, 0, source, 0, size);
//}
//void Util::CopyMemory(void * destination, int offsetDestination, const void * source, int size)
//{
//    CopyMemory(destination, offsetDestination, source, 0, size);
//}
//void Util::CopyMemory(void * destination, const void * source, int offsetSource, int size)
//{
//    CopyMemory(destination, 0, source, offsetSource, size);
//}
//void Util::CopyMemory(void * destination, int offsetDestination, const void * source, int offsetSource, int size)
//{
//    memcpy((uint8_t *)destination + offsetDestination, (uint8_t *)source + offsetSource, size);
//}
//
//int Util::NextPowerOfTwo(int value)
//{
//    int rest = value;
//    int numberOfOnes = 0;
//    int power = 0;
//    while (rest != 0)
//    {
//        power++;
//        if ((rest & 0x01) != 0)
//            numberOfOnes++;
//        rest >>= 1;
//    }
//    return (numberOfOnes != 1) ? 1 << power : 1 << (power - 1);
//}
//
//void Util::DumpToStream(std::ostream & stream, const ByteArray & data)
//{
//    DumpToStream(stream, data.Data(), data.Size());
//}
//
//static const int BytesPerRow = 16;
//
//void Util::DumpToStream(ostream & stream, const uint8_t * data, int length)
//{
//    for (int offset = 0; offset < length; offset += BytesPerRow)
//    {
//        for (int i = 0; i < BytesPerRow; i++)
//        {
//            if (i + offset < length)
//            {
//                uint8_t value = data[i + offset];
//                stream << hex << setw(2) << setfill('0') << (int)value << " ";
//            }
//            else
//            {
//                stream << "   ";
//            }
//        }
//        for (int i = 0; i < BytesPerRow; i++)
//        {
//            if (i + offset < length)
//            {
//                uint8_t value = data[i + offset];
//                stream << (char)(((value >= 32) && (value < 128)) ? value : '?') << " ";
//            }
//        }
//        stream << endl;
//    }
//    stream << endl << flush;
//}

static const unsigned char charToBase64[256] =
{
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255, 62,255,255,255, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61,255,255,255,  0,255,255,
    255,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,255,255,255,255,255,
    255, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
};

static const char base64ToChar[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void Util::Base64Encode(const OSAL::ByteArray & input, OSAL::String & output)
{
    output.clear();
    int index = 0;
    int size = input.Size();
    while (index < size - 2)
    {
        output += base64ToChar[(input[index + 0] & 0xFC) >> 2];
        output += base64ToChar[(input[index + 0] & 0x03) << 4 | (input[index + 1] & 0xF0) >> 4];
        output += base64ToChar[(input[index + 1] & 0x0F) << 2 | (input[index + 2] & 0xC0) >> 6];
        output += base64ToChar[(input[index + 2] & 0x3F) >> 0];
        index += 3;
    }
    if ((size % 3) == 1)
    {
        output += base64ToChar[(input[index + 0] & 0xFC) >> 2];
        output += base64ToChar[(input[index + 0] & 0x03) << 4];
        output += "==";
    }
    if ((size % 3) == 2)
    {
        output += base64ToChar[(input[index + 0] & 0xFC) >> 2];
        output += base64ToChar[(input[index + 0] & 0x03) << 4 | (input[index + 1] & 0xF0) >> 4];
        output += base64ToChar[(input[index + 1] & 0x0F) << 2];
        output += '=';
    }
}

OSAL::String Util::Base64Encode(const OSAL::ByteArray & input)
{
    OSAL::String result;
    Base64Encode(input, result);
    return result;
}

void Util::Base64Decode(const OSAL::String & input, OSAL::ByteArray & output)
{
    Base64Decode(input.c_str(), output);
}

OSAL::ByteArray Util::Base64Decode(const OSAL::String & input)
{
    OSAL::ByteArray result;
    Base64Decode(input.c_str(), result);
    return result;
}

OSAL::ByteArray Util::Base64Decode(const OSAL::Char * input)
{
    OSAL::ByteArray result;
    Base64Decode(input, result);
    return result;
}

void Util::Base64Decode(const OSAL::Char * input, OSAL::ByteArray & output)
{
    if (input == nullptr)
        throw OSAL::ArgumentNullException(__func__, __FILE__, __LINE__, "dataBase64");
    int index = 0;
    int offset = 0;
    int length = strlen(input);
    while (index < length)
    {
        uint8_t base64_0 {};
        uint8_t base64_1 {};
        uint8_t base64_2 {};
        uint8_t base64_3 {};
        bool endCharFound {};

        base64_0 = charToBase64[size_t(input[index])];
        if (base64_0 == 0xFF)
            throw OSAL::ArgumentException(__func__, __FILE__, __LINE__, "dataBase64",
                                          "Incorrect format for base64 string at offset " + Serialize(index) +
                                          ": " + string(input));
        index++;

        if (index >= length)
            throw OSAL::ArgumentException(__func__, __FILE__, __LINE__, "dataBase64",
                                          "Incorrect format for base64 string, expected more input at offset " + Serialize(index) +
                                          ": " + string(input));
        base64_1 = charToBase64[size_t(input[index])];
        if (base64_1 == 0xFF)
            throw OSAL::ArgumentException(__func__, __FILE__, __LINE__, "dataBase64",
                                          "Incorrect format for base64 string at offset " + Serialize(index) +
                                          ": " + string(input));
        output.SetUInt8(offset++, (base64_0 & 0x3F) << 2 | (base64_1 & 0x30) >> 4);
        index++;

        if ((index < length) & (input[index] == '='))
        {
            endCharFound = true;
        }
        if ((index < length) & !endCharFound)
        {
            base64_2 = charToBase64[size_t(input[index])];
            if (base64_2 == 0xFF)
                throw OSAL::ArgumentException(__func__, __FILE__, __LINE__, "dataBase64",
                                              "Incorrect format for base64 string at offset " + Serialize(index) +
                                              ": " + string(input));
            output.SetUInt8(offset++, (base64_1 & 0x0F) << 4 | (base64_2 & 0x3C) >> 2);
        }
        index++;

        if ((index < length) & endCharFound && (input[index] != '='))
        {
            throw OSAL::ArgumentException(__func__, __FILE__, __LINE__, "dataBase64",
                                          "Expected end of string or '=' for base64 string at offset " + Serialize(index) +
                                          ": " + string(input));
        }
        if ((index < length) & (input[index] == '='))
        {
            endCharFound = true;
        }
        if ((index < length) & !endCharFound)
        {
            base64_3 = charToBase64[size_t(input[index])];
            if (base64_3 == 0xFF)
                throw OSAL::ArgumentException(__func__, __FILE__, __LINE__, "dataBase64",
                                              "Incorrect format for base64 string at offset " + Serialize(index) +
                                              ": " + string(input));
            output.SetUInt8(offset++, (base64_2 & 0x03) << 6 | (base64_3 & 0x3F) >> 0);
        }
        index++;
    }
}

//class FileWrapper
//{
//public:
//    FileWrapper() :
//        handle(nullptr)
//    {
//    }
//    FileWrapper(const FileWrapper &) = delete;
//    ~FileWrapper()
//    {
//        Close();
//    }
//
//    FileWrapper & operator = (const FileWrapper &) = delete;
//
//    operator FILE * ()
//    {
//        return handle;
//    }
//    bool Open(const char * path, const char * mode)
//    {
//        Close();
//        handle = fopen(path, mode);
//        return (handle != nullptr);
//    }
//    void Close()
//    {
//        if (handle != nullptr)
//            fclose(handle);
//        handle = nullptr;
//    }
//    size_t Size()
//    {
//        fseek(handle, 0, SEEK_END);
//        size_t result = ftell(handle);
//        fseek(handle, 0, SEEK_SET);
//        return result;
//    }
//    size_t Read(uint8_t * buffer, size_t numBytes)
//    {
//        return fread(buffer, sizeof(uint8_t), numBytes, handle);
//    }
//    size_t Write(const uint8_t * buffer, size_t numBytes)
//    {
//        return fwrite(buffer, sizeof(uint8_t), numBytes, handle);
//    }
//
//protected:
//    FILE * handle;
//};
//
//class MallocBuffer
//{
//public:
//    MallocBuffer() = delete;
//    MallocBuffer(const MallocBuffer &) = delete;
//    MallocBuffer(size_t size) :
//        size(size),
//        contents(nullptr)
//    {
//        contents = (char *)malloc(size);
//    }
//    ~MallocBuffer()
//    {
//        free(contents);
//        size = 0;
//        contents = nullptr;
//    }
//
//    MallocBuffer & operator = (const MallocBuffer &) = delete;
//
//    size_t size;
//    char * contents;
//};
//
//ssize_t ReadLine(MallocBuffer & buffer, FILE * file)
//{
//    ssize_t count = getline(&buffer.contents, &buffer.size, file);
//    return count;
//}
//
//bool Util::CompareFiles(const char* pathA, const char* pathB)
//{
//    bool result = false;
//    FileWrapper fileA;
//    FileWrapper fileB;
//
//    if (fileA.Open(pathA, "rb"))
//    {
//        if (fileB.Open(pathB, "rb"))
//        {
//            int fileSizeA = fileA.Size();
//            int fileSizeB = fileB.Size();
//            if ((fileSizeA != fileSizeB))
//                return false;
//            ByteArray bufferA(fileSizeA);
//            ByteArray bufferB(fileSizeB);
//            size_t bytesReadA = fileA.Read(bufferA, fileSizeA);
//            size_t bytesReadB = fileB.Read(bufferB, fileSizeB);
//            if (bytesReadA != (size_t)fileSizeA)
//                return false;
//            if (bytesReadB != (size_t)fileSizeB)
//                return false;
//           result = Compare(bufferA.Data(), bufferB.Data(), fileSizeA);
//        }
//    }
//    return result;
//}
//
//bool Util::CompareFiles(const string & pathA, const string & pathB)
//{
//    return CompareFiles(pathA.c_str(), pathB.c_str());
//}
//
//bool CompareLines(MallocBuffer & bufferA, FileWrapper & fileA, MallocBuffer & bufferB, FileWrapper & fileB)
//{
//    int line = 1;
//    while (!feof(fileA) && !feof(fileB))
//    {
//        ssize_t countA = ReadLine(bufferA, fileA);
//        ssize_t countB = ReadLine(bufferB, fileB);
//        if ((countA == -1) && (countB == -1))
//            return true;
//        if ((countA == -1) || (countB == -1))
//        {
//            ostringstream stream;
//            stream << "Difference between text files in line " << line;
//            TheLogger().Warn("Util::CompareTextFiles", stream.str());
//            return false;
//        }
//        if (countA != countB)
//        {
//            ostringstream stream;
//            stream << "Difference between text files in line " << line;
//            TheLogger().Warn("Util::CompareTextFiles", stream.str());
//            return false;
//        }
//        if (strcmp(bufferA.contents, bufferB.contents) != 0)
//        {
//            ostringstream stream;
//            stream << "Difference between text files in line " << line;
//            TheLogger().Warn("Util::CompareTextFiles", stream.str());
//            return false;
//        }
//        line++;
//    }
//    return (feof(fileA) && feof(fileB));
//}
//
//bool Util::CompareTextFiles(const char* pathA, const char* pathB)
//{
//    bool result = false;
//    FileWrapper fileA;
//    FileWrapper fileB;
//
//    if (fileA.Open(pathA, "r"))
//    {
//        if (fileB.Open(pathB, "r"))
//        {
//            const size_t BufferSize = 1024;
//            MallocBuffer bufferA(BufferSize);
//            MallocBuffer bufferB(BufferSize);
//            result = CompareLines(bufferA, fileA, bufferB, fileB);
//        }
//    }
//    return result;
//}
//
//bool Util::CompareTextFiles(const string & pathA, const string & pathB)
//{
//    return CompareTextFiles(pathA.c_str(), pathB.c_str());
//}
//
//bool CompareLogLines(MallocBuffer & bufferA, FileWrapper & fileA, MallocBuffer & bufferB, FileWrapper & fileB)
//{
//    while (!feof(fileA) && !feof(fileB))
//    {
//        ssize_t countA = ReadLine(bufferA, fileA);
//        ssize_t countB = ReadLine(bufferB, fileB);
//        if ((countA == -1) || (countB == -1))
//            break;
//        if (countA != countB)
//        {
//            return false;
//        }
//        string stringA = string(bufferA.contents, countA);
//        string stringB = string(bufferB.contents, countB);
//        stringA = stringA.substr(stringA.find(" - "));
//        stringB = stringB.substr(stringB.find(" - "));
//        if (stringA != stringB)
//            return false;
//    }
//    return (feof(fileA) && feof(fileB));
//}
//
//bool Util::CompareLogFiles(const char* pathA, const char* pathB)
//{
//    bool result = false;
//    FileWrapper fileA;
//    FileWrapper fileB;
//
//    if (fileA.Open(pathA, "r"))
//    {
//        if (fileB.Open(pathB, "r"))
//        {
//            const size_t BufferSize = 1024;
//            MallocBuffer bufferA(BufferSize);
//            MallocBuffer bufferB(BufferSize);
//            result = CompareLogLines(bufferA, fileA, bufferB, fileB);
//        }
//    }
//    return result;
//}
//
//bool Util::CompareLogFiles(const string & pathA, const string & pathB)
//{
//    return CompareLogFiles(pathA.c_str(), pathB.c_str());
//}
//
//bool Util::Compare(double expected, double actual, double epsilonAbs, double epsilonRel)
//{
//    if (std::isinf(expected) && std::isinf(actual))
//    {
//        return (std::isinf(expected) == std::isinf(actual));
//    }
//    double errorAbs = fabs(expected - actual);
//    double deltaMax = max(epsilonAbs, epsilonRel * max(fabs(actual), fabs(expected)));
//    return (errorAbs <= deltaMax);
//}
//bool Util::Compare(float expected, float actual, float epsilonAbs, float epsilonRel)
//{
//    if (std::isinf(expected) && std::isinf(actual))
//    {
//        return (std::isinf(expected) == std::isinf(actual));
//    }
//    double errorAbs = fabsf(expected - actual);
//    double deltaMax = max(epsilonAbs, epsilonRel * max(fabsf(actual), fabsf(expected)));
//    return (errorAbs <= deltaMax);
//}
//
