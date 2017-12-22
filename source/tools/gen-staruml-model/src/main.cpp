#include <iomanip>
#include <fstream>
#include <iostream>

#include <osal/Console.h>
#include <core/ConsoleLogger.h>
#include <core/DefaultLogger.h>
#include <parse-c++/Parser.h>
#include <staruml/Document.h>
#include "CommandLineOptionsParser.h"
#include "ModelGenerator.h"

using namespace std;
using namespace StarUML;

#if defined(UNICODE) || defined(_UNICODE)
int wmain(int argc, const wchar_t * argv[])
#else
int main(int UNUSED(argc), const char * UNUSED(argv[]))
#endif
{
    OSAL::Console console;
    Core::ConsoleLogger logger(Core::TheLogger(), console);
    OSAL::String applicationName = argv[0];

    console << fgcolor(OSAL::ConsoleColor::Magenta | OSAL::ConsoleColor::Intensity);
    console << applicationName << ": StarUML source model generator" << std::endl;
    console << fgcolor(OSAL::ConsoleColor::Default);

    CommandLineOptionsParser commandLineParser(console);

    if (!commandLineParser.Parse(argc, argv))
    {
        exit(EXIT_FAILURE);
    }
    if (commandLineParser.sourceFile.empty())
    {
        console << fgcolor(OSAL::ConsoleColor::Red) << "No source file specified" << fgcolor(OSAL::ConsoleColor::Default) << endl;
        commandLineParser.GetHelp(applicationName, true);
        exit(EXIT_FAILURE);
    }
    std::ostream * outputFile = &cout;
    std::ofstream modelFile;
    if (!commandLineParser.modelFile.empty())
    {
        modelFile.open(commandLineParser.modelFile);
        outputFile = &modelFile;
    }

    console << _("Source file: ") << commandLineParser.sourceFile << std::endl;
    console << _("Model file : ") << (commandLineParser.modelFile.empty() ? "stdout" : commandLineParser.modelFile) << std::endl;

    CPPParser::Parser parser(commandLineParser.sourceFile);
    CPPParser::OptionsList options = {
        "-x", "c++",
        "-Isource/components/osal/export",
        "-Isource/components/osal/include"
    };
    parser.Parse(options);
    Document document;
    document.SetupDefaultProject();
    ModelGenerator generator(console);
    generator.Run(parser, document);

    document.Serialize(*outputFile);

    return EXIT_SUCCESS;
}
