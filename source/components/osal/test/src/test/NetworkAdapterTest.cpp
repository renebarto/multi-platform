#include "unit-test-c++/UnitTestC++.h"
#include <osal/NetworkAdapter.h>
#include <osal/IPV4Address.h>

using namespace std;

namespace OSAL {
namespace Network {
namespace Test
{

class NetworkAdapterTest
    : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void NetworkAdapterTest::SetUp()
{

}

void NetworkAdapterTest::TearDown()
{

}

TEST_SUITE(osal)
{

TEST_FIXTURE(NetworkAdapterTest, Constructor)
{
    OSAL::String name = _("eth0");
    AddressPtr localAddress = std::make_shared<IPV4Address>(IPV4Address::Parse(_("10.1.1.1")));
    AddressPtr netmask = std::make_shared<IPV4Address>(IPV4Address::Parse(_("255.255.255.0")));
    AddressPtr broadcastAddress = std::make_shared<IPV4Address>(IPV4Address::Parse(_("10.1.1.255")));
    AddressPtr destAddress = std::make_shared<IPV4Address>(IPV4Address::Parse(_("127.0.0.1")));
    AdapterFlags flags = AdapterFlags::InterfaceUp;
    OSAL::Network::Adapter target(name, localAddress, netmask,
                                  broadcastAddress, destAddress, flags);
    EXPECT_EQ(name, target.Name());
    EXPECT_EQ(localAddress, target.LocalAddress());
    EXPECT_EQ(netmask, target.NetMask());
    EXPECT_EQ(broadcastAddress, target.BroadcastAddress());
    EXPECT_EQ(destAddress, target.DestAddress());
    EXPECT_EQ(flags, target.Flags());
}

} // TEST_SUITE(osal)
} // namespace Test
} // namespace Network
} // namespace OSAL
