#include "unit-test-c++/UnitTestC++.h"

#include "osal/NetworkAddress.h"

using namespace std;

namespace OSAL
{
namespace Network
{
namespace Test
{

class NetworkAddressTest
    : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void NetworkAddressTest::SetUp()
{
}

void NetworkAddressTest::TearDown()
{
}

TEST_FIXTURE(NetworkAddressTest, StreamOperatorSocketFamily)
{
    OSAL::String expected = _("Unknown");
    std::basic_ostringstream<OSAL::Char> stream;
    stream << SocketFamily::Any;
    OSAL::String actual = stream.str();

    EXPECT_EQ(expected, actual);

    expected = _("IPV4");
    stream.str(_(""));
    stream << SocketFamily::Internet;
    actual = stream.str();

    EXPECT_EQ(expected, actual);

    expected = _("IPV4");
    stream.str(_(""));
    stream << SocketFamily::InternetV4;
    actual = stream.str();

    EXPECT_EQ(expected, actual);

    expected = _("IPV6");
    stream.str(_(""));
    stream << SocketFamily::InternetV6;
    actual = stream.str();

    EXPECT_EQ(expected, actual);

    expected = _("IPX");
    stream.str(_(""));
    stream << SocketFamily::IPX;
    actual = stream.str();

    EXPECT_EQ(expected, actual);

    expected = _("Packet");
    stream.str(_(""));
    stream << SocketFamily::Packet;
    actual = stream.str();

    EXPECT_EQ(expected, actual);

#if defined(LINUX)
    expected = _("UNIX");
    stream.str(_(""));
    stream << SocketFamily::Unix;
    actual = stream.str();

    EXPECT_EQ(expected, actual);
#endif
}

} // namespace Test
} // namespace Network
} // namespace OSAL
