#include <core/serialization/DeserializationImpl.h>

#include <climits>
#include <sstream>
#include <osal/Strings.h>

static bool HasValidCharactersForBase(const OSAL::String & text, int base)
{
    if (text.empty())
        return false;
    switch (base)
    {
        case 2:
            for (size_t i = 0; i < text.length(); i++)
            {
                if (OSAL::Strings::strchr(_("01"), static_cast<wchar_t>(OSAL::Strings::toupper(text[i]))) == nullptr)
                    return false;
            }
            break;
        case 8:
            for (size_t i = 0; i < text.length(); i++)
            {
                if (OSAL::Strings::strchr(_("01234567"), static_cast<wchar_t>(OSAL::Strings::toupper(text[i]))) == nullptr)
                    return false;
            }
            break;
        case 10:
            for (size_t i = 0; i < text.length(); i++)
            {
                if (OSAL::Strings::strchr(_("0123456789+-"), static_cast<wchar_t>(OSAL::Strings::toupper(text[i]))) == nullptr)
                    return false;
            }
            break;
        case 16:
            for (size_t i = 0; i < text.length(); i++)
            {
                if (OSAL::Strings::strchr(_("0123456789ABCDEF"), static_cast<wchar_t>(OSAL::Strings::toupper(text[i]))) == nullptr)
                    return false;
            }
            break;
        default:
            return false;
    }
    return true;
}

static bool HasValidCharactersFloatingPoint(const OSAL::String & text)
{
    if (text.empty())
        return false;
    for (size_t i = 0; i < text.length(); i++)
    {
        if (OSAL::Strings::strchr(_("0123456789.+-Ee"), static_cast<wchar_t>(OSAL::Strings::toupper(text[i]))) == nullptr)
            return false;
    }
    return true;
}


bool Core::Deserialize(const OSAL::String & text, bool & value)
{
    if (OSAL::IsEqualIgnoreCase(_("true"), text))
    {
        value = true;
        return true;
    }
    if (OSAL::IsEqualIgnoreCase(_("false"), text))
    {
        value = false;
        return true;
    }

    return false;
}

bool Core::Deserialize(const OSAL::String & text, int8_t & value, int base)
{
    if (!HasValidCharactersForBase(text, base))
        return false;

    long result = OSAL::Strings::strtol(text.c_str(), nullptr, base);
    if ((base == 10) && ((result < CHAR_MIN) || (result > CHAR_MAX)))
        return false;
    value = (int8_t)result;

    return true;
}

bool Core::Deserialize(const OSAL::String & text, uint8_t & value, int base /*= 10*/)
{
    if (!HasValidCharactersForBase(text, base))
        return false;

    long result = OSAL::Strings::strtol(text.c_str(), nullptr, base);
    if ((base == 10) && ((result < 0) || (result > UCHAR_MAX)))
        return false;
    value = (uint8_t)result;

    return true;
}

bool Core::Deserialize(const OSAL::String & text, int16_t & value, int base /*= 10*/)
{
    if (!HasValidCharactersForBase(text, base))
        return false;

    long result = OSAL::Strings::strtol(text.c_str(), nullptr, base);
    if ((base == 10) && ((result < SHRT_MIN) || (result > SHRT_MAX)))
        return false;
    value = (int16_t)result;

    return true;
}

bool Core::Deserialize(const OSAL::String & text, uint16_t & value, int base /*= 10*/)
{
    if (!HasValidCharactersForBase(text, base))
        return false;

    long result = OSAL::Strings::strtol(text.c_str(), nullptr, base);
    if ((base == 10) && ((result < 0) || (result > USHRT_MAX)))
        return false;
    value = (uint16_t)result;

    return true;
}

bool Core::Deserialize(const OSAL::String & text, int32_t & value, int base /*= 10*/)
{
    if (!HasValidCharactersForBase(text, base))
        return false;

    long result = OSAL::Strings::strtol(text.c_str(), nullptr, base);
    if ((base == 10) && ((result < INT_MIN) || (result > INT_MAX)))
        return false;
    value = (int32_t)result;

    return true;
}

bool Core::Deserialize(const OSAL::String & text, uint32_t & value, int base /*= 10*/)
{
    if (!HasValidCharactersForBase(text, base))
        return false;

    long result = OSAL::Strings::strtol(text.c_str(), nullptr, base);
    if ((base == 10) && ((result < 0) || (result > (long)UINT_MAX)))
        return false;
    value = (uint32_t)result;

    return true;
}

bool Core::Deserialize(const OSAL::String & text, int64_t & value, int base /*= 10*/)
{
    if (!HasValidCharactersForBase(text, base))
        return false;

    // For some strange reason stroull behaves incorrectly in some cases.
    long long result = OSAL::Strings::strtoll(text.c_str(), nullptr, base);
    unsigned long long result_ull = OSAL::Strings::strtoull(text.c_str(), nullptr, base);
    if ((base == 10) && ((result < LLONG_MIN) || (result > LLONG_MAX)))
        return false;
    if ((base == 16) && (result_ull > LLONG_MAX))
        value = static_cast<long long>(result_ull);
    else
        value = result;

    return true;
}

bool Core::Deserialize(const OSAL::String & text, uint64_t & value, int base /*= 10*/)
{
    if (!HasValidCharactersForBase(text, base))
        return false;

    unsigned long long result = OSAL::Strings::strtoull(text.c_str(), nullptr, base);
    value = (uint64_t)result;

    return true;
}

bool Core::Deserialize(const OSAL::String & text, float & value)
{
    if (!HasValidCharactersFloatingPoint(text))
        return false;

    std::basic_istringstream<OSAL::Char> stream(text);
    stream >> value;

    return true;
}

bool Core::Deserialize(const OSAL::String & text, double & value)
{
    if (!HasValidCharactersFloatingPoint(text))
        return false;

    std::basic_istringstream<OSAL::Char> stream(text);
    stream >> value;

    return true;
}

