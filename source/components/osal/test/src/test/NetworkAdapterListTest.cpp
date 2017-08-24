#include "unit-test-c++/UnitTestC++.h"
#include <osal/NetworkAdapterList.h>
#include <osal/NetworkAdapter.h>
#include <osal/IPV4Address.h>
#include <osal/IPV6Address.h>
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
                EXPECT_EQ(IPV4Address::Parse(_("127.0.0.1")), *(adapter->LocalAddress()));
                ASSERT_NOT_NULL(adapter->NetMask());
                EXPECT_EQ(IPV4Address::Parse(_("255.0.0.0")), *(adapter->NetMask()));
                EXPECT_NULL(adapter->BroadcastAddress());
                EXPECT_NULL(adapter->DestAddress());
            }
            else if (SocketFamily::InternetV6 == family)
            {
                EXPECT_EQ(IPV6Address::Parse(_("::1")), *(adapter->LocalAddress()));
                ASSERT_NOT_NULL(adapter->NetMask());
                EXPECT_EQ(IPV6Address::Parse(_("FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF")), *(adapter->NetMask()));
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
