#include <unittest-cpp/UnitTestC++.h>

#include "osal/GUID.h"
#include "osal/Exception.h"

using namespace std;

namespace OSAL {
namespace Test {

class GUIDTest : public UnitTestCpp::TestFixture
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

bool GUIDTest::_hadAssertion = false;
bool GUIDTest::_expression = false;
const char * GUIDTest::_expressionText = nullptr;
const char * GUIDTest::_file = nullptr;
int GUIDTest::_line = 0;
const char * GUIDTest::_func = nullptr;

TEST_SUITE(osal)
{

TEST_FIXTURE(GUIDTest, Constructor)
{
    OSAL::GUID target;
    OSAL::ByteArray reference({0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0});
    const std::string expected = "00000000-0000-0000-0000-000000000000";
    EXPECT_TRUE(reference == target.GetBytes());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(GUIDTest, ConstructorCopy)
{
    OSAL::GUID guid({0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                     0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F});
    OSAL::GUID target(guid);
    const std::string expected = "00010203-0405-0607-0809-0a0b0c0d0e0f";
    EXPECT_TRUE(guid == target.GetBytes());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(GUIDTest, ConstructorByteArray)
{
    OSAL::ByteArray guid({0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                          0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F});
    OSAL::GUID target(guid);
    const std::string expected = "00010203-0405-0607-0809-0a0b0c0d0e0f";
    EXPECT_TRUE(guid == target.GetBytes());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(GUIDTest, ConstructorByteArrayTooShort)
{
    EXPECT_FALSE(_hadAssertion);
    OSAL::ByteArray guidTooShort({0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                  0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E});
    OSAL::GUID target(guidTooShort);
    const std::string expected = "00010203-0405-0607-0809-0a0b0c0d0e00";
    EXPECT_NE(guidTooShort, target.GetBytes());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());

    EXPECT_TRUE(_hadAssertion);
}

TEST_FIXTURE(GUIDTest, ConstructorByteArrayTooLong)
{
    EXPECT_FALSE(_hadAssertion);
    OSAL::ByteArray guidTooLong({0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
                                 0x10});
    OSAL::GUID target(guidTooLong);
    const std::string expected = "00010203-0405-0607-0809-0a0b0c0d0e0f";
    EXPECT_NE(guidTooLong, target.GetBytes());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());

    EXPECT_TRUE(_hadAssertion);
}

TEST_FIXTURE(GUIDTest, Generate)
{
    OSAL::GUID guid1 = GUID::Generate();
    OSAL::GUID guid2 = GUID::Generate();
    EXPECT_EQ(GUIDVersion::Random, guid1.GetVersion());
    EXPECT_EQ(0x01, guid1.GetVariant());
    EXPECT_EQ(GUIDVersion::Random, guid2.GetVersion());
    EXPECT_EQ(0x01, guid2.GetVariant());

    EXPECT_NE(guid1, guid2);
}

TEST_FIXTURE(GUIDTest, GenerateTime)
{
    OSAL::GUID guid1 = GUID::GenerateTime();
    OSAL::GUID guid2 = GUID::GenerateTime();
    EXPECT_EQ(GUIDVersion::DateTimeMac, guid1.GetVersion());
    EXPECT_EQ(0x01, guid1.GetVariant());
    EXPECT_EQ(GUIDVersion::DateTimeMac, guid2.GetVersion());
    EXPECT_EQ(0x01, guid2.GetVariant());

    EXPECT_NE(guid1, guid2);
}

TEST_FIXTURE(GUIDTest, GenerateTimeSafe)
{
    OSAL::GUID guid1 = GUID::GenerateTimeSafe();
    OSAL::GUID guid2 = GUID::GenerateTimeSafe();
    EXPECT_EQ(GUIDVersion::DateTimeMac, guid1.GetVersion());
    EXPECT_EQ(0x01, guid1.GetVariant());
    EXPECT_EQ(GUIDVersion::DateTimeMac, guid2.GetVersion());
    EXPECT_EQ(0x01, guid2.GetVariant());

    EXPECT_NE(guid1, guid2);
}

TEST_FIXTURE(GUIDTest, GenerateRandom)
{
    OSAL::GUID guid1 = GUID::GenerateRandom();
    OSAL::GUID guid2 = GUID::GenerateRandom();
    EXPECT_EQ(GUIDVersion::Random, guid1.GetVersion());
    EXPECT_EQ(0x01, guid1.GetVariant());
    EXPECT_EQ(GUIDVersion::Random, guid2.GetVersion());
    EXPECT_EQ(0x01, guid2.GetVariant());

    EXPECT_NE(guid1, guid2);
}

TEST_FIXTURE(GUIDTest, Parse)
{
    const std::string text = "0f0e0d0c-0b0a-0908-0706-050403020100";
    OSAL::ByteArray guid({0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08,
                          0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00});
    OSAL::GUID expected(guid);
    const OSAL::GUID & actual = OSAL::GUID::Parse(text);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(GUIDTest, ParseInvalid)
{
    const std::string text1 = "0f0e0d0c-0b0a-0908-0706050403020100";
    const std::string text2 = "0f0e0d0c-0b0a-09080706-050403020100";
    const std::string text3 = "0f0e0d0c-0b0a0908-0706-050403020100";
    const std::string text4 = "0f0e0d0c0b0a-0908-0706-050403020100";
    const std::string text5 = "0f0e0d0c0b0a09080706050403020100";
    const std::string text6 = "0f0e0d0c-0b0a-0908-0706-0504030201";
    const std::string text7 = "0f0e0d0c-0b0a-0908-0706-05040302010000";

    EXPECT_THROW(OSAL::GUID::Parse(text1), OSAL::ArgumentException);
    EXPECT_THROW(OSAL::GUID::Parse(text2), OSAL::ArgumentException);
    EXPECT_THROW(OSAL::GUID::Parse(text3), OSAL::ArgumentException);
    EXPECT_THROW(OSAL::GUID::Parse(text4), OSAL::ArgumentException);
    EXPECT_THROW(OSAL::GUID::Parse(text5), OSAL::ArgumentException);
    EXPECT_THROW(OSAL::GUID::Parse(text6), OSAL::ArgumentException);
    EXPECT_THROW(OSAL::GUID::Parse(text7), OSAL::ArgumentException);
}

TEST_FIXTURE(GUIDTest, TryParse)
{
    const std::string text = "0f0e0d0c-0b0a-0908-0706-050403020100";
    OSAL::ByteArray guid({0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08,
                          0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00});
    OSAL::GUID actual;
    OSAL::GUID expected(guid);
    EXPECT_TRUE(OSAL::GUID::TryParse(text, actual));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(GUIDTest, TryParseInvalid)
{
    const std::string text1 = "0f0e0d0c-0b0a-0908-0706050403020100";
    const std::string text2 = "0f0e0d0c-0b0a-09080706-050403020100";
    const std::string text3 = "0f0e0d0c-0b0a0908-0706-050403020100";
    const std::string text4 = "0f0e0d0c0b0a-0908-0706-050403020100";
    const std::string text5 = "0f0e0d0c0b0a09080706050403020100";
    const std::string text6 = "0f0e0d0c-0b0a-0908-0706-0504030201";
    const std::string text7 = "0f0e0d0c-0b0a-0908-0706-05040302010000";
    OSAL::GUID guid;
    EXPECT_FALSE(OSAL::GUID::TryParse(text1, guid));
    EXPECT_FALSE(OSAL::GUID::TryParse(text2, guid));
    EXPECT_FALSE(OSAL::GUID::TryParse(text3, guid));
    EXPECT_FALSE(OSAL::GUID::TryParse(text4, guid));
    EXPECT_FALSE(OSAL::GUID::TryParse(text5, guid));
    EXPECT_FALSE(OSAL::GUID::TryParse(text6, guid));
    EXPECT_FALSE(OSAL::GUID::TryParse(text7, guid));
}

TEST_FIXTURE(GUIDTest, OperatorIndex)
{
    OSAL::ByteArray guid({0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08,
                          0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00});
    OSAL::GUID target(guid);
    EXPECT_EQ(guid[size_t{0}], target[0]);
    EXPECT_EQ(guid[size_t{1}], target[1]);
    EXPECT_EQ(guid[size_t{2}], target[2]);
    EXPECT_EQ(guid[size_t{3}], target[3]);
    EXPECT_EQ(guid[size_t{4}], target[4]);
    EXPECT_EQ(guid[size_t{5}], target[5]);
    EXPECT_EQ(guid[size_t{6}], target[6]);
    EXPECT_EQ(guid[size_t{7}], target[7]);
    EXPECT_EQ(guid[size_t{8}], target[8]);
    EXPECT_EQ(guid[size_t{9}], target[9]);
    EXPECT_EQ(guid[size_t{10}], target[10]);
    EXPECT_EQ(guid[size_t{11}], target[11]);
    EXPECT_EQ(guid[size_t{12}], target[12]);
    EXPECT_EQ(guid[size_t{13}], target[13]);
    EXPECT_EQ(guid[size_t{14}], target[14]);
    EXPECT_EQ(guid[size_t{15}], target[15]);
}

TEST_FIXTURE(GUIDTest, OperatorEqual)
{
    OSAL::ByteArray guid({0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08,
                          0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00});
    OSAL::GUID target(guid);
    OSAL::GUID ref1;
    OSAL::GUID ref2({0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0});
    OSAL::GUID ref3(guid);
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

TEST_FIXTURE(GUIDTest, OperatorNotEqual)
{
    OSAL::ByteArray guid({0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08,
                          0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00});
    OSAL::GUID target(guid);
    OSAL::GUID ref1;
    OSAL::GUID ref2({0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0});
    OSAL::GUID ref3(guid);
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

TEST_FIXTURE(GUIDTest, PrintTo)
{
    OSAL::GUID target({0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08,
                       0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00});
    const std::string expected = "0f0e0d0c-0b0a-0908-0706-050403020100";
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(GUIDTest, PrintToNonMember)
{
    OSAL::GUID target({0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08,
                       0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00});
    const std::string expected = "0f0e0d0c-0b0a-0908-0706-050403020100";
    ostringstream stream;
    PrintTo(target, stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(GUIDTest, OperatorStream)
{
    OSAL::GUID target({0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08,
                       0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00});
    const std::string expected = "0f0e0d0c-0b0a-0908-0706-050403020100";
    ostringstream stream;
    stream << target;
    EXPECT_EQ(expected, stream.str());
}

} // TEST_SUITE(osal)

} // namespace Test
} // namespace OSAL
