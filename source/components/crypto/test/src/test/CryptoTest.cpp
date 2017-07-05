#include <unit-test-c++/UnitTestC++.h>

//#include <crypto/crypto.h>

using namespace std;

namespace Crypto {
namespace Test {

class CryptoTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void CryptoTest::SetUp()
{
}

void CryptoTest::TearDown()
{
}

TEST_FIXTURE(CryptoTest, Construct)
{
//    MyFlag f;
//
//    EXPECT_EQ(f, uint8_t{0});
//    EXPECT_EQ(f, MyFlags::ZERO);
}

} // namespace Test
} // namespace Crypto
