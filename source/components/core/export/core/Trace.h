#pragma once

#include <memory>
#include <sstream>
#include <string>
#include "core/core.h"
#include "core/DateTime.h"
#include "core/Print.h"

namespace Core {

enum class TraceClass
{
    FunctionEntry,
    FunctionLeave,
    Debug,
    Info,
    Warning,
    Error,
};

CORE_EXPORT std::ostream & operator << (std::ostream & stream, const Core::TraceClass & value);

class CORE_EXPORT TraceHandler
{
public:
    bool DoOutput(TraceClass traceClass) const;
    void Send(TraceClass traceClass, const std::string message);
    const DateTime & LastTimeStamp() const { return _lastTimestamp; }

private:
    TraceHandler();
    static void Initialize(std::ostream * stream)
    {
        _trace.SetTraceTarget(stream);
    }
    static TraceHandler & Instance()
    {
        return _trace;
    }
    void SetTraceTarget(std::ostream * stream);

    static std::ostream * _stream;
    static DateTime _lastTimestamp;
    static std::ostringstream _null;
    static TraceHandler _trace;

    friend CORE_EXPORT void InitializeTrace(std::ostream * stream);
    friend CORE_EXPORT void DeinitializeTrace();
    friend CORE_EXPORT TraceHandler & GetTraceHandler();
    friend CORE_EXPORT void TraceWarning(const std::string & text);
    friend CORE_EXPORT void TraceError(const std::string & text);
};

extern CORE_EXPORT void InitializeTrace(std::ostream * stream);
extern CORE_EXPORT void DeinitializeTrace();
extern CORE_EXPORT TraceHandler & GetTraceHandler();

void CORE_EXPORT TraceWarning(const std::string & format);
void CORE_EXPORT TraceError(const std::string & format);

template <typename ... Args>
void Trace(TraceClass traceClass, char const * const format,
           Args const & ... args) noexcept
{
    TraceHandler & trace = GetTraceHandler();
    if (trace.DoOutput(traceClass))
    {
        std::string str;
        Format(str, format, args ...);
        trace.Send(traceClass, str);
    }
}

template <typename ... Args>
void TraceFunctionBegin(char const * const format,
                        Args const & ... args) noexcept
{
    Trace(TraceClass::FunctionEntry, format, args ...);
}

template <typename ... Args>
void TraceFunctionEnd(char const * const format,
                      Args const & ... args) noexcept
{
    Trace(TraceClass::FunctionLeave, format, args ...);
}

template <typename ... Args>
void TraceDebug(char const * const format,
                Args const & ... args) noexcept
{
    Trace(TraceClass::Debug, format, args ...);
}

template <typename ... Args>
void TraceInfo(char const * const format,
               Args const & ... args) noexcept
{
    Trace(TraceClass::Info, format, args ...);
}

template <typename ... Args>
void TraceWarning(char const * const format,
                  Args const & ... args) noexcept
{
    Trace(TraceClass::Warning, format, args ...);
}

template <typename ... Args>
void TraceError(char const * const format,
                Args const & ... args) noexcept
{
    Trace(TraceClass::Error, format, args ...);
}

} // namespace Core

