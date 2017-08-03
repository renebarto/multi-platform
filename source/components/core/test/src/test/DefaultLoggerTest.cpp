#include "unit-test-c++/UnitTestC++.h"
#include "core/DefaultLogger.h"
#include "core/ILogListener.h"

using namespace std;

namespace Core {
namespace Test {

TEST_SUITE(core) {

static const LogLevel LogLevelFilterAll = LogLevel::All;
static const LogLevel LogLevelFilterError = LogLevel::Error;
static const LogLevel LogLevelFilterWarning = LogLevel::Warning;
static const OSAL::String ComponentName = _("DefaultLoggerTest");
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

class DefaultLoggerListener : public ILogListener
{
public:
    DefaultLoggerListener()
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

class DefaultLoggerTest : public UnitTestCpp::TestFixture
{
public:
    DefaultLoggerTest()
        : _logger(LogLevelFilterAll)
    {
    }

    virtual void SetUp();
    virtual void TearDown();

    DefaultLogger _logger;
    DefaultLoggerListener _listener;
};

void DefaultLoggerTest::SetUp()
{
}

void DefaultLoggerTest::TearDown()
{
    _logger.RemoveObserver(&_listener);
}

TEST_FIXTURE(DefaultLoggerTest, Construct)
{
    EXPECT_EQ(LogLevelFilterAll | LogLevel::Fatal | LogLevel::Recursion, _logger.GetLogLevelFilter());
}

TEST_FIXTURE(DefaultLoggerTest, GetSetLogLevelFilter)
{
    _logger.SetLogLevelFilter(LogLevelFilterError);
    EXPECT_EQ(LogLevelFilterError, _logger.GetLogLevelFilter());
}

TEST_FIXTURE(DefaultLoggerTest, ErrorNoListener)
{
    _logger.Error(ComponentName, Message);
    EXPECT_EQ(0, _listener._logCount);
    EXPECT_EQ(OSAL::String(), _listener._domainName);
    EXPECT_EQ(OSAL::String(), _listener._componentName);
    EXPECT_EQ(LogLevel::None, _listener._logLevel);
    EXPECT_EQ(OSAL::String(), _listener._message);
}

TEST_FIXTURE(DefaultLoggerTest, ErrorFiltered)
{
    _logger.AddObserver(&_listener);
    _logger.SetLogLevelFilter(LogLevelFilterWarning);
    _logger.Error(ComponentName, Message);
    EXPECT_EQ(0, _listener._logCount);
    EXPECT_EQ(OSAL::String(), _listener._domainName);
    EXPECT_EQ(OSAL::String(), _listener._componentName);
    EXPECT_EQ(LogLevel::None, _listener._logLevel);
    EXPECT_EQ(OSAL::String(), _listener._message);
}

TEST_FIXTURE(DefaultLoggerTest, Error)
{
    _logger.AddObserver(&_listener);
    _logger.Error(ComponentName, Message);
    EXPECT_EQ(1, _listener._logCount);
    EXPECT_EQ(LogHandler::DefaultDomain, _listener._domainName);
    EXPECT_EQ(ComponentName, _listener._componentName);
    EXPECT_EQ(LogLevel::Error, _listener._logLevel);
    EXPECT_EQ(Message, _listener._message);
}

TEST_FIXTURE(DefaultLoggerTest, ErrorException)
{
    _logger.AddObserver(&_listener);
    _logger.Error(ComponentName, Message, DummyException());
    EXPECT_EQ(1, _listener._logCount);
    EXPECT_EQ(LogHandler::DefaultDomain, _listener._domainName);
    EXPECT_EQ(ComponentName, _listener._componentName);
    EXPECT_EQ(LogLevel::Error, _listener._logLevel);
    EXPECT_EQ(Message + _(" (") + OSAL::ToString(Exception) + _(")"), _listener._message);
}

TEST_FIXTURE(DefaultLoggerTest, Warn)
{
    _logger.AddObserver(&_listener);
    _logger.Warn(ComponentName, Message);
    EXPECT_EQ(1, _listener._logCount);
    EXPECT_EQ(LogHandler::DefaultDomain, _listener._domainName);
    EXPECT_EQ(ComponentName, _listener._componentName);
    EXPECT_EQ(LogLevel::Warning, _listener._logLevel);
    EXPECT_EQ(Message, _listener._message);
}

TEST_FIXTURE(DefaultLoggerTest, WarnException)
{
    _logger.AddObserver(&_listener);
    _logger.Warn(ComponentName, Message, DummyException());
    EXPECT_EQ(1, _listener._logCount);
    EXPECT_EQ(LogHandler::DefaultDomain, _listener._domainName);
    EXPECT_EQ(ComponentName, _listener._componentName);
    EXPECT_EQ(LogLevel::Warning, _listener._logLevel);
    EXPECT_EQ(Message + _(" (") + OSAL::ToString(Exception) + _(")"), _listener._message);
}

TEST_FIXTURE(DefaultLoggerTest, Message)
{
    _logger.AddObserver(&_listener);
    _logger.Message(ComponentName, Message);
    EXPECT_EQ(1, _listener._logCount);
    EXPECT_EQ(LogHandler::DefaultDomain, _listener._domainName);
    EXPECT_EQ(ComponentName, _listener._componentName);
    EXPECT_EQ(LogLevel::Message, _listener._logLevel);
    EXPECT_EQ(Message, _listener._message);
}

TEST_FIXTURE(DefaultLoggerTest, Info)
{
    _logger.AddObserver(&_listener);
    _logger.Info(ComponentName, Message);
    EXPECT_EQ(1, _listener._logCount);
    EXPECT_EQ(LogHandler::DefaultDomain, _listener._domainName);
    EXPECT_EQ(ComponentName, _listener._componentName);
    EXPECT_EQ(LogLevel::Info, _listener._logLevel);
    EXPECT_EQ(Message, _listener._message);
}

TEST_FIXTURE(DefaultLoggerTest, Debug)
{
    _logger.AddObserver(&_listener);
    _logger.Debug(ComponentName, Message);
    EXPECT_EQ(1, _listener._logCount);
    EXPECT_EQ(LogHandler::DefaultDomain, _listener._domainName);
    EXPECT_EQ(ComponentName, _listener._componentName);
    EXPECT_EQ(LogLevel::Debug, _listener._logLevel);
    EXPECT_EQ(Message, _listener._message);
}

} // TEST_SUITE(core)

} // namespace Test

} // namespace Core
