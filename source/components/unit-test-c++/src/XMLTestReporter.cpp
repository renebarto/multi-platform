#include <unit-test-c++/XMLTestReporter.h>

#include <sstream>
#include <unit-test-c++/TestFailure.h>
#include <unit-test-c++/TestResults.h>

using namespace std;

namespace
{

void ReplaceChar(OSAL::String & str, OSAL::Char c, const OSAL::String & replacement)
{
    for (size_t pos = str.find(c); pos != OSAL::String::npos; pos = str.find(c, pos + 1))
        str.replace(pos, 1, replacement);
}

OSAL::String XmlEscape(const OSAL::String & value)
{
    OSAL::String escaped = value;

    ReplaceChar(escaped, _('&'), _("&amp;"));
    ReplaceChar(escaped, _('<'), _("&lt;"));
    ReplaceChar(escaped, _('>'), _("&gt;"));
    ReplaceChar(escaped, _('\''), _("&apos;"));
    ReplaceChar(escaped, _('\"'), _("&quot;"));

    return escaped;
}

OSAL::String BuildFailureMessage(const OSAL::String & file, int line, const OSAL::String & message)
{
    basic_ostringstream<OSAL::Char> failureMessage;
    failureMessage << file << _("(") << line << _(") : ") << message;
    return failureMessage.str();
}

}

namespace UnitTestCpp
{

XMLTestReporter::XMLTestReporter(basic_ostream<OSAL::Char> & _stream)
    : _stream(_stream)
{
}

void XMLTestReporter::ReportTestRunSummary(const TestResults * results, int milliSecondsElapsed)
{
    AddXmlElement(nullptr);

    BeginResults(results->GetTotalTestCount(), results->GetFailedTestCount(),
                 results->GetFailureCount(), milliSecondsElapsed);

    for (auto result : Results())
    {
        BeginTest(result);

        if (result.Failed())
            AddFailure(result);

        EndTest(result);
    }

    EndResults();
}

void XMLTestReporter::AddXmlElement(const OSAL::Char * encoding)
{
    _stream << _("<?xml version=\"1.0\"");

    if (encoding != nullptr)
        _stream << _(" encoding=\"") << encoding << _("\"");

    _stream << _("?>") << endl;
}

void XMLTestReporter::BeginResults(int totalTestCount, int failedTestCount,
                                   int failureCount, int milliSecondsElapsed)
{
   _stream << _("<unittest-results")
       << _(" tests=\"") << totalTestCount << _("\"")
       << _(" failedtests=\"") << failedTestCount << _("\"")
       << _(" failures=\"") << failureCount << _("\"")
       << _(" time=\"") << static_cast<double>(milliSecondsElapsed) / 1000.0 << _("\"")
       << _(">") << endl;
}

void XMLTestReporter::EndResults()
{
    _stream << _("</unittest-results>") << endl;
}

void XMLTestReporter::BeginTest(const TestDetailedResult & result)
{
    _stream << _("<test")
        << _(" suite=\"") << result.suiteName << _("\"")
        << _(" fixture=\"") << result.fixtureName << _("\"")
        << _(" name=\"") << result.testName << _("\"")
        << _(" time=\"") << static_cast<double>(result.MilliSecondsElapsed()) / 1000.0 << _("\"");
}

void XMLTestReporter::EndTest(const TestDetailedResult & result)
{
    if (result.Failed())
        _stream << _("</test>") << endl;
    else
        _stream << _("/>") << endl;
}

void XMLTestReporter::AddFailure(const TestDetailedResult & result)
{
    _stream << _(">") << endl; // close <test> element

    for (auto failure : result.Failures())
    {
        OSAL::String const escapedMessage = XmlEscape(failure.second);
        OSAL::String const message = BuildFailureMessage(result.fileName, failure.first, escapedMessage);

        _stream << _("<failure") << _(" message=\"") << message << _("\"") << _("/>") << endl;
    }
}

} // namespace UnitTestCpp
