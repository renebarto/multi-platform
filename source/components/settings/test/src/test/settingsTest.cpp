#include <unittest-cpp/UnitTestC++.h>

#include "settings/settings.h"

using namespace std;
using namespace JSON;

namespace Settings {
namespace Test {

class variantTest: public UnitTestCpp::TestFixture
{
public:
};

class settingsTest: public UnitTestCpp::TestFixture
{
public:
};

TEST_SUITE(settings)
{

TEST_FIXTURE(settingsTest, Construct)
{
    settings target;

    EXPECT_FALSE(target.Have("A"));
}

TEST_FIXTURE(settingsTest, AddInt8)
{
    settings target;

    int8_t valueInitial {123};
    int8_t value {valueInitial};
    shared_ptr<Number> valueContainer = make_shared<Number>(value);
    EXPECT_TRUE(target.Add("A", valueContainer));
    EXPECT_TRUE(target.Have("A"));
    EXPECT_TRUE(valueContainer->GetValue(value));
    EXPECT_EQ(valueInitial, value);
}

TEST_FIXTURE(settingsTest, AddUInt8)
{
    settings target;

    uint8_t valueInitial {255};
    uint8_t value {valueInitial};
    shared_ptr<Number> valueContainer = make_shared<Number>(value);
    EXPECT_TRUE(target.Add("A", valueContainer));
    EXPECT_TRUE(target.Have("A"));
    EXPECT_TRUE(valueContainer->GetValue(value));
    EXPECT_EQ(valueInitial, value);
}

TEST_FIXTURE(settingsTest, AddInt16)
{
    settings target;

    int16_t valueInitial {12356};
    int16_t value {valueInitial};
    shared_ptr<Number> valueContainer = make_shared<Number>(value);
    EXPECT_TRUE(target.Add("A", valueContainer));
    EXPECT_TRUE(target.Have("A"));
    EXPECT_TRUE(valueContainer->GetValue(value));
    EXPECT_EQ(valueInitial, value);
}

TEST_FIXTURE(settingsTest, AddUInt16)
{
    settings target;

    uint16_t valueInitial {23456};
    uint16_t value {valueInitial};
    shared_ptr<Number> valueContainer = make_shared<Number>(value);
    EXPECT_TRUE(target.Add("A", valueContainer));
    EXPECT_TRUE(target.Have("A"));
    EXPECT_TRUE(valueContainer->GetValue(value));
    EXPECT_EQ(valueInitial, value);
}

TEST_FIXTURE(settingsTest, AddInt32)
{
    settings target;

    int32_t valueInitial {12345678};
    int32_t value {valueInitial};
    shared_ptr<Number> valueContainer = make_shared<Number>(value);
    EXPECT_TRUE(target.Add("A", valueContainer));
    EXPECT_TRUE(target.Have("A"));
    EXPECT_TRUE(valueContainer->GetValue(value));
    EXPECT_EQ(valueInitial, value);
}

TEST_FIXTURE(settingsTest, AddUInt32)
{
    settings target;

    uint32_t valueInitial {23456789};
    uint32_t value {valueInitial};
    shared_ptr<Number> valueContainer = make_shared<Number>(value);
    EXPECT_TRUE(target.Add("A", valueContainer));
    EXPECT_TRUE(target.Have("A"));
    EXPECT_TRUE(valueContainer->GetValue(value));
    EXPECT_EQ(valueInitial, value);
}

TEST_FIXTURE(settingsTest, AddInt64)
{
    settings target;

    int64_t valueInitial {123456789};
    int64_t value {valueInitial};
    shared_ptr<Number> valueContainer = make_shared<Number>(value);
    EXPECT_TRUE(target.Add("A", valueContainer));
    EXPECT_TRUE(target.Have("A"));
    EXPECT_TRUE(valueContainer->GetValue(value));
    EXPECT_EQ(valueInitial, value);
}

TEST_FIXTURE(settingsTest, AddUInt64)
{
    settings target;

    uint64_t valueInitial {234567890};
    uint64_t value {valueInitial};
    shared_ptr<Number> valueContainer = make_shared<Number>(value);
    EXPECT_TRUE(target.Add("A", valueContainer));
    EXPECT_TRUE(target.Have("A"));
    EXPECT_TRUE(valueContainer->GetValue(value));
    EXPECT_EQ(valueInitial, value);
}

TEST_FIXTURE(settingsTest, AddFloat)
{
    settings target;

    float valueInitial {1.23E05F};
    float value {valueInitial};
    shared_ptr<Number> valueContainer = make_shared<Number>(value);
    EXPECT_TRUE(target.Add("A", valueContainer));
    EXPECT_TRUE(target.Have("A"));
    EXPECT_TRUE(valueContainer->GetValue(value));
    EXPECT_EQ(valueInitial, value);
}

TEST_FIXTURE(settingsTest, AddDouble)
{
    settings target;

    double valueInitial {1.23E05};
    double value {valueInitial};
    shared_ptr<Number> valueContainer = make_shared<Number>(value);
    EXPECT_TRUE(target.Add("A", valueContainer));
    EXPECT_TRUE(target.Have("A"));
    EXPECT_TRUE(valueContainer->GetValue(value));
    EXPECT_EQ(valueInitial, value);
}

TEST_FIXTURE(settingsTest, AddLongDouble)
{
    settings target;

    long double valueInitial {1.23E05L};
    long double value {valueInitial};
    shared_ptr<Number> valueContainer = make_shared<Number>(value);
    EXPECT_TRUE(target.Add("A", valueContainer));
    EXPECT_TRUE(target.Have("A"));
    EXPECT_TRUE(valueContainer->GetValue(value));
    EXPECT_EQ(valueInitial, value);
}

TEST_FIXTURE(settingsTest, AddString)
{
    settings target;

    std::string valueInitial {"ABCD"};
    std::string value {valueInitial};
    shared_ptr<String> valueContainer = make_shared<String>(value);
    EXPECT_TRUE(target.Add("A", valueContainer));
    EXPECT_TRUE(target.Have("A"));
    value = valueContainer->GetValue();
    EXPECT_EQ(valueInitial, value);
}

TEST_FIXTURE(settingsTest, AddBoolean)
{
    settings target;

    bool valueInitial {true};
    bool value {valueInitial};
    shared_ptr<Boolean> valueContainer = make_shared<Boolean>(value);
    EXPECT_TRUE(target.Add("A", valueContainer));
    EXPECT_TRUE(target.Have("A"));
    value = valueContainer->GetValue();
    EXPECT_EQ(valueInitial, value);
}

TEST_FIXTURE(settingsTest, AddArray)
{
    settings target;

    std::vector<uint32_t> valueInitial { 0, 1, 2, 3, 4, 5 };
    shared_ptr<Array> valueContainer = make_shared<Array>();
    for (auto elementValue : valueInitial)
    {
        valueContainer->AddValue(make_shared<Number>(elementValue));
    }
    EXPECT_TRUE(target.Add("A", valueContainer));
    EXPECT_TRUE(target.Have("A"));
    auto sourceIterator = valueInitial.begin();
    auto destIterator = valueContainer->GetIterator();
    while ((sourceIterator != valueInitial.end()) && !destIterator.AtEnd())
    {
        uint32_t elementValue;
        EXPECT_TRUE(dynamic_pointer_cast<Number>(*destIterator)->GetValue(elementValue));
        EXPECT_EQ(*sourceIterator, elementValue);
        ++sourceIterator;
        ++destIterator;
    }
}

TEST_FIXTURE(settingsTest, AddObject)
{
    settings target;

    uint32_t valueInitial {1234};
    uint32_t value {valueInitial};
    shared_ptr<Object> valueContainer = make_shared<Object>();
    valueContainer->AddPair(KVPair("A", make_shared<Number>(valueInitial)));
    EXPECT_TRUE(target.Add("B", valueContainer));
    EXPECT_TRUE(target.Have("B"));
    EXPECT_TRUE((*target.Get("B"))["A"]);
    EXPECT_EQ(valueInitial, value);
}

//TEST_FIXTURE(settingsTest, AddDuplicate)
//{
//    settings target;
//
//    uint32_t initialA {1234};
//    uint32_t actualA {initialA};
//    uint32_t initialB {12345};
//    uint32_t actualB {initialB};
//
//    group groupA;
//    group groupB;
//    EXPECT_TRUE(groupA.Add("A", actualA));
//    EXPECT_FALSE(groupA.Add("A", actualB));
//    EXPECT_TRUE(target.Add("B", groupA));
//    EXPECT_FALSE(target.Add("B", groupB));
//    EXPECT_TRUE(target.Have("B"));
//    EXPECT_TRUE(groupA.Have("A"));
//    EXPECT_EQ(initialA, actualA);
//    EXPECT_EQ(initialB, actualB);
//}
//
//TEST_FIXTURE(settingsTest, Load)
//{
//    std::string serialization = "{}";
//    std::istringstream serializationStream(serialization);
//    settings target;
//
//    uint32_t initial {123};
//    uint32_t actual {initial};
//    uint32_t expected {initial};
//
//    target.Add("A", actual);
//    EXPECT_TRUE(target.Load(serializationStream));
//    EXPECT_EQ(expected, actual);
//}
//
//TEST_FIXTURE(settingsTest, LoadSingleValue)
//{
//    std::string serialization = "{ \"A\": 1234 }";
//    std::istringstream deserializationStream(serialization);
//    settings target;
//
//    uint32_t initial {123};
//    uint32_t actual {initial};
//    uint32_t expected {1234};
//
//    EXPECT_TRUE(target.Add("A", actual));
//    EXPECT_TRUE(target.Load(deserializationStream));
//    EXPECT_EQ(expected, actual);
//}
//
//TEST_FIXTURE(settingsTest, LoadSingleValueUnexpected)
//{
//    std::string serialization = "{ \"A\": 1234 }";
//    std::istringstream deserializationStream(serialization);
//    settings target;
//
//    uint32_t initial {123};
//    uint32_t actual {initial};
//    uint32_t expected {initial};
//
//    EXPECT_TRUE(target.Add("A", actual));
//    EXPECT_FALSE(target.Load(deserializationStream));
//    EXPECT_EQ(expected, actual);
//}
//
//TEST_FIXTURE(settingsTest, LoadObject)
//{
//    std::string serialization = "{ \"A\": 1234, \"B\":1234.5678, \"C\":\"Hello\" }";
//    std::istringstream serializationStream(serialization);
//    settings target;
//
//    uint32_t initialA {1234};
//    uint32_t actualA {initialA};
//    uint32_t expectedA {initialA};
//    double initialB {1234.5678};
//    double actualB {initialB};
//    double expectedB {initialB};
//    std::string initialC { "Hello" };
//    std::string actualC {initialC};
//    std::string expectedC {initialC};
//
//    EXPECT_TRUE(target.Add("A", actualA));
//    EXPECT_TRUE(target.Add("B", actualB));
//    EXPECT_TRUE(target.Add("C", actualC));
//    EXPECT_TRUE(target.Load(serializationStream));
//    EXPECT_EQ(expectedA, actualA);
//    EXPECT_EQ(expectedB, actualB);
//    EXPECT_EQ(expectedC, actualC);
//}
//
////TEST_FIXTURE(settingsTest, LoadArray)
////{
////    std::string serialization = "{ \"A\": 1234, \"B\":1234.5678, \"C\":[0,1,2,3,4,5] }";
////    std::istringstream serializationStream(serialization);
////    settings target;
////    uint32_t valueA {1234};
////    uint32_t valueAFound {};
////    double valueB {1234.5678};
////    double valueBFound {};
////    std::vector<uint32_t> valueC { 0, 1, 2, 3, 4, 5 };
////    std::vector<uint32_t> valueCFound {};
////
////    EXPECT_FALSE(target.Have("A"));
////    EXPECT_FALSE(target.Have("B"));
////    EXPECT_FALSE(target.Have("C"));
////    EXPECT_TRUE(target.Load(serializationStream));
////    EXPECT_TRUE(target.Have("A"));
////    EXPECT_TRUE(target.Get("A", valueAFound));
////    EXPECT_EQ(valueA, valueAFound);
////    EXPECT_TRUE(target.Get("B", valueBFound));
////    EXPECT_EQ(valueB, valueBFound);
////    EXPECT_TRUE(target.Get("C", valueCFound));
////    EXPECT_EQ(valueC, valueCFound);
////}
////
////TEST_FIXTURE(settingsTest, LoadNested)
////{
////    std::string serialization = "{ \"A\": 1234, \"B\":1234.5678, \"C\":[0,1,2,3,4,5] }";
////    std::istringstream serializationStream(serialization);
////    settings target;
////    uint32_t valueA {1234};
////    uint32_t valueAFound {};
////    double valueB {1234.5678};
////    double valueBFound {};
////    std::vector<uint32_t> valueC { 0, 1, 2, 3, 4, 5 };
////    std::vector<uint32_t> valueCFound {};
////
////    EXPECT_FALSE(target.Have("A"));
////    EXPECT_FALSE(target.Have("B"));
////    EXPECT_FALSE(target.Have("C"));
////    EXPECT_TRUE(target.Load(serializationStream));
////    EXPECT_TRUE(target.Have("A"));
////    EXPECT_TRUE(target.Get("A", valueAFound));
////    EXPECT_EQ(valueA, valueAFound);
////    EXPECT_TRUE(target.Get("B", valueBFound));
////    EXPECT_EQ(valueB, valueBFound);
////    EXPECT_TRUE(target.Get("C", valueCFound));
////    EXPECT_EQ(valueC, valueCFound);
////    FAIL();
////}
////
////TEST_FIXTURE(settingsTest, SaveSingleValue)
////{
////    std::string serialization = "{\n    \"A\" : 1234\n}";
////    std::ostringstream serializationStream;
////    settings target;
////    uint32_t value {1234};
////    uint32_t valueFound {};
////    EXPECT_TRUE(target.Set("A", value));
////
////    EXPECT_TRUE(target.Have("A"));
////    EXPECT_TRUE(target.Get("A", valueFound));
////    EXPECT_EQ(value, valueFound);
////    target.Save(serializationStream);
////    EXPECT_EQ(serialization, serializationStream.str());
////}
////
////TEST_FIXTURE(settingsTest, SaveObject)
////{
////    std::string serialization = "{\n    \"A\" : 1234,\n    \"B\" : 1234.5678,\n    \"C\" : \"Hello\"\n}";
////    std::ostringstream serializationStream;
////    settings target;
////    uint32_t valueA {1234};
////    uint32_t valueAFound {};
////    double valueB {1234.5678};
////    double valueBFound {};
////    std::string valueC { "Hello" };
////    std::string valueCFound {};
////    EXPECT_TRUE(target.Set("A", valueA));
////    EXPECT_TRUE(target.Set("B", valueB));
////    EXPECT_TRUE(target.Set("C", valueC));
////
////    EXPECT_TRUE(target.Have("A"));
////    EXPECT_TRUE(target.Have("B"));
////    EXPECT_TRUE(target.Have("C"));
////    EXPECT_TRUE(target.Get("A", valueAFound));
////    EXPECT_EQ(valueA, valueAFound);
////    EXPECT_TRUE(target.Get("B", valueBFound));
////    EXPECT_EQ(valueB, valueBFound);
////    EXPECT_TRUE(target.Get("C", valueCFound));
////    EXPECT_EQ(valueC, valueCFound);
////    target.Save(serializationStream);
////    EXPECT_EQ(serialization, serializationStream.str());
////}
////
////TEST_FIXTURE(settingsTest, SaveArray)
////{
////    std::string serialization = "{\n"
////                                "    \"A\" : 1234,\n"
////                                "    \"B\" : 1234.5678,\n"
////                                "    \"C\" : [\n"
////                                "        0,\n"
////                                "        1,\n"
////                                "        2,\n"
////                                "        3,\n"
////                                "        4,\n"
////                                "        5\n"
////                                "    ]\n"
////                                "}";
////    std::ostringstream serializationStream;
////    settings target;
////    uint32_t valueA {1234};
////    uint32_t valueAFound {};
////    double valueB {1234.5678};
////    double valueBFound {};
////    std::vector<uint32_t> valueC { 0, 1, 2, 3, 4, 5 };
////    std::vector<uint32_t> valueCFound {};
////    EXPECT_TRUE(target.Set("A", valueA));
////    EXPECT_TRUE(target.Set("B", valueB));
////    EXPECT_TRUE(target.Set("C", valueC));
////
////    EXPECT_TRUE(target.Have("A"));
////    EXPECT_TRUE(target.Have("B"));
////    EXPECT_TRUE(target.Have("C"));
////    EXPECT_TRUE(target.Get("A", valueAFound));
////    EXPECT_EQ(valueA, valueAFound);
////    EXPECT_TRUE(target.Get("B", valueBFound));
////    EXPECT_EQ(valueB, valueBFound);
////    EXPECT_TRUE(target.Get("C", valueCFound));
////    EXPECT_EQ(valueC, valueCFound);
////    target.Save(serializationStream);
////    EXPECT_EQ(serialization, serializationStream.str());
////}
////
////TEST_FIXTURE(settingsTest, SaveNested)
////{
////    std::string serialization = "{\n"
////        "    \"A\" : 1234,\n"
////        "    \"B\" : 1234.5678,\n"
////        "    \"C\" : [\n"
////        "        0,\n"
////        "        1,\n"
////        "        2,\n"
////        "        3,\n"
////        "        4,\n"
////        "        5\n"
////        "    ]\n"
////        "}";
////    std::ostringstream serializationStream;
////    settings target;
////    uint32_t valueA {1234};
////    uint32_t valueAFound {};
////    double valueB {1234.5678};
////    double valueBFound {};
////    std::vector<uint32_t> valueC { 0, 1, 2, 3, 4, 5 };
////    std::vector<uint32_t> valueCFound {};
////    EXPECT_TRUE(target.Set("A", valueA));
////    EXPECT_TRUE(target.Set("B", valueB));
////    EXPECT_TRUE(target.Set("C", valueC));
////
////    EXPECT_TRUE(target.Have("A"));
////    EXPECT_TRUE(target.Have("B"));
////    EXPECT_TRUE(target.Have("C"));
////    EXPECT_TRUE(target.Get("A", valueAFound));
////    EXPECT_EQ(valueA, valueAFound);
////    EXPECT_TRUE(target.Get("B", valueBFound));
////    EXPECT_EQ(valueB, valueBFound);
////    EXPECT_TRUE(target.Get("C", valueCFound));
////    EXPECT_EQ(valueC, valueCFound);
////    target.Save(serializationStream);
////    EXPECT_EQ(serialization, serializationStream.str());
////    FAIL();
////}

} // TEST_SUITE(settings)

} // namespace Test
} // namespace Settings

