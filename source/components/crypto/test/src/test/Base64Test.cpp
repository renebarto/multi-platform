#include <unittest-c++/UnitTestC++.h>

#include <crypto/Base64.h>

using namespace std;

namespace Crypto {
namespace Test
{

class Base64Test
    : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();

    virtual void TearDown();
};

void Base64Test::SetUp()
{
}

void Base64Test::TearDown()
{
}

TEST_SUITE(crypto)
{

struct TestDataBase64
{
    OSAL::String dataDecoded;
    OSAL::String dataEncoded;
};

const vector<TestDataBase64> TestVectors1 =
{
    {
        _("fo"),
        _("Zm8="),
    },
};

TEST_FIXTURE(Base64Test, Process1)
{
    Base64 target;
    for (auto testVector : TestVectors1)
    {
        OSAL::String dataIn = testVector.dataDecoded;
        OSAL::String dataEncoded;
        OSAL::String dataDecoded;
        OSAL::String referenceEncrypted = testVector.dataEncoded;
        OSAL::String referenceDecrypted = testVector.dataDecoded;
        target.Encode(dataIn, dataEncoded);
        EXPECT_EQ(referenceEncrypted, dataEncoded);
        target.Decode(dataEncoded, dataDecoded);
        EXPECT_EQ(referenceDecrypted, dataDecoded);
    }
}

const vector<TestDataBase64> TestVectors2 =
{
    {
        _("foobar"),
        _("Zm9vYmFy"),
    },
};

TEST_FIXTURE(Base64Test, Process2)
{
    Base64 target;
    for (auto testVector : TestVectors2)
    {
        OSAL::String dataIn = testVector.dataDecoded;
        OSAL::String dataEncoded;
        OSAL::String dataDecoded;
        OSAL::String referenceEncrypted = testVector.dataEncoded;
        OSAL::String referenceDecrypted = testVector.dataDecoded;
        target.Encode(dataIn, dataEncoded);
        EXPECT_EQ(referenceEncrypted, dataEncoded);
        target.Decode(dataEncoded, dataDecoded);
        EXPECT_EQ(referenceDecrypted, dataDecoded);
    }
}

const vector<TestDataBase64> TestVectors3 =
{
    {
        _("Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust"
        " of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, "
        "exceeds the short vehemence of any carnal pleasure."),
            _("TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0aGlz\n"
        "IHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1c3Qgb2Yg\n"
        "dGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGlu\n"
        "dWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdlLCBleGNlZWRzIHRo\n"
        "ZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4="),
    },
};

TEST_FIXTURE(Base64Test, Process3)
{
    Base64 target;
    for (auto testVector : TestVectors3)
    {
        OSAL::String dataIn = testVector.dataDecoded;
        OSAL::String dataEncoded;
        OSAL::String dataDecoded;
        OSAL::String referenceEncrypted = testVector.dataEncoded;
        OSAL::String referenceDecrypted = testVector.dataDecoded;
        target.Encode(dataIn, dataEncoded);
        EXPECT_EQ(referenceEncrypted, dataEncoded);
        target.Decode(dataEncoded, dataDecoded);
        EXPECT_EQ(referenceDecrypted, dataDecoded);
    }
}

} // TEST_SUITE(crypto)

} // namespace Test
} // namespace Crypto
