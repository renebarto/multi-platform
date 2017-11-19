#include <unit-test-c++/UnitTestC++.h>

#include <fstream>
#include <core/ConsoleLogger.h>
#include <core/DefaultLogger.h>
#include "CommandLineOptionsParser.h"
#include "osal/OSAL.h"
#include "osal/Console.h"

static const OSAL::String moduleName = _("osal");

#if defined(UNICODE) || defined(_UNICODE)
int wmain(int argc, const wchar_t * argv[])
#else
int main(int argc, const char * argv[])
#endif
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

    console << _("Application: ") << applicationName << std::endl;
    console << _("XML output : ") << parser.xmlOutput << std::endl;
    console << _("Suite      : ") << parser.testSuiteName << std::endl;
    console << _("Fixture    : ") << parser.testFixtureName << std::endl;
    console << _("Test       : ") << parser.testName << std::endl;
    console << _("Test data  : ") << TEST_DATA_ROOT << std::endl;

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
        const OSAL::Char * suiteName = parser.testSuiteName.empty() ? nullptr : parser.testSuiteName.c_str();
        const OSAL::Char * fixtureName = parser.testFixtureName.empty() ? nullptr : parser.testFixtureName.c_str();
        const OSAL::Char * testName = parser.testName.empty() ? nullptr : parser.testName.c_str();
        if ((suiteName != nullptr) || (fixtureName != nullptr) || (testName != nullptr))
        {
            return RunSelectedTests(reporter, UnitTestCpp::InSelection(suiteName, fixtureName, testName));
        }
        result = RunAllTests(reporter);
    }

    return result;
}
