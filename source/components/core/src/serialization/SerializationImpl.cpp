#include <core/serialization/SerializationImpl.h>

#include <bitset>
#include <climits>
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;

string Core::Serialize(bool value)
{
    return value ? BooleanTrueLiteral : BooleanFalseLiteral;
}

string Core::Serialize(int8_t value, int base)
{
    std::ostringstream stream;

    switch (base)
    {
        case 2:
        {
            std::bitset<8> x(static_cast<uint8_t>(value));
            stream << std::setfill('0') << std::setw(8) << x;
        }
            break;
        case 8:
        {
            stream << std::oct << std::setfill('0') << std::setw(3) << (int)value;
        }
            break;
        case 10:
        {
            stream << std::dec << std::setfill('0') << (int)value;
        }
            break;
        case 16:
        {
            stream << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)value;
        }
            break;
        default:
            assert(false);
    }
    return stream.str();
}

string Core::Serialize(uint8_t value, int base)
{
    std::ostringstream stream;

    switch (base)
    {
        case 2:
        {
            std::bitset<8> x(value);
            stream << std::setfill('0') << std::setw(8) << x;
        }
            break;
        case 8:
        {
            stream << std::oct << std::setfill('0') << std::setw(3) << (int)value;
        }
            break;
        case 10:
        {
            stream << std::dec << std::setfill('0') << (int)value;
        }
            break;
        case 16:
        {
            stream << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)value;
        }
            break;
        default:
            assert(false);
    }
    return stream.str();
}

string Core::Serialize(int16_t value, int base)
{
    std::ostringstream stream;

    switch (base)
    {
        case 2:
        {
            std::bitset<16> x(static_cast<uint16_t>(value));
            stream << std::setfill('0') << std::setw(16) << x;
        }
            break;
        case 8:
        {
            stream << std::oct << std::setfill('0') << std::setw(6) << value;
        }
            break;
        case 10:
        {
            stream << std::dec << std::setfill('0') << value;
        }
            break;
        case 16:
        {
            stream << std::hex << std::uppercase << std::setfill('0') << std::setw(4) << value;
        }
            break;
        default:
            assert(false);
    }
    return stream.str();
}

string Core::Serialize(uint16_t value, int base)
{
    std::ostringstream stream;

    switch (base)
    {
        case 2:
        {
            std::bitset<16> x(value);
            stream << std::setfill('0') << std::setw(16) << x;
        }
            break;
        case 8:
        {
            stream << std::oct << std::setfill('0') << std::setw(6) << value;
        }
            break;
        case 10:
        {
            stream << std::dec << std::setfill('0') << value;
        }
            break;
        case 16:
        {
            stream << std::hex << std::uppercase << std::setfill('0') << std::setw(4) << value;
        }
            break;
        default:
            assert(false);
    }
    return stream.str();
}

string Core::Serialize(int32_t value, int base)
{
    std::ostringstream stream;

    switch (base)
    {
        case 2:
        {
            std::bitset<32> x(static_cast<uint32_t>(value));
            stream << std::setfill('0') << std::setw(32) << x;
        }
            break;
        case 8:
        {
            stream << std::oct << std::setfill('0') << std::setw(11) << value;
        }
            break;
        case 10:
        {
            stream << std::dec << std::setfill('0') << value;
        }
            break;
        case 16:
        {
            stream << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << value;
        }
            break;
        default:
            assert(false);
    }
    return stream.str();
}

string Core::Serialize(uint32_t value, int base)
{
    std::ostringstream stream;

    switch (base)
    {
        case 2:
        {
            std::bitset<32> x(value);
            stream << std::setfill('0') << std::setw(32) << x;
        }
            break;
        case 8:
        {
            stream << std::oct << std::setfill('0') << std::setw(11) << value;
        }
            break;
        case 10:
        {
            stream << std::dec << std::setfill('0') << value;
        }
            break;
        case 16:
        {
            stream << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << value;
        }
            break;
        default:
            assert(false);
    }
    return stream.str();
}

string Core::Serialize(int64_t value, int base)
{
    std::ostringstream stream;

    switch (base)
    {
        case 2:
        {
            std::bitset<64> x(static_cast<uint64_t>(value));
            stream << std::setfill('0') << std::setw(64) << x;
        }
            break;
        case 8:
        {
            stream << std::oct << std::setfill('0') << std::setw(22) << value;
        }
            break;
        case 10:
        {
            stream << std::dec << std::setfill('0') << value;
        }
            break;
        case 16:
        {
            stream << std::hex << std::uppercase << std::setfill('0') << std::setw(16) << value;
        }
            break;
        default:
            assert(false);
    }
    return stream.str();
}

string Core::Serialize(uint64_t value, int base)
{
    std::ostringstream stream;

    switch (base)
    {
        case 2:
        {
            std::bitset<64> x(value);
            stream << std::setfill('0') << std::setw(64) << x;
        }
            break;
        case 8:
        {
            stream << std::oct << std::setfill('0') << std::setw(22) << value;
        }
            break;
        case 10:
        {
            stream << std::dec << std::setfill('0') << value;
        }
            break;
        case 16:
        {
            stream << std::hex << std::uppercase << std::setfill('0') << std::setw(16) << value;
        }
            break;
        default:
            assert(false);
    }
    return stream.str();
}

string Core::Serialize(float value, int precision)
{
    std::ostringstream stream;

    stream << std::setprecision(precision) << value;

    return stream.str();
}

string Core::Serialize(double value, int precision)
{
    std::ostringstream stream;

    stream << std::setprecision(precision) << value;

    return stream.str();
}

string Core::Serialize(long double value, int precision)
{
    std::ostringstream stream;

    stream << std::setprecision(precision) << value;

    return stream.str();
}

