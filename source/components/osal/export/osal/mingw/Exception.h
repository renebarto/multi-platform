#pragma once

namespace OSAL
{

class SystemError : public Exception
{
protected:
    int errorCode;

public:
    SystemError(char const * functionName, char const * fileName, int line, int errorCode)
        : Exception(functionName, fileName, line)
        , errorCode(errorCode)
    {
    }
    SystemError(char const * functionName,
                char const * fileName,
                int line,
                int errorCode,
                std::string message)
        : Exception(functionName, fileName, line, message)
        , errorCode(errorCode)
    {
    }
    ~SystemError()
    {
    }
    int GetErrorCode() const
    {
        return errorCode;
    }
    virtual std::string BuildMessage() const override
    {
        std::ostringstream stream;
        char buffer[1024];
        ::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, errorCode, 0, buffer, sizeof(buffer), nullptr);
        stream << Exception::BuildMessage() << " errno=" << errorCode << " (0x" << std::hex
            << std::setw(8) << std::setfill('0') << errorCode << "): \"" << buffer << "\"";
        return stream.str();
    }
};

inline void ThrowOnError(const char * functionName, const char * fileName, int line, int errorCode)
{
    if (errorCode != 0)
    {
        throw SystemError(functionName, fileName, line, errorCode);
    }
}

} // namespace OSAL
