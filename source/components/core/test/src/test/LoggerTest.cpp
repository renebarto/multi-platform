#include "unit-test-c++/UnitTestC++.h"
#include "core/Logger.h"
#include "core/ILogListener.h"

using namespace std;
using namespace Core;

namespace Core {
namespace Test {

TEST_SUITE(core) {

static const OSAL::String DomainName = _("Domain");
static const LogLevel LogLevelFilterAll = LogLevel::All;
static const LogLevel LogLevelFilterError = LogLevel::Error;
static const LogLevel LogLevelFilterWarning = LogLevel::Warning;
static const OSAL::String ComponentName = _("LoggerTest");
static const OSAL::String Message = _("Message");
static const char * Exception = "Exception";

class DummyException : public exception
{
public:
    const char* what() const noexcept
    {
        return Exception;
    }
};

class LoggerListener : public ILogListener
{
public:
    LoggerListener()
    {
    }

    void Log(const DateTime & timestamp,
             const OSAL::String & domainName,
             const OSAL::String & componentName,
             LogLevel logLevel,
             const OSAL::String & message)
    {
        ++this->_logCount;
        this->_timestamp = timestamp;
        this->_domainName = domainName;
        this->_componentName = componentName;
        this->_logLevel = logLevel;
        this->_message = message;
    }

    int _logCount {};
    DateTime _timestamp {};
    OSAL::String _domainName {};
    OSAL::String _componentName {};
    LogLevel _logLevel {};
    OSAL::String _message {};
};

class LoggerTest : public UnitTestCpp::TestFixture
{
public:
    LoggerTest()
        : logger(DomainName, LogLevelFilterAll)
    {
    }

    virtual void SetUp();
    virtual void TearDown();

    Logger logger;
    LoggerListener listener;
};

void LoggerTest::SetUp()
{
}

void LoggerTest::TearDown()
{
    logger.RemoveObserver(&listener);
}

TEST_FIXTURE(LoggerTest, Construct)
{
    EXPECT_EQ(LogLevelFilterAll | LogLevel::Fatal | LogLevel::Recursion, logger.GetLogLevelFilter());
}

TEST_FIXTURE(LoggerTest, GetSetLogLevelFilter)
{
    logger.SetLogLevelFilter(LogLevelFilterError);
    EXPECT_EQ(LogLevelFilterError, logger.GetLogLevelFilter());
}

TEST_FIXTURE(LoggerTest, ErrorNoListener)
{
    logger.Error(ComponentName, Message);
    EXPECT_EQ(0, listener._logCount);
    EXPECT_EQ(OSAL::String(), listener._domainName);
    EXPECT_EQ(OSAL::String(), listener._componentName);
    EXPECT_EQ(LogLevel::None, listener._logLevel);
    EXPECT_EQ(OSAL::String(), listener._message);
}

TEST_FIXTURE(LoggerTest, ErrorFiltered)
{
    logger.AddObserver(&listener);
    logger.SetLogLevelFilter(LogLevelFilterWarning);
    logger.Error(ComponentName, Message);
    EXPECT_EQ(0, listener._logCount);
    EXPECT_EQ(OSAL::String(), listener._domainName);
    EXPECT_EQ(OSAL::String(), listener._componentName);
    EXPECT_EQ(LogLevel::None, listener._logLevel);
    EXPECT_EQ(OSAL::String(), listener._message);
}

TEST_FIXTURE(LoggerTest, Error)
{
    logger.AddObserver(&listener);
    logger.Error(ComponentName, Message);
    EXPECT_EQ(1, listener._logCount);
    EXPECT_EQ(DomainName, listener._domainName);
    EXPECT_EQ(ComponentName, listener._componentName);
    EXPECT_EQ(LogLevel::Error, listener._logLevel);
    EXPECT_EQ(Message, listener._message);
}

TEST_FIXTURE(LoggerTest, ErrorException)
{
    logger.AddObserver(&listener);
    logger.Error(ComponentName, Message, DummyException());
    EXPECT_EQ(1, listener._logCount);
    EXPECT_EQ(DomainName, listener._domainName);
    EXPECT_EQ(ComponentName, listener._componentName);
    EXPECT_EQ(LogLevel::Error, listener._logLevel);
    EXPECT_EQ(Message + _(" (") + OSAL::ToString(Exception) + _(")"), listener._message);
}

TEST_FIXTURE(LoggerTest, Warn)
{
    logger.AddObserver(&listener);
    logger.Warn(ComponentName, Message);
    EXPECT_EQ(1, listener._logCount);
    EXPECT_EQ(DomainName, listener._domainName);
    EXPECT_EQ(ComponentName, listener._componentName);
    EXPECT_EQ(LogLevel::Warning, listener._logLevel);
    EXPECT_EQ(Message, listener._message);
}

TEST_FIXTURE(LoggerTest, WarnException)
{
    logger.AddObserver(&listener);
    logger.Warn(ComponentName, Message, DummyException());
    EXPECT_EQ(1, listener._logCount);
    EXPECT_EQ(DomainName, listener._domainName);
    EXPECT_EQ(ComponentName, listener._componentName);
    EXPECT_EQ(LogLevel::Warning, listener._logLevel);
    EXPECT_EQ(Message + _(" (") + OSAL::ToString(Exception) + _(")"), listener._message);
}

TEST_FIXTURE(LoggerTest, Message)
{
    logger.AddObserver(&listener);
    logger.Message(ComponentName, Message);
    EXPECT_EQ(1, listener._logCount);
    EXPECT_EQ(DomainName, listener._domainName);
    EXPECT_EQ(ComponentName, listener._componentName);
    EXPECT_EQ(LogLevel::Message, listener._logLevel);
    EXPECT_EQ(Message, listener._message);
}

TEST_FIXTURE(LoggerTest, Info)
{
    logger.AddObserver(&listener);
    logger.Info(ComponentName, Message);
    EXPECT_EQ(1, listener._logCount);
    EXPECT_EQ(DomainName, listener._domainName);
    EXPECT_EQ(ComponentName, listener._componentName);
    EXPECT_EQ(LogLevel::Info, listener._logLevel);
    EXPECT_EQ(Message, listener._message);
}

TEST_FIXTURE(LoggerTest, Debug)
{
    logger.AddObserver(&listener);
    logger.Debug(ComponentName, Message);
    EXPECT_EQ(1, listener._logCount);
    EXPECT_EQ(DomainName, listener._domainName);
    EXPECT_EQ(ComponentName, listener._componentName);
    EXPECT_EQ(LogLevel::Debug, listener._logLevel);
    EXPECT_EQ(Message, listener._message);
}

} // TEST_SUITE(core)

} // namespace Test

} // namespace Core
