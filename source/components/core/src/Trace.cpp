#include <iomanip>
#include <osal/osal.h>
#include "core/Trace.h"
#include "core/serialization/EnumSerialization.h"

using namespace std;

WARNING_PUSH
WARNING_DISABLE(4592)

template<>
const std::vector<Core::EnumConversion<Core::TraceClass>> Core::EnumSerializationInfo<Core::TraceClass>::Info =
{
    {Core::TraceClass::FunctionEnter, "Enter"},
    {Core::TraceClass::FunctionLeave, "Leave"},
    {Core::TraceClass::Debug,         "Debug"},
    {Core::TraceClass::Info,          "Info "},
    {Core::TraceClass::Warning,       "Warn "},
    {Core::TraceClass::Error,         "Error"},
};

WARNING_POP

std::ostream & Core::operator << (std::ostream & stream, const TraceClass & value)
{
    stream << Serialize(value);
    return stream;
}

namespace Core {

std::ostream * TraceHandler::_stream = nullptr;
DateTime TraceHandler::_lastTimestamp;
TraceHandler TraceHandler::_trace;

TraceHandler::TraceHandler()
{
}

void TraceHandler::SetTraceTarget(std::ostream * stream)
{
    _stream = stream;
}

bool TraceHandler::DoOutput(TraceClass UNUSED(traceClass)) const
{
    return true;
}

void TraceHandler::Send(TraceClass traceClass, const std::string message)
{
    if (_stream != nullptr)
    {
        _lastTimestamp = DateTime::NowUTC();
        (*_stream) << _lastTimestamp << "|" << traceClass << "|" << message << std::endl;
    }
}

void InitializeTrace(std::ostream *stream)
{
    TraceHandler::Instance().SetTraceTarget(stream);
}
void DeinitializeTrace()
{
    TraceHandler::Instance().SetTraceTarget(nullptr);
}
TraceHandler & GetTraceHandler()
{
    return TraceHandler::Instance();
}

void TraceError(const std::string & text)
{
    TraceHandler::Instance().Send(TraceClass::Error, text);
}

void TraceWarning(const std::string & text)
{
    TraceHandler::Instance().Send(TraceClass::Warning, text);
}

void TraceInfo(const std::string & text)
{
    TraceHandler::Instance().Send(TraceClass::Info, text);
}

void TraceDebug(const std::string & text)
{
    TraceHandler::Instance().Send(TraceClass::Debug, text);
}

void TraceFunctionEnter(const std::string & text)
{
    TraceHandler::Instance().Send(TraceClass::FunctionEnter, text);
}

void TraceFunctionLeave(const std::string & text)
{
    TraceHandler::Instance().Send(TraceClass::FunctionLeave, text);
}

} // namespace Core