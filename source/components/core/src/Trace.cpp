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
    {Core::TraceClass::FunctionEntry, "Entry"},
    {Core::TraceClass::FunctionLeave, "Leave"},
    {Core::TraceClass::Debug,         "Debug"},
    {Core::TraceClass::Info,          "Info"},
    {Core::TraceClass::Warning,       "Warn"},
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
std::ostringstream TraceHandler::_null;
TraceHandler TraceHandler::_trace;

TraceHandler::TraceHandler()
{
}

void TraceHandler::SetTraceTarget(std::ostream * stream)
{
    if (stream != nullptr)
        _stream = stream;
    else
        _stream = &_null;
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

void TraceWarning(const std::string & text)
{
    TraceHandler::Instance().Send(TraceClass::Warning, text);
}

void TraceError(const std::string & text)
{
    TraceHandler::Instance().Send(TraceClass::Error, text);
}

} // namespace Core