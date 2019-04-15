#include <unittest-cpp/UnitTestC++.h>

#include <fstream>
#include "osal/Console.h"
#include "core/Trace.h"
#include <unittest-cpp/ConsoleGoogleTestReporter.h>
#include "CommandLineOptionsParser.h"

using namespace std;

static const std::string moduleName = "json";

int main(int argc, char * argv[])
{
    OSAL::Console console;

    console << fgcolor(OSAL::ConsoleColor::Magenta | OSAL::ConsoleColor::Intensity);
    console << "Running tests for: " << moduleName << std::endl;
    console << fgcolor(OSAL::ConsoleColor::Default);
    Core::InitializeTrace(console.GetStream());

    CommandLineOptionsParser parser(console);
    std::string applicationName = argv[0];

    if (!parser.Parse(argc, argv))
    {
        exit(1);
    }

    console << "Application           : " << applicationName << std::endl;
    console << "XML output            : " << parser.XmlOutput() << std::endl;
    console << "Google Test emulation : " << (parser.GoogleTestEmulation() ? "Yes" : "No") << std::endl;
    console << "Test filter           : " << parser.Filter() << std::endl;
    console << "Test lists            : " << (parser.List() ? "Yes" : "No") << std::endl;
    console << "Test color            : " << (parser.Color() ? "Yes" : "No") << std::endl;
    console << "Test data             : " << TEST_DATA_ROOT << std::endl;

    UnitTestCpp::True nonFiltering;
    UnitTestCpp::InSelectionFilter filtering(parser.Filter());
    UnitTestCpp::Selector * filter = &nonFiltering;
    if (!parser.Filter().empty())
        filter = &filtering;
    if (!parser.XmlOutput().empty())
    {
        std::basic_ofstream<char> outputFile;

        outputFile.open(parser.XmlOutput());
        UnitTestCpp::XMLTestReporter reporter(outputFile);
        return RunSelectedTests(reporter, *filter);
    }

    UnitTestCpp::ConsoleGoogleTestReporter reporterGoogleEmulation(parser.Color());
    UnitTestCpp::ConsoleTestReporter reporter(parser.Color());

    UnitTestCpp::ITestReporter * reporterInstance = &reporter;
    if (parser.GoogleTestEmulation())
    {
        reporterInstance = &reporterGoogleEmulation;
    }

    return RunSelectedTests(*reporterInstance, *filter);
}