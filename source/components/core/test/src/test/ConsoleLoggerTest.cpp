#include "core/ConsoleLogger.h"
#include "core/DefaultLogger.h"
#include "unit-test-c++/UnitTestC++.h"

using namespace std;

namespace Core
{
namespace Test
{

TEST_SUITE(core)
{
    static const string DomainName              = "Domain";
    static const LogLevel LogLevelFilterAll     = LogLevel::All;
    static const LogLevel LogLevelFilterError   = LogLevel::Error;
    static const LogLevel LogLevelFilterWarning = LogLevel::Warning;
    static const string ComponentName           = "ConsoleLoggerTest";
    static const string Error                   = "Error";
    static const string Warning                 = "Warning";
    static const string Message                 = "Message";
    static const char * Exception               = "Exception";

    class DummyException : public exception
    {
    public:
        const char * what() const noexcept
        {
            return Exception;
        }
    };

    class ConsoleLoggerTest : public UnitTestCpp::TestFixture
    {
    public:
        ConsoleLoggerTest() : _console(), _logger(TheLogger(), _console)
        {
        }

        virtual void SetUp();
        virtual void TearDown();

        OSAL::Console _console;
        ConsoleLogger _logger;
    };

    void ConsoleLoggerTest::SetUp()
    {
    }

    void ConsoleLoggerTest::TearDown()
    {
    }

    TEST_FIXTURE(ConsoleLoggerTest, Error)
    {
        TheLogger().Error(ComponentName, Error);
    }

    TEST_FIXTURE(ConsoleLoggerTest, ErrorException)
    {
        TheLogger().Error(ComponentName, Error, DummyException());
    }

    TEST_FIXTURE(ConsoleLoggerTest, Warn)
    {
        TheLogger().Warn(ComponentName, Warning);
    }

    TEST_FIXTURE(ConsoleLoggerTest, WarnException)
    {
        TheLogger().Warn(ComponentName, Warning, DummyException());
    }

    TEST_FIXTURE(ConsoleLoggerTest, Message)
    {
        TheLogger().Message(ComponentName, Message);
    }

    TEST_FIXTURE(ConsoleLoggerTest, Info)
    {
        TheLogger().Info(ComponentName, Message);
    }

    TEST_FIXTURE(ConsoleLoggerTest, Debug)
    {
        TheLogger().Debug(ComponentName, Message);
    }

} // TEST_SUITE(core)
} // namespace Test

} // namespace Core
