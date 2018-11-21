#include <unittest-cpp/UnitTestC++.h>

#include "osal/osal.h"
#include "osal/IPV4EndPoint.h"
#include "osal/IPV6EndPoint.h"
#include "osal/Exception.h"

using namespace std;

namespace OSAL {
namespace Network {
namespace Test {

class NetworkEndPointTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void NetworkEndPointTest::SetUp()
{

}

void NetworkEndPointTest::TearDown()
{

}

TEST_SUITE(network)
{

TEST_FIXTURE(NetworkEndPointTest, CreateIPV4SocketAddress)
{
    sockaddr_in endpointAddress({1, 2, 3, 4}, 1234);
    EndPointPtr target = Create(reinterpret_cast<sockaddr *>(&endpointAddress));
    ASSERT_NOT_NULL(target);
    EXPECT_EQ(SocketFamily::InternetV4, target->Family());
}

TEST_FIXTURE(NetworkEndPointTest, CreateIPV6SocketAddress)
{
    sockaddr_in6 endpointAddress({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }, 1234, 12345678, 87654321);
    EndPointPtr target = Create(reinterpret_cast<sockaddr *>(&endpointAddress));
    ASSERT_NOT_NULL(target);
    EXPECT_EQ(SocketFamily::InternetV6, target->Family());
}

#if defined(LINUX) || defined(APPLE)
TEST_FIXTURE(NetworkEndPointTest, CreateDomainSocketAddress)
{
    sockaddr_in6 endpointAddress({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }, 1234, 12345678, 87654321);
    EndPointPtr target = Create(reinterpret_cast<sockaddr *>(&endpointAddress));
    ASSERT_NOT_NULL(target);
    EXPECT_EQ(SocketFamily::InternetV6, target->Family());
}
#endif

TEST_FIXTURE(NetworkEndPointTest, PrintToIPV4SocketAddress)
{
    sockaddr_in endpointAddress({1, 2, 3, 4}, 1234);
    EndPointPtr target = Create(reinterpret_cast<sockaddr *>(&endpointAddress));
    ASSERT_NOT_NULL(target);
    string expected = "1.2.3.4:1234";
    ostringstream stream;
    target->PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(NetworkEndPointTest, PrintToIPV6SocketAddress)
{
    sockaddr_in6 endpointAddress({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }, 1234, 12345678, 87654321);
    EndPointPtr target = Create(reinterpret_cast<sockaddr *>(&endpointAddress));
    ASSERT_NOT_NULL(target);
    string expected = "[102:304:506:708:90a:b0c:d0e:f10]:1234%87654321";
    ostringstream stream;
    target->PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(NetworkEndPointTest, PrintToNonMemberIPV4SocketAddress)
{
    sockaddr_in endpointAddress({1, 2, 3, 4}, 1234);
    EndPointPtr target = Create(reinterpret_cast<sockaddr *>(&endpointAddress));
    ASSERT_NOT_NULL(target);
    string expected = "1.2.3.4:1234";
    ostringstream stream;
    PrintTo(*target, stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(NetworkEndPointTest, PrintToNonMemberIPV6SocketAddress)
{
    sockaddr_in6 endpointAddress({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }, 1234, 12345678, 87654321);
    EndPointPtr target = Create(reinterpret_cast<sockaddr *>(&endpointAddress));
    ASSERT_NOT_NULL(target);
    string expected = "[102:304:506:708:90a:b0c:d0e:f10]:1234%87654321";
    ostringstream stream;
    PrintTo(*target, stream);
    EXPECT_EQ(expected, stream.str());
}

} // TEST_SUITE(network)

} // namespace Test
} // namespace Network
} // namespace OSAL
