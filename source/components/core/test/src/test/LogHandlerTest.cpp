#include <unittest-c++/UnitTestC++.h>
#include "core/LogHandler.h"

using namespace std;

namespace Core {
namespace Test {

TEST_SUITE(core) {

const OSAL::String DomainName = _("Domain");

class LogHandlerTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();

    static void MyHandlerDefault(const OSAL::String & domainName,
                                 const OSAL::String & componentName,
                                 LogLevel logLevel,
                                 const OSAL::String & message,
                                 void * userData)
    {
        LogHandlerTest * pThis = reinterpret_cast<LogHandlerTest *>(userData);
        pThis->_domainNameDefault = domainName;
        pThis->_componentNameDefault = componentName;
        pThis->_logLevelDefault = logLevel;
        pThis->_messageDefault = message;
    }
    static void MyHandler(const OSAL::String & domainName,
                          const OSAL::String & componentName,
                          LogLevel logLevel,
                          const OSAL::String & message,
                          void * userData)
    {
        LogHandlerTest * pThis = reinterpret_cast<LogHandlerTest *>(userData);
        pThis->_domainName = domainName;
        pThis->_componentName = componentName;
        pThis->_logLevel = logLevel;
        pThis->_message = message;
    }
    OSAL::String _domainNameDefault {};
    OSAL::String _componentNameDefault {};
    LogLevel _logLevelDefault {};
    OSAL::String _messageDefault {};
    OSAL::String _domainName {};
    OSAL::String _componentName {};
    LogLevel _logLevel {};
    OSAL::String _message {};
    LogHandlerInfo _savedHandler ;

    void ExpectDefault(const OSAL::String & domainName,
                       const OSAL::String & componentName,
                       LogLevel logLevel,
                       const OSAL::String & message)
    {
        EXPECT_EQ(domainName, this->_domainNameDefault);
        EXPECT_EQ(componentName, this->_componentNameDefault);
        EXPECT_EQ(logLevel, this->_logLevelDefault);
        EXPECT_EQ(message, this->_messageDefault);
    }
    void ExpectDomain(const OSAL::String & domainName,
                      const OSAL::String & componentName,
                      LogLevel logLevel,
                      const OSAL::String & message)
    {
        EXPECT_EQ(domainName, this->_domainName);
        EXPECT_EQ(componentName, this->_componentName);
        EXPECT_EQ(logLevel, this->_logLevel);
        EXPECT_EQ(message, this->_message);
    }
};

void LogHandlerTest::SetUp()
{
    _savedHandler = LogHandler::Set(LogHandler::DefaultDomain, LogLevel::All, MyHandlerDefault, this);
}

void LogHandlerTest::TearDown()
{
    LogHandler::Set(LogHandler::DefaultDomain, _savedHandler);
    LogHandler::Reset(DomainName);
}

TEST_FIXTURE(LogHandlerTest, DefaultHandler)
{
    LogHandler::Log(LogHandler::DefaultDomain, _("LogHandlerTest"), LogLevel::Error, _("Should not be shown on console"));
}

TEST_FIXTURE(LogHandlerTest, NonExistingDomainHandler)
{
    LogHandler::Log(_("Dummy"), _("LogHandlerTest"), LogLevel::Error, _("Should not be shown on console"));
}

TEST_FIXTURE(LogHandlerTest, ResetDefaultDomainHandler)
{
    LogHandler::Reset(LogHandler::DefaultDomain);
    LogHandler::Log(LogHandler::DefaultDomain, _("LogHandlerTest"), LogLevel::Error, _("Should be shown on cout"));
}

TEST_FIXTURE(LogHandlerTest, ResetNonExistingDomainHandler)
{
    LogHandler::Reset(_("Dummy"));
    LogHandler::Log(_("Dummy"), _("LogHandlerTest"), LogLevel::Error, _("Should not be shown on cout"));
    LogHandler::Reset(LogHandler::DefaultDomain);
    LogHandler::Log(_("Dummy"), _("LogHandlerTest"), LogLevel::Error, _("Should be shown on cout"));
}

TEST_FIXTURE(LogHandlerTest, SetDefaultHandler)
{
    OSAL::String aComponentName = _("LogHandlerTest");
    LogLevel aLogLevel = LogLevel::Error;
    OSAL::String aMessage = _("Message");

    LogHandler::Set(LogHandler::DefaultDomain, LogLevel::All, MyHandlerDefault, this);
    LogHandler::Log(DomainName, aComponentName, aLogLevel, aMessage);
    ExpectDefault(DomainName, aComponentName, aLogLevel, aMessage);
    ExpectDomain(_(""), _(""), LogLevel::None, _(""));

    LogHandler::Log(LogHandler::DefaultDomain, aComponentName, aLogLevel, aMessage);
    ExpectDefault(LogHandler::DefaultDomain, aComponentName, aLogLevel, aMessage);
    ExpectDomain(_(""), _(""), LogLevel::None, _(""));
}

TEST_FIXTURE(LogHandlerTest, SetDomainHandler)
{
    OSAL::String aComponentName = _("LogHandlerTest");
    LogLevel aLogLevel = LogLevel::Error;
    OSAL::String aMessage = _("Message");

    LogHandler::Set(DomainName, LogLevel::All, MyHandler, this);
    LogHandler::Log(DomainName, aComponentName, aLogLevel, aMessage);
    ExpectDefault(_(""), _(""), LogLevel::None, _(""));
    ExpectDomain(DomainName, aComponentName, aLogLevel, aMessage);

    LogHandler::Log(LogHandler::DefaultDomain, aComponentName, aLogLevel, aMessage);
    ExpectDefault(LogHandler::DefaultDomain, aComponentName, aLogLevel, aMessage);
    ExpectDomain(DomainName, aComponentName, aLogLevel, aMessage);
}

TEST_FIXTURE(LogHandlerTest, SetDefaultHandlerInfo)
{
    OSAL::String aComponentName = _("LogHandlerTest");
    LogLevel aLogLevel = LogLevel::Error;
    OSAL::String aMessage = _("Message");

    LogHandlerInfo info(LogLevel::All, MyHandlerDefault, this);
    LogHandler::Set(LogHandler::DefaultDomain, info);
    LogHandler::Log(DomainName, aComponentName, aLogLevel, aMessage);
    ExpectDefault(DomainName, aComponentName, aLogLevel, aMessage);
    ExpectDomain(_(""), _(""), LogLevel::None, _(""));

    LogHandler::Log(LogHandler::DefaultDomain, aComponentName, aLogLevel, aMessage);
    ExpectDefault(LogHandler::DefaultDomain, aComponentName, aLogLevel, aMessage);
    ExpectDomain(_(""), _(""), LogLevel::None, _(""));
}

TEST_FIXTURE(LogHandlerTest, SetDomainHandlerInfo)
{
    OSAL::String aComponentName = _("LogHandlerTest");
    LogLevel aLogLevel = LogLevel::Error;
    OSAL::String aMessage = _("Message");

    LogHandlerInfo info(LogLevel::All, MyHandler, this);
    LogHandler::Set(DomainName, info);
    LogHandler::Log(DomainName, aComponentName, aLogLevel, aMessage);
    ExpectDefault(_(""), _(""), LogLevel::None, _(""));
    ExpectDomain(DomainName, aComponentName, aLogLevel, aMessage);

    LogHandler::Log(LogHandler::DefaultDomain, aComponentName, aLogLevel, aMessage);
    ExpectDefault(LogHandler::DefaultDomain, aComponentName, aLogLevel, aMessage);
    ExpectDomain(DomainName, aComponentName, aLogLevel, aMessage);
}

TEST_FIXTURE(LogHandlerTest, SetDomainHandler2)
{
    OSAL::String DomainName2 = _("Domain2");
    OSAL::String aComponentName = _("LogHandlerTest");
    LogLevel aLogLevel = LogLevel::Error;
    OSAL::String aMessage1 = _("Message1");
    OSAL::String aMessage2 = _("Message2");

    LogHandler::Set(DomainName, LogLevel::All, MyHandler, this);
    LogHandler::Log(DomainName, aComponentName, aLogLevel, aMessage1);
    LogHandler::Log(DomainName2, aComponentName, aLogLevel, aMessage2);
    ExpectDefault(DomainName2, aComponentName, aLogLevel, aMessage2);
    ExpectDomain(DomainName, aComponentName, aLogLevel, aMessage1);

    LogHandler::Set(DomainName2, LogLevel::All, MyHandler, this);
    LogHandler::Log(DomainName, aComponentName, aLogLevel, aMessage1);
    ExpectDomain(DomainName, aComponentName, aLogLevel, aMessage1);
    LogHandler::Log(DomainName2, aComponentName, aLogLevel, aMessage2);
    ExpectDomain(DomainName2, aComponentName, aLogLevel, aMessage2);
}

TEST_FIXTURE(LogHandlerTest, ResetDomainHandler)
{
    OSAL::String aComponentName = _("LogHandlerTest");
    LogLevel aLogLevel = LogLevel::Error;
    OSAL::String aMessage1 = _("Message1");
    OSAL::String aMessage2 = _("Message2");

    LogHandler::Set(DomainName, LogLevel::All, MyHandler, this);
    LogHandler::Log(DomainName, aComponentName, aLogLevel, aMessage1);
    ExpectDefault(_(""), _(""), LogLevel::None, _(""));
    ExpectDomain(DomainName, aComponentName, aLogLevel, aMessage1);

    LogHandler::Reset(DomainName);
    LogHandler::Log(DomainName, aComponentName, aLogLevel, aMessage2);
    ExpectDefault(DomainName, aComponentName, aLogLevel, aMessage2);
    ExpectDomain(DomainName, aComponentName, aLogLevel, aMessage1);
}

TEST_FIXTURE(LogHandlerTest, GetSetLogLevelFilterDefault)
{
    LogHandler::Set(LogHandler::DefaultDomain, LogLevel::Error, MyHandlerDefault, this);
    EXPECT_EQ(LogLevel::Error, LogHandler::GetLogLevelFilter(LogHandler::DefaultDomain));

    LogHandler::SetLogLevelFilter(LogHandler::DefaultDomain, LogLevel::Warning);
    EXPECT_EQ(LogLevel::Warning, LogHandler::GetLogLevelFilter(LogHandler::DefaultDomain));
}

TEST_FIXTURE(LogHandlerTest, GetSetLogLevelFilterDomain)
{
    LogHandler::Set(DomainName, LogLevel::Error, MyHandler, this);
    EXPECT_EQ(LogLevel::Error, LogHandler::GetLogLevelFilter(DomainName));

    LogHandler::SetLogLevelFilter(DomainName, LogLevel::Warning);
    EXPECT_EQ(LogLevel::Warning, LogHandler::GetLogLevelFilter(DomainName));
}

TEST_FIXTURE(LogHandlerTest, SetLogLevelFiltered)
{
    OSAL::String aComponentName = _("LogHandlerTest");
    LogLevel aLogLevel = LogLevel::Warning;
    OSAL::String aMessage = _("Message");

    LogHandler::Set(DomainName, LogLevel::Error, MyHandler, this);
    LogHandler::Log(DomainName, aComponentName, aLogLevel, aMessage);
    ExpectDefault(_(""), _(""), LogLevel::None, _(""));
    ExpectDomain(_(""), _(""), LogLevel::None, _(""));

    LogHandler::SetLogLevelFilter(DomainName, LogLevel::Warning);
    LogHandler::Log(DomainName, aComponentName, aLogLevel, aMessage);
    ExpectDefault(_(""), _(""), LogLevel::None, _(""));
    ExpectDomain(DomainName, aComponentName, aLogLevel, aMessage);
}

} // TEST_SUITE(core)

} // namespace Test

} // namespace Core
