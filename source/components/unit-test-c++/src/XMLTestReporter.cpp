#include <unit-test-c++/XMLTestReporter.h>

#include <sstream>
#include <unit-test-c++/TestFailure.h>
#include <unit-test-c++/TestResults.h>

using namespace std;

namespace
{

void ReplaceChar(string & str, char c, const string & replacement)
{
    for (size_t pos = str.find(c); pos != string::npos; pos = str.find(c, pos + 1))
        str.replace(pos, 1, replacement);
}

string XmlEscape(const string & value)
{
    string escaped = value;

    ReplaceChar(escaped, '&', "&amp;");
    ReplaceChar(escaped, '<', "&lt;");
    ReplaceChar(escaped, '>', "&gt;");
    ReplaceChar(escaped, '\'', "&apos;");
    ReplaceChar(escaped, '\"', "&quot;");

    return escaped;
}

string BuildFailureMessage(const string & file, int line, const string & message)
{
    ostringstream failureMessage;
    failureMessage << file << "(" << line << ") : " << message;
    return failureMessage.str();
}

}

namespace UnitTestCpp
{

XMLTestReporter::XMLTestReporter(ostream & stream)
    : stream(stream)
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

void XMLTestReporter::AddXmlElement(const char * encoding)
{
    stream << "<?xml version=\"1.0\"";

    if (encoding != nullptr)
        stream << " encoding=\"" << encoding << "\"";

    stream << "?>" << endl;
}

void XMLTestReporter::BeginResults(int totalTestCount, int failedTestCount,
                                   int failureCount, int milliSecondsElapsed)
{
   stream << "<unittest-results"
       << " tests=\"" << totalTestCount << "\""
       << " failedtests=\"" << failedTestCount << "\""
       << " failures=\"" << failureCount << "\""
       << " time=\"" << milliSecondsElapsed << "\""
       << ">" << endl;
}

void XMLTestReporter::EndResults()
{
    stream << "</unittest-results>" << endl;
}

void XMLTestReporter::BeginTest(const TestDetailedResult & result)
{
    stream << "<test"
        << " suite=\"" << result.suiteName << "\""
        << " fixture=\"" << result.fixtureName << "\""
        << " name=\"" << result.testName << "\""
        << " time=\"" << result.MilliSecondsElapsed() << "\"";
}

void XMLTestReporter::EndTest(const TestDetailedResult & result)
{
    if (result.Failed())
        stream << "</test>" << endl;
    else
        stream << "/>" << endl;
}

void XMLTestReporter::AddFailure(const TestDetailedResult & result)
{
    stream << ">" << endl; // close <test> element

    for (auto failure : result.Failures())
    {
        string const escapedMessage = XmlEscape(failure.second);
        string const message = BuildFailureMessage(result.fileName, failure.first, escapedMessage);

        stream << "<failure" << " message=\"" << message << "\"" << "/>" << endl;
    }
}

} // namespace UnitTestCpp
