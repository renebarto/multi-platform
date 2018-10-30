#pragma once

namespace OSAL
{

class SystemErrorWindows : public Exception
{
protected:
    DWORD errorCode;

public:
    SystemErrorWindows(char const * functionName, char const * fileName, int line, DWORD errorCode)
        : Exception(functionName, fileName, line)
        , errorCode(errorCode)
    {
    }
    SystemErrorWindows(char const * functionName,
                       char const * fileName,
                       int line,
                       DWORD errorCode,
                       std::string message)
        : Exception(functionName, fileName, line, message)
        , errorCode(errorCode)
    {
    }
    ~SystemErrorWindows()
    {
    }
    DWORD GetErrorCode() const
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

inline void ThrowOnError(const char * functionName, const char * fileName, int line, DWORD errorCode)
{
    if (errorCode != 0)
    {
        throw SystemErrorWindows(functionName, fileName, line, errorCode);
    }
}

} // namespace OSAL
