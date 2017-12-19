#include <unittest-c++/UnitTestC++.h>
#include <osal/NetworkAdapter.h>
#include <osal/IPV4EndPoint.h>
#include <osal/IPV6EndPoint.h>

using namespace std;

namespace OSAL {
namespace Network {
namespace Test {

class NetworkAdapterTest : public UnitTestCpp::TestFixture
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

TEST_FIXTURE(NetworkAdapterTest, ConstructorIPV4)
{
    OSAL::String name = _("eth0");

    IPV4Address localAddress = IPV4Address::Parse(_("10.1.1.1"));
    IPV4Address netmask = IPV4Address::Parse(_("255.255.255.0"));
    IPV4Address broadcastAddress = IPV4Address::Parse(_("10.1.1.255"));
    IPV4Address destAddress = IPV4Address::Parse(_("127.0.0.1"));
    EndPointPtr localEndPoint = std::make_shared<IPV4EndPoint>(localAddress, 0);
    EndPointPtr netmaskEndPoint = std::make_shared<IPV4EndPoint>(netmask, 0);
    EndPointPtr broadcastEndPoint = std::make_shared<IPV4EndPoint>(broadcastAddress, 0);
    EndPointPtr destEndPoint = std::make_shared<IPV4EndPoint>(destAddress, 0);
    AdapterFlags flags = AdapterFlags::InterfaceUp;
    OSAL::Network::Adapter target(name, localEndPoint, netmaskEndPoint,
                                  broadcastEndPoint, destEndPoint, flags);
    EXPECT_EQ(name, target.Name());
    EXPECT_EQ(localEndPoint, target.LocalAddress());
    EXPECT_EQ(netmaskEndPoint, target.NetMask());
    EXPECT_EQ(broadcastEndPoint, target.BroadcastAddress());
    EXPECT_EQ(destEndPoint, target.DestAddress());
    EXPECT_EQ(flags, target.Flags());
}

TEST_FIXTURE(NetworkAdapterTest, ConstructorIPV6)
{
    OSAL::String name = _("eth0");
    IPV6Address localAddress = IPV6Address::Parse(_("fd3d:4056:8978:0:faed:b421:6da:5cf5"));
    IPV6Address netmask = IPV6Address::Parse(_("fd3d:4056:8978::0"));
    IPV6Address broadcastAddress = IPV6Address::Parse(_("ff02::1"));
    IPV6Address destAddress = IPV6Address::Parse(_("::1"));
    EndPointPtr localEndPoint = std::make_shared<IPV6EndPoint>(localAddress, 0);
    EndPointPtr netmaskEndPoint = std::make_shared<IPV6EndPoint>(netmask, 0);
    EndPointPtr broadcastEndPoint = std::make_shared<IPV6EndPoint>(broadcastAddress, 0);
    EndPointPtr destEndPoint = std::make_shared<IPV6EndPoint>(destAddress, 0);
    AdapterFlags flags = AdapterFlags::InterfaceUp;
    OSAL::Network::Adapter target(name, localEndPoint, netmaskEndPoint,
                                  broadcastEndPoint, destEndPoint, flags);
    EXPECT_EQ(name, target.Name());
    EXPECT_EQ(localEndPoint, target.LocalAddress());
    EXPECT_EQ(netmaskEndPoint, target.NetMask());
    EXPECT_EQ(broadcastEndPoint, target.BroadcastAddress());
    EXPECT_EQ(destEndPoint, target.DestAddress());
    EXPECT_EQ(flags, target.Flags());
}

} // TEST_SUITE(osal)
} // namespace Test
} // namespace Network
} // namespace OSAL
