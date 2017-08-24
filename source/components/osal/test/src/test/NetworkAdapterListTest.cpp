#include "unit-test-c++/UnitTestC++.h"
#include <osal/NetworkAdapterList.h>
#include <osal/IPV4Address.h>

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
    FAIL();
}

} // TEST_SUITE(osal)
} // namespace Test
} // namespace Network
} // namespace OSAL
