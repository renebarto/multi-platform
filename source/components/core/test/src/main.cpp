#include <unittest-c++/UnitTestC++.h>

#include <fstream>
#include "osal/osal.h"
#include "osal/Console.h"
//#include <core/ConsoleLogger.h>
//#include <core/DefaultLogger.h>
#include <unittest-c++/ConsoleGoogleTestReporter.h>
#include "CommandLineOptionsParser.h"

using namespace std;

static const string moduleName = "core";

int main(int argc, const char * argv[])
{
    OSAL::Console console;

    console << fgcolor(OSAL::ConsoleColor::Magenta | OSAL::ConsoleColor::Intensity);
    console << "Running tests for: " << moduleName << std::endl;
    console << fgcolor(OSAL::ConsoleColor::Default);
//    Core::ConsoleLogger logger(Core::TheLogger(), console);

    CommandLineOptionsParser parser(console);
    string applicationName = argv[0];

    if (!parser.Parse(argc, argv))
    {
        exit(1);
    }

    console << "Application : " << applicationName << std::endl;
    console << "XML output  : " << parser.xmlOutput << std::endl;
    console << "Suite       : " << parser.testSuiteName << std::endl;
    console << "Fixture     : " << parser.testFixtureName << std::endl;
    console << "Test        : " << parser.testName << std::endl;
    console << "GTest filter: " << parser.testFilter << std::endl;
    console << "GTest color : " << parser.testColor << std::endl;
    console << "Test data   : " << TEST_DATA_ROOT << std::endl;

    int result = 0;

    if (!parser.xmlOutput.empty())
    {
        std::basic_ofstream<char> outputFile;

        outputFile.open(parser.xmlOutput);
        UnitTestCpp::XMLTestReporter reporter(outputFile);
        const char * suiteName = parser.testSuiteName.empty() ? 0 : parser.testSuiteName.c_str();
        const char * fixtureName = parser.testFixtureName.empty() ? 0 : parser.testFixtureName.c_str();
        const char * testName = parser.testName.empty() ? 0 : parser.testName.c_str();
        if ((suiteName != 0) || (fixtureName != 0) || (testName != 0))
        {
            return RunSelectedTests(reporter, UnitTestCpp::InSelection(suiteName, fixtureName, testName));
        }
        result = RunAllTests(reporter);
    }
    else
    {
        UnitTestCpp::ConsoleGoogleTestReporter reporterGoogleEmulation;
        UnitTestCpp::ConsoleTestReporter reporter;

        UnitTestCpp::ITestReporter * reporterInstance = &reporter;
        if (!parser.testFilter.empty() || !parser.testColor.empty())
        {
            reporterInstance = &reporterGoogleEmulation;
        }

        const char * suiteName = parser.testSuiteName.empty() ? nullptr : parser.testSuiteName.c_str();
        const char * fixtureName = parser.testFixtureName.empty() ? nullptr : parser.testFixtureName.c_str();
        const char * testName = parser.testName.empty() ? nullptr : parser.testName.c_str();
        if ((suiteName != nullptr) || (fixtureName != nullptr) || (testName != nullptr))
        {
            return RunSelectedTests(*reporterInstance, UnitTestCpp::InSelection(suiteName, fixtureName, testName));
        }
        result = RunAllTests(*reporterInstance);
    }

    return result;
}
