#include <core/serialization/DeserializationImpl.h>

#include <climits>
#include <cfloat>
#include <ctgmath>

static bool HasValidCharactersForBase(const std::string & text, int base, bool maybeSigned)
{
    if (text.empty())
        return false;
    switch (base)
    {
        case 2:
            for (size_t i = 0; i < text.length(); i++)
            {
                if (OSAL::Strings::strchr("01", static_cast<char>(OSAL::Strings::toupper(text[i]))) == nullptr)
                    return false;
            }
            break;
        case 8:
            for (size_t i = 0; i < text.length(); i++)
            {
                if (OSAL::Strings::strchr("01234567", static_cast<char>(OSAL::Strings::toupper(text[i]))) == nullptr)
                    return false;
            }
            break;
        case 10:
            for (size_t i = 0; i < text.length(); i++)
            {
                if (maybeSigned)
                {
                    if (OSAL::Strings::strchr("0123456789+-",
                                              static_cast<char>(OSAL::Strings::toupper(text[i]))) == nullptr)
                        return false;
                }
                else
                {
                    if (OSAL::Strings::strchr("0123456789+",
                                              static_cast<char>(OSAL::Strings::toupper(text[i]))) == nullptr)
                        return false;
                }
            }
            break;
        case 16:
            for (size_t i = 0; i < text.length(); i++)
            {
                if (OSAL::Strings::strchr("0123456789ABCDEF", static_cast<char>(OSAL::Strings::toupper(text[i]))) == nullptr)
                    return false;
            }
            break;
        default:
            return false;
    }
    return true;
}

static bool HasValidCharactersFloatingPoint(const std::string & text)
{
    if (text.empty())
        return false;
    for (size_t i = 0; i < text.length(); i++)
    {
        if (OSAL::Strings::strchr("0123456789.+-Ee", static_cast<char>(OSAL::Strings::toupper(text[i]))) == nullptr)
            return false;
    }
    return true;
}


bool Core::Deserialize(const std::string & text, bool & value)
{
    if (OSAL::Strings::IsEqualIgnoreCase("true", text))
    {
        value = true;
        return true;
    }
    if (OSAL::Strings::IsEqualIgnoreCase("false", text))
    {
        value = false;
        return true;
    }

    return false;
}

bool Core::Deserialize(const std::string & text, int8_t & value, int base)
{
    value = {};
    if (!HasValidCharactersForBase(text, base, true))
        return false;

    long result = OSAL::Strings::strtol(text.c_str(), nullptr, base);
    if ((base == 10) && ((result < CHAR_MIN) || (result > CHAR_MAX)))
        return false;
    value = static_cast<int8_t>(result);

    return true;
}

bool Core::Deserialize(const std::string & text, uint8_t & value, int base /*= 10*/)
{
    value = {};
    if (!HasValidCharactersForBase(text, base, false))
        return false;

    long result = OSAL::Strings::strtol(text.c_str(), nullptr, base);
    if ((base == 10) && ((result < 0) || (result > UCHAR_MAX)))
        return false;
    value = static_cast<uint8_t>(result);

    return true;
}

bool Core::Deserialize(const std::string & text, int16_t & value, int base /*= 10*/)
{
    value = {};
    if (!HasValidCharactersForBase(text, base, true))
        return false;

    long result = OSAL::Strings::strtol(text.c_str(), nullptr, base);
    if ((base == 10) && ((result < SHRT_MIN) || (result > SHRT_MAX)))
        return false;
    value = static_cast<int16_t>(result);

    return true;
}

bool Core::Deserialize(const std::string & text, uint16_t & value, int base /*= 10*/)
{
    value = {};
    if (!HasValidCharactersForBase(text, base, false))
        return false;

    long result = OSAL::Strings::strtol(text.c_str(), nullptr, base);
    if ((base == 10) && ((result < 0) || (result > USHRT_MAX)))
        return false;
    value = static_cast<uint16_t>(result);

    return true;
}

