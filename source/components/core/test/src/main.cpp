#include <unit-test-c++/UnitTestC++.h>

#include <fstream>
#include <core/ConsoleLogger.h>
#include <core/DefaultLogger.h>
#include "CommandLineOptionsParser.h"
#include "osal/OSAL.h"
#include "osal/Console.h"

static const OSAL::String moduleName = _("core");

int main(int argc, const OSAL::Char * argv[])
{
    OSAL::Console console;

    console << fgcolor(OSAL::ConsoleColor::Magenta | OSAL::ConsoleColor::Intensity);
    console << _("Running tests for: ") << moduleName << std::endl;
    console << fgcolor(OSAL::ConsoleColor::Default);
    Core::ConsoleLogger logger(Core::TheLogger(), console);

    CommandLineOptionsParser parser(console);
    OSAL::String applicationName = argv[0];

    if (!parser.Parse(argc, argv))
    {
        exit(1);
    }

    console << "Application: " << applicationName << std::endl;
    console << "XML output : " << parser.xmlOutput << std::endl;
    console << "Suite      : " << parser.testSuiteName << std::endl;
    console << "Fixture    : " << parser.testFixtureName << std::endl;
    console << "Test       : " << parser.testName << std::endl;

    int result = 0;

    if (!parser.xmlOutput.empty())
    {
        std::basic_ofstream<OSAL::Char> outputFile;

        outputFile.open(parser.xmlOutput);
        UnitTestCpp::XMLTestReporter reporter(outputFile);
        const OSAL::Char * suiteName = parser.testSuiteName.empty() ? 0 : parser.testSuiteName.c_str();
        const OSAL::Char * fixtureName = parser.testFixtureName.empty() ? 0 : parser.testFixtureName.c_str();
        const OSAL::Char * testName = parser.testName.empty() ? 0 : parser.testName.c_str();
        if ((suiteName != 0) || (fixtureName != 0) || (testName != 0))
        {
            return RunSelectedTests(reporter, UnitTestCpp::InSelection(suiteName, fixtureName, testName));
        }
        result = RunAllTests(reporter);
    }
    else
    {
        UnitTestCpp::ConsoleTestReporter reporter;
        const OSAL::Char * suiteName = parser.testSuiteName.empty() ? 0 : parser.testSuiteName.c_str();
        const OSAL::Char * fixtureName = parser.testFixtureName.empty() ? 0 : parser.testFixtureName.c_str();
        const OSAL::Char * testName = parser.testName.empty() ? 0 : parser.testName.c_str();
        if ((suiteName != 0) || (fixtureName != 0) || (testName != 0))
        {
            return RunSelectedTests(reporter, UnitTestCpp::InSelection(suiteName, fixtureName, testName));
        }
        result = RunAllTests(reporter);
    }

    return result;
}
