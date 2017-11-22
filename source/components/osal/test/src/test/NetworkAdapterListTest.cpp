#include "unit-test-c++/UnitTestC++.h"

#include <osal/NetworkAdapterList.h>
#include <osal/NetworkAdapter.h>
#include <osal/IPV4EndPoint.h>
#include <osal/IPV6EndPoint.h>
#include <osal/MACAddress.h>

using namespace std;

namespace OSAL {
namespace Network {
namespace Test
{

class NetworkAdapterListTest
    : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void NetworkAdapterListTest::SetUp()
{

}

void NetworkAdapterListTest::TearDown()
{

}

TEST_SUITE(osal)
{

TEST_FIXTURE(NetworkAdapterListTest, Constructor)
{
    OSAL::Network::AdapterList target;

    EXPECT_NE(size_t {0}, target.GetAdapters().size());

#if defined(LINUX)
    for (auto adapter : target.GetAdapters())
    {
        if (adapter->Name() == _("lo"))
        {
            ASSERT_NOT_NULL(adapter->LocalAddress());
            SocketFamily family = adapter->LocalAddress()->Family();
            if (SocketFamily::InternetV4 == family)
            {
                ASSERT_NOT_NULL(adapter->LocalAddress());
                shared_ptr<IPV4EndPoint> endPoint = dynamic_pointer_cast<IPV4EndPoint>(adapter->LocalAddress());
                ASSERT_NOT_NULL(endPoint);
                EXPECT_EQ(IPV4EndPoint::Parse(_("127.0.0.1:0")), endPoint->GetIPAddress());
                EXPECT_EQ(uint16_t{0}, endPoint->GetPort());
                ASSERT_NOT_NULL(adapter->NetMask());
                endPoint = dynamic_pointer_cast<IPV4EndPoint>(adapter->NetMask());
                ASSERT_NOT_NULL(endPoint);
                EXPECT_EQ(IPV4EndPoint::Parse(_("255.0.0.0:0")), endPoint->GetIPAddress());
                EXPECT_EQ(uint16_t{0}, endPoint->GetPort());
                EXPECT_NULL(adapter->BroadcastAddress());
                EXPECT_NULL(adapter->DestAddress());
            }
            else if (SocketFamily::InternetV6 == family)
            {
                ASSERT_NOT_NULL(adapter->LocalAddress());
                shared_ptr<IPV6EndPoint> endPoint = dynamic_pointer_cast<IPV6EndPoint>(adapter->LocalAddress());
                ASSERT_NOT_NULL(endPoint);
                EXPECT_EQ(IPV6Address::Parse(_("::1")), endPoint->GetIPAddress());
                EXPECT_EQ(uint16_t{0}, endPoint->GetPort());
                EXPECT_EQ(uint32_t{0}, endPoint->GetFlowInfo());
                EXPECT_EQ(uint32_t{0}, endPoint->GetScopeID());
                ASSERT_NOT_NULL(adapter->NetMask());
                endPoint = dynamic_pointer_cast<IPV6EndPoint>(adapter->NetMask());
                ASSERT_NOT_NULL(endPoint);
                EXPECT_EQ(IPV6Address::Parse(_("FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF")), endPoint->GetIPAddress());
                EXPECT_EQ(uint16_t{0}, endPoint->GetPort());
                EXPECT_EQ(uint32_t{0}, endPoint->GetFlowInfo());
                EXPECT_EQ(uint32_t{0}, endPoint->GetScopeID());
                EXPECT_NULL(adapter->BroadcastAddress());
                EXPECT_NULL(adapter->DestAddress());
            }
            else if (SocketFamily::Packet == family)
            {
                EXPECT_EQ(MACAddress({0, 0, 0, 0, 0, 0}), *(adapter->LocalAddress()));
            }
        }
    }
#endif
}

} // TEST_SUITE(osal)
} // namespace Test
} // namespace Network
} // namespace OSAL