bool Core::Deserialize(const std::string & text, int32_t & value, int base /*= 10*/)
{
    value = {};
    if (!HasValidCharactersForBase(text, base, true))
        return false;

    long result = OSAL::Strings::strtol(text.c_str(), nullptr, base);
    unsigned long result_ul = OSAL::Strings::strtoul(text.c_str(), nullptr, base);
    if (result < INT_MIN)
        return false;
    if ((base == 10) && (result >= 0) && (result_ul > INT_MAX))
        return false;
    if ((base != 10) && (result_ul > INT_MAX))
        value = static_cast<int32_t>(result_ul);
    else
        value = static_cast<int32_t>(result);

    return true;
}

bool Core::Deserialize(const std::string & text, uint32_t & value, int base /*= 10*/)
{
    value = {};
    if (!HasValidCharactersForBase(text, base, false))
        return false;

    unsigned long long result = OSAL::Strings::strtoull(text.c_str(), nullptr, base);
    if ((base == 10) && (result > UINT_MAX))
        return false;
    value = static_cast<uint32_t>(result);

    return true;
}

bool Core::Deserialize(const std::string & text, int64_t & value, int base /*= 10*/)
{
    value = {};
    if (!HasValidCharactersForBase(text, base, true))
        return false;

    // For some strange reason stroull behaves incorrectly in some cases.
    long long result = OSAL::Strings::strtoll(text.c_str(), nullptr, base);
    unsigned long long result_ull = OSAL::Strings::strtoull(text.c_str(), nullptr, base);
    if (result < LLONG_MIN)
        return false;
    if ((base == 10) && (result >= 0) && (result_ull > LLONG_MAX))
        return false;
    if ((base != 10) && (result_ull > LLONG_MAX))
        value = static_cast<int64_t>(result_ull);
    else
        value = result;

    return true;
}

bool Core::Deserialize(const std::string & text, uint64_t & value, int base /*= 10*/)
{
    value = {};
    if (!HasValidCharactersForBase(text, base, false))
        return false;

    unsigned long long result = OSAL::Strings::strtoull(text.c_str(), nullptr, base);
    value = static_cast<uint64_t>(result);

    return true;
}

bool Core::Deserialize(const std::string & text, float & value)
{
    value = {};
    if (!HasValidCharactersFloatingPoint(text))
        return false;

    long double tmp;
    std::basic_istringstream<char> stream(text);
    stream >> tmp;
    if ((std::abs(tmp) < FLT_MIN) || (std::abs(tmp) > FLT_MAX))
        return false;

    value = static_cast<float>(tmp);
    return true;
}

bool Core::Deserialize(const std::string & text, double & value)
{
    value = {};
    if (!HasValidCharactersFloatingPoint(text))
        return false;

    long double tmp;
    std::basic_istringstream<char> stream(text);
    stream >> tmp;
    if ((std::abs(tmp) < DBL_MIN) || (std::abs(tmp) > DBL_MAX))
        return false;

    value = static_cast<double>(tmp);
    return true;
}

bool Core::Deserialize(const std::string & text, long double & value)
{
    value = {};
    if (!HasValidCharactersFloatingPoint(text))
        return false;

    std::basic_istringstream<char> stream(text);
    stream >> value;

    return true;
}

#if defined(LINUX) || defined(APPLE)
bool Core::Deserialize(const std::string & text, OSAL::Network::DomainSocketAddress & value)
{
    return OSAL::Network::DomainSocketAddress::TryParse(text, value);
}
#endif

bool Core::Deserialize(const std::string & text, OSAL::Network::IPV4Address & value)
{
    return OSAL::Network::IPV4Address::TryParse(text, value);
}

bool Core::Deserialize(const std::string & text, OSAL::Network::IPV6Address & value)
{
    return OSAL::Network::IPV6Address::TryParse(text, value);
}

bool Core::Deserialize(const std::string & text, OSAL::Network::MACAddress & value)
{
    return OSAL::Network::MACAddress::TryParse(text, value);
}

// TODO: Implement bytearray deserialization