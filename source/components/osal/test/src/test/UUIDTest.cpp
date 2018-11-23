#include <unittest-cpp/UnitTestC++.h>

#include "osal/UUID.h"
#include "osal/Exception.h"

using namespace std;

namespace OSAL {
namespace Test {

class UUIDTest : public UnitTestCpp::TestFixture
{
public:
    static void AssertionHandler(bool expression, const char * expressionText, const char * file, int line, const char * func)
    {
        _hadAssertion = true;
        _expression = expression;
        _expressionText = expressionText;
        _file = file;
        _line = line;
        _func = func;
    }
    static bool _hadAssertion;
    static bool _expression;
    static const char * _expressionText;
    static const char * _file;
    static int _line;
    static const char * _func;

    void SetUp() override
    {
        _hadAssertion = false;
        _expression = false;
        _expressionText = nullptr;
        _file = nullptr;
        _line = 0;
        _func = nullptr;
        SetAssertionHandler(AssertionHandler);
    }
};

bool UUIDTest::_hadAssertion = false;
bool UUIDTest::_expression = false;
const char * UUIDTest::_expressionText = nullptr;
const char * UUIDTest::_file = nullptr;
int UUIDTest::_line = 0;
const char * UUIDTest::_func = nullptr;

TEST_SUITE(osal)
{

TEST_FIXTURE(UUIDTest, Constructor)
{
    OSAL::UUID target;
    OSAL::bytearray reference({0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0});
    const std::string expected = "00000000-0000-0000-0000-000000000000";
    EXPECT_TRUE(reference == target.GetBytes());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(UUIDTest, ConstructorCopy)
{
    OSAL::UUID uuid({0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                     0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F});
    OSAL::UUID target(uuid);
    const std::string expected = "00010203-0405-0607-0809-0a0b0c0d0e0f";
    EXPECT_TRUE(uuid == target.GetBytes());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(UUIDTest, ConstructorByteArray)
{
    OSAL::bytearray uuid({0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                          0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F});
    OSAL::UUID target(uuid);
    const std::string expected = "00010203-0405-0607-0809-0a0b0c0d0e0f";
    EXPECT_TRUE(uuid == target.GetBytes());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(UUIDTest, ConstructorByteArrayTooShort)
{
    EXPECT_FALSE(_hadAssertion);
    OSAL::bytearray uuidTooShort({0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                  0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E});
    OSAL::UUID target(uuidTooShort);
    const std::string expected = "00010203-0405-0607-0809-0a0b0c0d0e00";
    EXPECT_NE(uuidTooShort, target.GetBytes());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());

    EXPECT_TRUE(_hadAssertion);
}

TEST_FIXTURE(UUIDTest, ConstructorByteArrayTooLong)
{
    EXPECT_FALSE(_hadAssertion);
    OSAL::bytearray uuidTooLong({0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
                                 0x10});
    OSAL::UUID target(uuidTooLong);
    const std::string expected = "00010203-0405-0607-0809-0a0b0c0d0e0f";
    EXPECT_NE(uuidTooLong, target.GetBytes());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());

    EXPECT_TRUE(_hadAssertion);
}

#if defined(WIN_MINGW) || defined(WIN_MSVC)

TEST_FIXTURE(UUIDTest, Generate)
{
    OSAL::UUID uuid1 = UUID::Generate();
    OSAL::UUID uuid2 = UUID::Generate();
    EXPECT_EQ(UUIDVersion::Random, uuid1.GetVersion());
    EXPECT_EQ(UUIDVariant::Microsoft, uuid1.GetVariant());
    EXPECT_EQ(UUIDVersion::Random, uuid2.GetVersion());
    EXPECT_EQ(UUIDVariant::Microsoft, uuid2.GetVariant());

    EXPECT_NE(uuid1, uuid2);
}

TEST_FIXTURE(UUIDTest, GenerateTime)
{
    OSAL::UUID uuid1 = UUID::GenerateTime();
    OSAL::UUID uuid2 = UUID::GenerateTime();
    EXPECT_EQ(UUIDVersion::DateTimeMac, uuid1.GetVersion());
    EXPECT_EQ(UUIDVariant::Microsoft, uuid1.GetVariant());
    EXPECT_EQ(UUIDVersion::DateTimeMac, uuid2.GetVersion());
    EXPECT_EQ(UUIDVariant::Microsoft, uuid2.GetVariant());

    EXPECT_NE(uuid1, uuid2);
}

TEST_FIXTURE(UUIDTest, GenerateTimeSafe)
{
    OSAL::UUID uuid1 = UUID::GenerateTimeSafe();
    OSAL::UUID uuid2 = UUID::GenerateTimeSafe();
    EXPECT_EQ(UUIDVersion::DateTimeMac, uuid1.GetVersion());
    EXPECT_EQ(UUIDVariant::Microsoft, uuid1.GetVariant());
    EXPECT_EQ(UUIDVersion::DateTimeMac, uuid2.GetVersion());
    EXPECT_EQ(UUIDVariant::Microsoft, uuid2.GetVariant());

    EXPECT_NE(uuid1, uuid2);
}

TEST_FIXTURE(UUIDTest, GenerateRandom)
{
    OSAL::UUID uuid1 = UUID::GenerateRandom();
    OSAL::UUID uuid2 = UUID::GenerateRandom();
    EXPECT_EQ(UUIDVersion::Random, uuid1.GetVersion());
    EXPECT_EQ(UUIDVariant::Microsoft, uuid1.GetVariant());
    EXPECT_EQ(UUIDVersion::Random, uuid2.GetVersion());
    EXPECT_EQ(UUIDVariant::Microsoft, uuid2.GetVariant());

    EXPECT_NE(uuid1, uuid2);
}

#else

TEST_FIXTURE(UUIDTest, Generate)
{
    OSAL::UUID uuid1 = UUID::Generate();
    OSAL::UUID uuid2 = UUID::Generate();
    EXPECT_EQ(UUIDVersion::Random, uuid1.GetVersion());
    EXPECT_EQ(UUIDVariant::RFC4122, uuid1.GetVariant());
    EXPECT_EQ(UUIDVersion::Random, uuid2.GetVersion());
    EXPECT_EQ(UUIDVariant::RFC4122, uuid2.GetVariant());

    EXPECT_NE(uuid1, uuid2);
}

TEST_FIXTURE(UUIDTest, GenerateTime)
{
    OSAL::UUID uuid1 = UUID::GenerateTime();
    OSAL::UUID uuid2 = UUID::GenerateTime();
    EXPECT_EQ(UUIDVersion::DateTimeMac, uuid1.GetVersion());
    EXPECT_EQ(UUIDVariant::RFC4122, uuid1.GetVariant());
    EXPECT_EQ(UUIDVersion::DateTimeMac, uuid2.GetVersion());
    EXPECT_EQ(UUIDVariant::RFC4122, uuid2.GetVariant());

    EXPECT_NE(uuid1, uuid2);
}

TEST_FIXTURE(UUIDTest, GenerateTimeSafe)
{
    OSAL::UUID uuid1 = UUID::GenerateTimeSafe();
    OSAL::UUID uuid2 = UUID::GenerateTimeSafe();
    EXPECT_EQ(UUIDVersion::DateTimeMac, uuid1.GetVersion());
    EXPECT_EQ(UUIDVariant::RFC4122, uuid1.GetVariant());
    EXPECT_EQ(UUIDVersion::DateTimeMac, uuid2.GetVersion());
    EXPECT_EQ(UUIDVariant::RFC4122, uuid2.GetVariant());

    EXPECT_NE(uuid1, uuid2);
}

TEST_FIXTURE(UUIDTest, GenerateRandom)
{
    OSAL::UUID uuid1 = UUID::GenerateRandom();
    OSAL::UUID uuid2 = UUID::GenerateRandom();
    EXPECT_EQ(UUIDVersion::Random, uuid1.GetVersion());
    EXPECT_EQ(UUIDVariant::RFC4122, uuid1.GetVariant());
    EXPECT_EQ(UUIDVersion::Random, uuid2.GetVersion());
    EXPECT_EQ(UUIDVariant::RFC4122, uuid2.GetVariant());

    EXPECT_NE(uuid1, uuid2);
}

#endif

TEST_FIXTURE(UUIDTest, Parse)
{
    const std::string text = "0f0e0d0c-0b0a-0908-0706-050403020100";
    OSAL::bytearray uuid({0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08,
                          0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00});
    OSAL::UUID expected(uuid);
    const OSAL::UUID & actual = OSAL::UUID::Parse(text);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(UUIDTest, ParseInvalid)
{
    const std::string text1 = "0f0e0d0c-0b0a-0908-0706050403020100";
    const std::string text2 = "0f0e0d0c-0b0a-09080706-050403020100";
    const std::string text3 = "0f0e0d0c-0b0a0908-0706-050403020100";
    const std::string text4 = "0f0e0d0c0b0a-0908-0706-050403020100";
    const std::string text5 = "0f0e0d0c0b0a09080706050403020100";
    const std::string text6 = "0f0e0d0c-0b0a-0908-0706-0504030201";
    const std::string text7 = "0f0e0d0c-0b0a-0908-0706-05040302010000";

    EXPECT_THROW(OSAL::UUID::Parse(text1), OSAL::ArgumentException);
    EXPECT_THROW(OSAL::UUID::Parse(text2), OSAL::ArgumentException);
    EXPECT_THROW(OSAL::UUID::Parse(text3), OSAL::ArgumentException);
    EXPECT_THROW(OSAL::UUID::Parse(text4), OSAL::ArgumentException);
    EXPECT_THROW(OSAL::UUID::Parse(text5), OSAL::ArgumentException);
    EXPECT_THROW(OSAL::UUID::Parse(text6), OSAL::ArgumentException);
    EXPECT_THROW(OSAL::UUID::Parse(text7), OSAL::ArgumentException);
}

TEST_FIXTURE(UUIDTest, TryParse)
{
    const std::string text = "0f0e0d0c-0b0a-0908-0706-050403020100";
    OSAL::bytearray uuid({0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08,
                          0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00});
    OSAL::UUID actual;
    OSAL::UUID expected(uuid);
    EXPECT_TRUE(OSAL::UUID::TryParse(text, actual));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(UUIDTest, TryParseInvalid)
{
    const std::string text1 = "0f0e0d0c-0b0a-0908-0706050403020100";
    const std::string text2 = "0f0e0d0c-0b0a-09080706-050403020100";
    const std::string text3 = "0f0e0d0c-0b0a0908-0706-050403020100";
    const std::string text4 = "0f0e0d0c0b0a-0908-0706-050403020100";
    const std::string text5 = "0f0e0d0c0b0a09080706050403020100";
    const std::string text6 = "0f0e0d0c-0b0a-0908-0706-0504030201";
    const std::string text7 = "0f0e0d0c-0b0a-0908-0706-05040302010000";
    OSAL::UUID uuid;
    EXPECT_FALSE(OSAL::UUID::TryParse(text1, uuid));
    EXPECT_FALSE(OSAL::UUID::TryParse(text2, uuid));
    EXPECT_FALSE(OSAL::UUID::TryParse(text3, uuid));
    EXPECT_FALSE(OSAL::UUID::TryParse(text4, uuid));
    EXPECT_FALSE(OSAL::UUID::TryParse(text5, uuid));
    EXPECT_FALSE(OSAL::UUID::TryParse(text6, uuid));
    EXPECT_FALSE(OSAL::UUID::TryParse(text7, uuid));
}

TEST_FIXTURE(UUIDTest, OperatorIndex)
{
    OSAL::bytearray uuid({0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08,
                          0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00});
    OSAL::UUID target(uuid);
    EXPECT_EQ(uuid[size_t{0}], target[0]);
    EXPECT_EQ(uuid[size_t{1}], target[1]);
    EXPECT_EQ(uuid[size_t{2}], target[2]);
    EXPECT_EQ(uuid[size_t{3}], target[3]);
    EXPECT_EQ(uuid[size_t{4}], target[4]);
    EXPECT_EQ(uuid[size_t{5}], target[5]);
    EXPECT_EQ(uuid[size_t{6}], target[6]);
    EXPECT_EQ(uuid[size_t{7}], target[7]);
    EXPECT_EQ(uuid[size_t{8}], target[8]);
    EXPECT_EQ(uuid[size_t{9}], target[9]);
    EXPECT_EQ(uuid[size_t{10}], target[10]);
    EXPECT_EQ(uuid[size_t{11}], target[11]);
    EXPECT_EQ(uuid[size_t{12}], target[12]);
    EXPECT_EQ(uuid[size_t{13}], target[13]);
    EXPECT_EQ(uuid[size_t{14}], target[14]);
    EXPECT_EQ(uuid[size_t{15}], target[15]);
}

TEST_FIXTURE(UUIDTest, OperatorEqual)
{
    OSAL::bytearray uuid({0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08,
                          0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00});
    OSAL::UUID target(uuid);
    OSAL::UUID ref1;
    OSAL::UUID ref2({0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0});
    OSAL::UUID ref3(uuid);
    EXPECT_FALSE(target == ref1);
    EXPECT_FALSE(target == ref2);
    EXPECT_TRUE(target == ref3);
    EXPECT_FALSE(ref1 == target);
    EXPECT_TRUE(ref1 == ref2);
    EXPECT_FALSE(ref1 == ref3);
    EXPECT_FALSE(ref2 == target);
    EXPECT_TRUE(ref2 == ref1);
    EXPECT_FALSE(ref2 == ref3);
    EXPECT_TRUE(ref3 == target);
    EXPECT_FALSE(ref3 == ref1);
    EXPECT_FALSE(ref3 == ref2);
}

TEST_FIXTURE(UUIDTest, OperatorNotEqual)
{
    OSAL::bytearray uuid({0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08,
                          0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00});
    OSAL::UUID target(uuid);
    OSAL::UUID ref1;
    OSAL::UUID ref2({0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0});
    OSAL::UUID ref3(uuid);
    EXPECT_TRUE(target != ref1);
    EXPECT_TRUE(target != ref2);
    EXPECT_FALSE(target != ref3);
    EXPECT_TRUE(ref1 != target);
    EXPECT_FALSE(ref1 != ref2);
    EXPECT_TRUE(ref1 != ref3);
    EXPECT_TRUE(ref2 != target);
    EXPECT_FALSE(ref2 != ref1);
    EXPECT_TRUE(ref2 != ref3);
    EXPECT_FALSE(ref3 != target);
    EXPECT_TRUE(ref3 != ref1);
    EXPECT_TRUE(ref3 != ref2);
}

TEST_FIXTURE(UUIDTest, PrintTo)
{
    OSAL::UUID target({0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08,
                       0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00});
    const std::string expected = "0f0e0d0c-0b0a-0908-0706-050403020100";
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(UUIDTest, PrintToNonMember)
{
    OSAL::UUID target({0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08,
                       0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00});
    const std::string expected = "0f0e0d0c-0b0a-0908-0706-050403020100";
    ostringstream stream;
    PrintTo(stream, target);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(UUIDTest, OperatorStream)
{
    OSAL::UUID target({0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08,
                       0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00});
    const std::string expected = "0f0e0d0c-0b0a-0908-0706-050403020100";
    ostringstream stream;
    stream << target;
    EXPECT_EQ(expected, stream.str());
}

} // TEST_SUITE(osal)

} // namespace Test
} // namespace OSAL
