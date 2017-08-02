#include <unit-test-c++/UnitTestC++.h>
#include <unit-test-c++/CurrentTest.h>
#include <unit-test-c++/RecordingReporter.h>
#include <unit-test-c++/ScopedCurrentTest.h>

using namespace std;

namespace UnitTestCpp
{

namespace _Test
{

TEST_SUITE(unit_test_cpp)
{

TEST_SUITE(CheckMacros)
{

TEST(ExpectTrueSucceedsOnTrue)
{
    bool failure = true;
    {
        RecordingReporter reporter;
        UnitTestCpp::TestResults testResults(&reporter);

        ScopedCurrentTest scopedResults(testResults);
        EXPECT_TRUE(true);

        failure = (testResults.GetFailureCount() > 0);
    }

    EXPECT_FALSE(failure);
}

TEST(ExpectTrueFailsOnFalse)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_TRUE(false);
        failure = (testResults.GetFailureCount() > 0);
    }

    EXPECT_TRUE(failure);
}

TEST(ExpectTrueFailureReportsCorrectTestName)
{
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_TRUE(false);
    }

    EXPECT_EQ(_details.testName, reporter.lastFailedTest);
}

TEST(ExpectTrueFailureIncludesCheckContents)
{
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        const bool yaddayadda = false;
        EXPECT_TRUE(yaddayadda);
    }

    EXPECT_TRUE(reporter.lastFailedMessage.find(_("Value of: yaddayadda")) != string::npos);
    EXPECT_TRUE(reporter.lastFailedMessage.find(_("Actual: false")) != string::npos);
    EXPECT_TRUE(reporter.lastFailedMessage.find(_("Expected: true")) != string::npos);
}

TEST(ExpectFalseSucceedsOnFalse)
{
    bool failure = true;
    {
        RecordingReporter reporter;
        UnitTestCpp::TestResults testResults(&reporter);

        ScopedCurrentTest scopedResults(testResults);
        EXPECT_FALSE(false);

        failure = (testResults.GetFailureCount() > 0);
    }

    EXPECT_FALSE(failure);
}

TEST(ExpectFalseFailsOnTrue)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_FALSE(true);
        failure = (testResults.GetFailureCount() > 0);
    }

    EXPECT_TRUE(failure);
}

TEST(ExpectFalseFailureReportsCorrectTestName)
{
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_FALSE(true);
    }

    EXPECT_EQ(_details.testName, reporter.lastFailedTest);
}

TEST(ExpectFalseFailureIncludesCheckContents)
{
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        const bool yaddayadda = true;
        EXPECT_FALSE(yaddayadda);
    }

    EXPECT_TRUE(reporter.lastFailedMessage.find(_("Value of: yaddayadda")) != string::npos);
    EXPECT_TRUE(reporter.lastFailedMessage.find(_("Actual: true")) != string::npos);
    EXPECT_TRUE(reporter.lastFailedMessage.find(_("Expected: false")) != string::npos);
}

int ThrowingFunction()
{
    throw "Doh";
}

TEST(CheckFailsOnException)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_TRUE(ThrowingFunction() == 1);
        failure = (testResults.GetFailureCount() > 0);
    }

    EXPECT_TRUE(failure);
}

TEST(CheckFailureBecauseOfExceptionIncludesCheckContents)
{
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_TRUE(ThrowingFunction() == 1);
    }

    EXPECT_TRUE(reporter.lastFailedMessage.find(_("ThrowingFunction() == 1")) != string::npos);
}

int g_sideEffect = 0;
int FunctionWithSideEffects()
{
    ++g_sideEffect;
    return 1;
}

TEST(CheckEqualSucceedsOnEqual)
{
    bool failure = true;
    {
        RecordingReporter reporter;
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_EQ(1, 1);
        failure = (testResults.GetFailureCount() > 0);
    }

    EXPECT_TRUE(!failure);
}

TEST(CheckEqualFailsOnNotEqual)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_EQ(1, 2);
        failure = (testResults.GetFailureCount() > 0);
    }

    EXPECT_TRUE(failure);
}

TEST(CheckEqualFailsOnException)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_EQ(ThrowingFunction(), 1);
        failure = (testResults.GetFailureCount() > 0);
    }

    EXPECT_TRUE(failure);
}

TEST(CheckEqualFailureContainsCorrectDetails)
{
    int line = 0;
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        UnitTestCpp::TestDetails const testDetails(_("testName"), _("fixtureName"), _("suiteName"), _("filename"), -1);
        ScopedCurrentTest scopedResults(testResults, &testDetails);

        EXPECT_EQ(1, 123);    line = __LINE__;
    }

    EXPECT_EQ(_("testName"), reporter.lastFailedTest);
    EXPECT_EQ(_("suiteName"), reporter.lastFailedSuite);
    EXPECT_EQ(_("fixtureName"), reporter.lastFailedFixture);
    EXPECT_EQ(_("filename"), reporter.lastFailedFile);
    EXPECT_EQ(line, reporter.lastFailedLine);
}

TEST(CheckEqualFailureBecauseOfExceptionContainsCorrectDetails)
{
    int line = 0;
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        UnitTestCpp::TestDetails const testDetails(_("testName"), _("fixtureName"), _("suiteName"), _("filename"), -1);
        ScopedCurrentTest scopedResults(testResults, &testDetails);

        EXPECT_EQ(ThrowingFunction(), 123);    line = __LINE__;
    }

    EXPECT_EQ(_("testName"), reporter.lastFailedTest);
    EXPECT_EQ(_("suiteName"), reporter.lastFailedSuite);
    EXPECT_EQ(_("fixtureName"), reporter.lastFailedFixture);
    EXPECT_EQ(_("filename"), reporter.lastFailedFile);
    EXPECT_EQ(line, reporter.lastFailedLine);
}

TEST(CheckEqualFailureBecauseOfExceptionIncludesCheckContents)
{
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_EQ(ThrowingFunction(), 123);
    }

    EXPECT_TRUE(reporter.lastFailedMessage.find(_("ThrowingFunction()")) != string::npos);
    EXPECT_TRUE(reporter.lastFailedMessage.find(_("123")) != string::npos);
}

TEST(CheckEqualDoesNotHaveSideEffectsWhenPassing)
{
    g_sideEffect = 0;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_EQ(1, FunctionWithSideEffects());
    }
    EXPECT_EQ(1, g_sideEffect);
}

TEST(CheckEqualDoesNotHaveSideEffectsWhenFailing)
{
    g_sideEffect = 0;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_EQ(2, FunctionWithSideEffects());
    }
    EXPECT_EQ(1, g_sideEffect);
}


TEST(ExpectEqualSucceedsOnEqual)
{
    bool failure = true;
    {
        RecordingReporter reporter;
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_EQ(1, 1);
        failure = (testResults.GetFailureCount() > 0);
    }

    EXPECT_FALSE(failure);
}

TEST(ExpectEqualFailsOnNotEqual)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_EQ(1, 2);
        failure = (testResults.GetFailureCount() > 0);
    }

    EXPECT_TRUE(failure);
}

TEST(ExpectEqualFailsOnException)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_EQ(ThrowingFunction(), 1);
        failure = (testResults.GetFailureCount() > 0);
    }

    EXPECT_TRUE(failure);
}

TEST(ExpectEqualFailureContainsCorrectDetails)
{
    int line = 0;
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        UnitTestCpp::TestDetails const testDetails(_("testName"), _("fixtureName"), _("suiteName"), _("filename"), -1);
        ScopedCurrentTest scopedResults(testResults, &testDetails);

        EXPECT_EQ(1, 123);    line = __LINE__;
    }

    EXPECT_EQ(_("testName"), reporter.lastFailedTest);
    EXPECT_EQ(_("suiteName"), reporter.lastFailedSuite);
    EXPECT_EQ(_("fixtureName"), reporter.lastFailedFixture);
    EXPECT_EQ(_("filename"), reporter.lastFailedFile);
    EXPECT_EQ(line, reporter.lastFailedLine);
}

TEST(ExpectEqualFailureBecauseOfExceptionContainsCorrectDetails)
{
    int line = 0;
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        UnitTestCpp::TestDetails const testDetails(_("testName"), _("fixtureName"), _("suiteName"),
                                                   _("filename"), -1);
        ScopedCurrentTest scopedResults(testResults, &testDetails);

        EXPECT_EQ(ThrowingFunction(), 123);    line = __LINE__;
    }

    EXPECT_EQ(_("testName"), reporter.lastFailedTest);
    EXPECT_EQ(_("suiteName"), reporter.lastFailedSuite);
    EXPECT_EQ(_("fixtureName"), reporter.lastFailedFixture);
    EXPECT_EQ(_("filename"), reporter.lastFailedFile);
    EXPECT_EQ(line, reporter.lastFailedLine);
}

TEST(ExpectEqualFailureBecauseOfExceptionIncludesCheckContents)
{
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_EQ(ThrowingFunction(), 123);
    }

    EXPECT_TRUE(reporter.lastFailedMessage.find(_("ThrowingFunction()")) != string::npos);
    EXPECT_TRUE(reporter.lastFailedMessage.find(_("123")) != string::npos);
}

TEST(ExpectEqualDoesNotHaveSideEffectsWhenPassing)
{
    g_sideEffect = 0;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_EQ(1, FunctionWithSideEffects());
    }
    EXPECT_EQ(1, g_sideEffect);
}

TEST(ExpectEqualDoesNotHaveSideEffectsWhenFailing)
{
    g_sideEffect = 0;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_EQ(2, FunctionWithSideEffects());
    }
    EXPECT_EQ(1, g_sideEffect);
}


TEST(ExpectNotEqualSucceedsOnNotEqual)
{
    bool failure = true;
    {
        RecordingReporter reporter;
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_NE(1, 2);
        failure = (testResults.GetFailureCount() > 0);
    }

    EXPECT_FALSE(failure);
}

TEST(ExpectNotEqualFailsOnEqual)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_NE(1, 1);
        failure = (testResults.GetFailureCount() > 0);
    }

    EXPECT_TRUE(failure);
}

TEST(ExpectNotEqualFailsOnException)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_NE(ThrowingFunction(), 2);
        failure = (testResults.GetFailureCount() > 0);
    }

    EXPECT_TRUE(failure);
}

TEST(ExpectNotEqualFailureContainsCorrectDetails)
{
    int line = 0;
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        UnitTestCpp::TestDetails const testDetails(_("testName"), _("fixtureName"), _("suiteName"), _("filename"), -1);
        ScopedCurrentTest scopedResults(testResults, &testDetails);

        EXPECT_NE(1, 1);    line = __LINE__;
    }

    EXPECT_EQ(_("testName"), reporter.lastFailedTest);
    EXPECT_EQ(_("suiteName"), reporter.lastFailedSuite);
    EXPECT_EQ(_("fixtureName"), reporter.lastFailedFixture);
    EXPECT_EQ(_("filename"), reporter.lastFailedFile);
    EXPECT_EQ(line, reporter.lastFailedLine);
}

TEST(ExpectNotEqualFailureBecauseOfExceptionContainsCorrectDetails)
{
    int line = 0;
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        UnitTestCpp::TestDetails const testDetails(_("testName"), _("fixtureName"), _("suiteName"), _("filename"), -1);
        ScopedCurrentTest scopedResults(testResults, &testDetails);

        EXPECT_NE(ThrowingFunction(), 1);    line = __LINE__;
    }

    EXPECT_EQ(_("testName"), reporter.lastFailedTest);
    EXPECT_EQ(_("suiteName"), reporter.lastFailedSuite);
    EXPECT_EQ(_("fixtureName"), reporter.lastFailedFixture);
    EXPECT_EQ(_("filename"), reporter.lastFailedFile);
    EXPECT_EQ(line, reporter.lastFailedLine);
}

TEST(ExpectNotEqualFailureBecauseOfExceptionIncludesCheckContents)
{
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_NE(ThrowingFunction(), 123);
    }

    EXPECT_TRUE(reporter.lastFailedMessage.find(_("ThrowingFunction()")) != string::npos);
    EXPECT_TRUE(reporter.lastFailedMessage.find(_("123")) != string::npos);
}

TEST(ExpectNotEqualDoesNotHaveSideEffectsWhenPassing)
{
    g_sideEffect = 0;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_NE(2, FunctionWithSideEffects());
    }
    EXPECT_EQ(1, g_sideEffect);
}

TEST(ExpectNotEqualDoesNotHaveSideEffectsWhenFailing)
{
    g_sideEffect = 0;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_NE(1, FunctionWithSideEffects());
    }
    EXPECT_EQ(1, g_sideEffect);
}


TEST(CheckCloseSucceedsOnEqual)
{
    bool failure = true;
    {
        RecordingReporter reporter;
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_NEAR(1.0f, 1.001f, 0.01f);
        failure = (testResults.GetFailureCount() > 0);
    }

    EXPECT_TRUE(!failure);
}

TEST(CheckCloseFailsOnNotEqual)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_NEAR(1.0f, 1.1f, 0.01f);
        failure = (testResults.GetFailureCount() > 0);
    }

    EXPECT_TRUE(failure);
}

TEST(CheckCloseFailsOnException)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_NEAR((float)ThrowingFunction(), 1.0001f, 0.1f);
        failure = (testResults.GetFailureCount() > 0);
    }

    EXPECT_TRUE(failure);
}

TEST(CheckCloseFailureContainsCorrectDetails)
{
    int line = 0;
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        UnitTestCpp::TestDetails testDetails(_("test"), _("fixture"), _("suite"), _("filename"), -1);
        ScopedCurrentTest scopedResults(testResults, &testDetails);

        EXPECT_NEAR(1.0f, 1.1f, 0.01f);    line = __LINE__;
    }

    EXPECT_EQ(_("test"), reporter.lastFailedTest);
    EXPECT_EQ(_("fixture"), reporter.lastFailedFixture);
    EXPECT_EQ(_("suite"), reporter.lastFailedSuite);
    EXPECT_EQ(_("filename"), reporter.lastFailedFile);
    EXPECT_EQ(line, reporter.lastFailedLine);
}

TEST(CheckCloseFailureBecauseOfExceptionContainsCorrectDetails)
{
    int line = 0;
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        UnitTestCpp::TestDetails testDetails(_("closeTest"), _("closeFixture"), _("closeSuite"), _("filename"), -1);
        ScopedCurrentTest scopedResults(testResults, &testDetails);
        EXPECT_NEAR((float)ThrowingFunction(), 1.0001f, 0.1f);    line = __LINE__;
    }

    EXPECT_EQ(_("closeTest"), reporter.lastFailedTest);
    EXPECT_EQ(_("closeFixture"), reporter.lastFailedFixture);
    EXPECT_EQ(_("closeSuite"), reporter.lastFailedSuite);
    EXPECT_EQ(_("filename"), reporter.lastFailedFile);
    EXPECT_EQ(line, reporter.lastFailedLine);
}

TEST(CheckCloseFailureBecauseOfExceptionIncludesCheckContents)
{
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_NEAR((float)ThrowingFunction(), 1.0001f, 0.1f);
    }

    EXPECT_TRUE(reporter.lastFailedMessage.find(_("(float)ThrowingFunction()")) != string::npos);
    EXPECT_TRUE(reporter.lastFailedMessage.find(_("1.0001f")) != string::npos);
}

TEST(CheckCloseDoesNotHaveSideEffectsWhenPassing)
{
    g_sideEffect = 0;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_NEAR(1, FunctionWithSideEffects(), 0.1f);
    }
    EXPECT_EQ(1, g_sideEffect);
}

TEST(CheckCloseDoesNotHaveSideEffectsWhenFailing)
{
    g_sideEffect = 0;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_NEAR(2, FunctionWithSideEffects(), 0.1f);
    }
    EXPECT_EQ(1, g_sideEffect);
}


TEST(ExpectNearSucceedsOnEqual)
{
    bool failure = true;
    {
        RecordingReporter reporter;
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_NEAR (1.0f, 1.001f, 0.01f);
        failure = (testResults.GetFailureCount() > 0);
    }

    EXPECT_FALSE(failure);
}

TEST(ExpectNearFailsOnNotEqual)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_NEAR (1.0f, 1.1f, 0.01f);
        failure = (testResults.GetFailureCount() > 0);
    }

    EXPECT_TRUE(failure);
}

TEST(ExpectNearFailsOnException)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_NEAR ((float)ThrowingFunction(), 1.0001f, 0.1f);
        failure = (testResults.GetFailureCount() > 0);
    }

    EXPECT_TRUE(failure);
}

TEST(ExpectNearFailureContainsCorrectDetails)
{
    int line = 0;
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        UnitTestCpp::TestDetails testDetails(_("test"), _("fixture"), _("suite"), _("filename"), -1);
        ScopedCurrentTest scopedResults(testResults, &testDetails);

        EXPECT_NEAR (1.0f, 1.1f, 0.01f);    line = __LINE__;
    }

    EXPECT_EQ(_("test"), reporter.lastFailedTest);
    EXPECT_EQ(_("fixture"), reporter.lastFailedFixture);
    EXPECT_EQ(_("suite"), reporter.lastFailedSuite);
    EXPECT_EQ(_("filename"), reporter.lastFailedFile);
    EXPECT_EQ(line, reporter.lastFailedLine);
}

TEST(ExpectNearFailureBecauseOfExceptionContainsCorrectDetails)
{
    int line = 0;
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        UnitTestCpp::TestDetails testDetails(_("closeTest"), _("closeFixture"), _("closeSuite"), _("filename"), -1);
        ScopedCurrentTest scopedResults(testResults, &testDetails);
        EXPECT_NEAR ((float)ThrowingFunction(), 1.0001f, 0.1f);    line = __LINE__;
    }

    EXPECT_EQ(_("closeTest"), reporter.lastFailedTest);
    EXPECT_EQ(_("closeFixture"), reporter.lastFailedFixture);
    EXPECT_EQ(_("closeSuite"), reporter.lastFailedSuite);
    EXPECT_EQ(_("filename"), reporter.lastFailedFile);
    EXPECT_EQ(line, reporter.lastFailedLine);
}

TEST(ExpectNearFailureBecauseOfExceptionIncludesCheckContents)
{
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_NEAR ((float)ThrowingFunction(), 1.0001f, 0.1f);
    }

    EXPECT_TRUE(reporter.lastFailedMessage.find(_("(float)ThrowingFunction()")) != string::npos);
    EXPECT_TRUE(reporter.lastFailedMessage.find(_("1.0001f")) != string::npos);
}

TEST(ExpectNearDoesNotHaveSideEffectsWhenPassing)
{
    g_sideEffect = 0;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_NEAR (1, FunctionWithSideEffects(), 0.1f);
    }
    EXPECT_EQ(1, g_sideEffect);
}

TEST(ExpectNearDoesNotHaveSideEffectsWhenFailing)
{
    g_sideEffect = 0;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);
        EXPECT_NEAR (2, FunctionWithSideEffects(), 0.1f);
    }
    EXPECT_EQ(1, g_sideEffect);
}


class ThrowingObject
{
public:
    float operator[](int) const
    {
        throw "Test throw";
    }
};


TEST(CheckArrayCloseSucceedsOnEqual)
{
    bool failure = true;
    {
        RecordingReporter reporter;
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        const float data[4] = { 0, 1, 2, 3 };
        EXPECT_ARRAY_NEAR(data, data, 4, 0.01f);
        failure = (testResults.GetFailureCount() > 0);
    }

    EXPECT_TRUE(!failure);
}

TEST(CheckArrayCloseFailsOnNotEqual)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);

        int const data1[4] = { 0, 1, 2, 3 };
        int const data2[4] = { 0, 1, 3, 3 };
        EXPECT_ARRAY_NEAR(data1, data2, 4, 0.01f);

        failure = (testResults.GetFailureCount() > 0);
    }

    EXPECT_TRUE(failure);
}

TEST(CheckArrayCloseFailureIncludesCheckExpectedAndActual)
{
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);

        int const data1[4] = { 0, 1, 2, 3 };
        int const data2[4] = { 0, 1, 3, 3 };
        EXPECT_ARRAY_NEAR(data1, data2, 4, 0.01f);
    }

    EXPECT_TRUE(reporter.lastFailedMessage.find(_("Value of: data2")) != string::npos);
    EXPECT_TRUE(reporter.lastFailedMessage.find(_("Actual: { 0, 1, 3, 3 }")) != string::npos);
    EXPECT_TRUE(reporter.lastFailedMessage.find(_("Expected: data1")) != string::npos);
    EXPECT_TRUE(reporter.lastFailedMessage.find(_("Which is: { 0, 1, 2, 3 }")) != string::npos);
}

TEST(CheckArrayCloseFailureContainsCorrectDetails)
{
    int line = 0;
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        UnitTestCpp::TestDetails testDetails(_("arrayCloseTest"), _("arrayCloseFixture"), _("arrayCloseSuite"), _("filename"), -1);
        ScopedCurrentTest scopedResults(testResults, &testDetails);

        int const data1[4] = { 0, 1, 2, 3 };
        int const data2[4] = { 0, 1, 3, 3 };
        EXPECT_ARRAY_NEAR(data1, data2, 4, 0.01f);     line = __LINE__;
    }

    EXPECT_EQ(_("arrayCloseTest"), reporter.lastFailedTest);
    EXPECT_EQ(_("arrayCloseFixture"), reporter.lastFailedFixture);
    EXPECT_EQ(_("arrayCloseSuite"), reporter.lastFailedSuite);
    EXPECT_EQ(_("filename"), reporter.lastFailedFile);
    EXPECT_EQ(line, reporter.lastFailedLine);
}

TEST(CheckArrayCloseFailureBecauseOfExceptionContainsCorrectDetails)
{
    int line = 0;
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        UnitTestCpp::TestDetails testDetails(_("arrayCloseTest"), _("arrayCloseFixture"), _("arrayCloseSuite"), _("filename"), -1);
        ScopedCurrentTest scopedResults(testResults, &testDetails);

        int const data[4] = { 0, 1, 2, 3 };
        EXPECT_ARRAY_NEAR(data, ThrowingObject(), 4, 0.01f); line = __LINE__;
    }

    EXPECT_EQ(_("arrayCloseTest"), reporter.lastFailedTest);
    EXPECT_EQ(_("arrayCloseFixture"), reporter.lastFailedFixture);
    EXPECT_EQ(_("arrayCloseSuite"), reporter.lastFailedSuite);
    EXPECT_EQ(_("filename"), reporter.lastFailedFile);
    EXPECT_EQ(line, reporter.lastFailedLine);
}

TEST(CheckArrayCloseFailureIncludesTolerance)
{
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);

        float const data1[4] = { 0, 1, 2, 3 };
        float const data2[4] = { 0, 1, 3, 3 };
        EXPECT_ARRAY_NEAR(data1, data2, 4, 0.01f);
    }

    EXPECT_TRUE(reporter.lastFailedMessage.find(_("0.01")) != string::npos);
}


TEST(CheckArrayCloseFailsOnException)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);

        const float data[4] = { 0, 1, 2, 3 };
        ThrowingObject obj;
        EXPECT_ARRAY_NEAR(data, obj, 3, 0.01f);

        failure = (testResults.GetFailureCount() > 0);
    }

    EXPECT_TRUE(failure);
}

TEST(CheckArrayCloseFailureOnExceptionIncludesCheckContents)
{
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);

        const float data[4] = { 0, 1, 2, 3 };
        ThrowingObject obj;
        EXPECT_ARRAY_NEAR(data, obj, 3, 0.01f);
    }

    EXPECT_TRUE(reporter.lastFailedMessage.find(_("data")) != string::npos);
    EXPECT_TRUE(reporter.lastFailedMessage.find(_("obj")) != string::npos);
}


TEST(CheckArrayEqualSuceedsOnEqual)
{
    bool failure = true;
    {
        RecordingReporter reporter;
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);

        const float data[4] = { 0, 1, 2, 3 };
        EXPECT_ARRAY_EQ(data, data, 4);

        failure = (testResults.GetFailureCount() > 0);
    }

    EXPECT_TRUE(!failure);
}

TEST(CheckArrayEqualFailsOnNotEqual)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);

        int const data1[4] = { 0, 1, 2, 3 };
        int const data2[4] = { 0, 1, 3, 3 };
        EXPECT_ARRAY_EQ(data1, data2, 4);

        failure = (testResults.GetFailureCount() > 0);
    }

    EXPECT_TRUE(failure);
}

TEST(CheckArrayEqualFailureIncludesCheckExpectedAndActual)
{
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);

        int const data1[4] = { 0, 1, 2, 3 };
        int const data2[4] = { 0, 1, 3, 3 };
        EXPECT_ARRAY_EQ(data1, data2, 4);
    }

    EXPECT_TRUE(reporter.lastFailedMessage.find(_("Value of: data2")) != string::npos);
    EXPECT_TRUE(reporter.lastFailedMessage.find(_("Actual: { 0, 1, 3, 3 }")) != string::npos);
    EXPECT_TRUE(reporter.lastFailedMessage.find(_("Expected: data1")) != string::npos);
    EXPECT_TRUE(reporter.lastFailedMessage.find(_("Which is: { 0, 1, 2, 3 }")) != string::npos);
}

TEST(CheckArrayEqualFailureContainsCorrectInfo)
{
    int line = 0;
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);

        int const data1[4] = { 0, 1, 2, 3 };
        int const data2[4] = { 0, 1, 3, 3 };
        EXPECT_ARRAY_EQ(data1, data2, 4);     line = __LINE__;
    }

    EXPECT_EQ(_("CheckArrayEqualFailureContainsCorrectInfo"), reporter.lastFailedTest);
    EXPECT_EQ(OSAL::ToString(__FILE__), reporter.lastFailedFile);
    EXPECT_EQ(line, reporter.lastFailedLine);
}

TEST(CheckArrayEqualFailsOnException)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);

        const float data[4] = { 0, 1, 2, 3 };
        ThrowingObject obj;
        EXPECT_ARRAY_EQ(data, obj, 3);

        failure = (testResults.GetFailureCount() > 0);
    }

    EXPECT_TRUE(failure);
}

TEST(CheckArrayEqualFailureOnExceptionIncludesCheckContents)
{
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);

        const float data[4] = { 0, 1, 2, 3 };
        ThrowingObject obj;
        EXPECT_ARRAY_EQ(data, obj, 3);
    }

    EXPECT_TRUE(reporter.lastFailedMessage.find(_("data")));
    EXPECT_TRUE(reporter.lastFailedMessage.find(_("obj")));
}

float const* FunctionWithSideEffects2()
{
    ++g_sideEffect;
    static float const data[] = {1,2,3,4};
    return data;
}

TEST(CheckArrayCloseDoesNotHaveSideEffectsWhenPassing)
{
    g_sideEffect = 0;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        const float data[] = { 0, 1, 2, 3 };
        EXPECT_ARRAY_NEAR(data, FunctionWithSideEffects2(), 4, 0.01f);
    }
    EXPECT_EQ(1, g_sideEffect);
}

TEST(CheckArrayCloseDoesNotHaveSideEffectsWhenFailing)
{
    g_sideEffect = 0;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        const float data[] = { 0, 1, 3, 3 };
        EXPECT_ARRAY_NEAR(data, FunctionWithSideEffects2(), 4, 0.01f);
    }

    EXPECT_EQ(1, g_sideEffect);
}

class ThrowingObject2D
{
public:
    float* operator[](int) const
    {
        throw "Test throw";
    }
};


TEST(CheckArray2DCloseSucceedsOnEqual)
{
    bool failure = true;
    {
        RecordingReporter reporter;
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);

        const float data[2][2] = { {0, 1}, {2, 3} };
        EXPECT_ARRAY2D_NEAR(data, data, 2, 2, 0.01f);

        failure = (testResults.GetFailureCount() > 0);
    }

    EXPECT_TRUE(!failure);
}

TEST(CheckArray2DCloseFailsOnNotEqual)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);

        int const data1[2][2] = { {0, 1}, {2, 3} };
        int const data2[2][2] = { {0, 1}, {3, 3} };
        EXPECT_ARRAY2D_NEAR(data1, data2, 2, 2, 0.01f);

        failure = (testResults.GetFailureCount() > 0);
    }

    EXPECT_TRUE(failure);
}

TEST(CheckArray2DCloseFailureIncludesCheckExpectedAndActual)
{
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);

        int const data1[2][2] = { {0, 1}, {2, 3} };
        int const data2[2][2] = { {0, 1}, {3, 3} };

        EXPECT_ARRAY2D_NEAR(data1, data2, 2, 2, 0.01f);
    }

    EXPECT_TRUE(reporter.lastFailedMessage.find(_("Value of: data2")) != string::npos);
    EXPECT_TRUE(reporter.lastFailedMessage.find(_("Actual: { { 0, 1 }, { 3, 3 } }")) != string::npos);
    EXPECT_TRUE(reporter.lastFailedMessage.find(_("Expected: data1")) != string::npos);
    EXPECT_TRUE(reporter.lastFailedMessage.find(_("Which is: { { 0, 1 }, { 2, 3 } }")) != string::npos);
}

TEST(CheckArray2DCloseFailureContainsCorrectDetails)
{
    int line = 0;
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        UnitTestCpp::TestDetails testDetails(_("array2DCloseTest"), _("array2DCloseFixture"), _("array2DCloseSuite"), _("filename"), -1);
        ScopedCurrentTest scopedResults(testResults, &testDetails);

        int const data1[2][2] = { {0, 1}, {2, 3} };
        int const data2[2][2] = { {0, 1}, {3, 3} };
        EXPECT_ARRAY2D_NEAR(data1, data2, 2, 2, 0.01f);     line = __LINE__;
    }

    EXPECT_EQ(_("array2DCloseTest"), reporter.lastFailedTest);
    EXPECT_EQ(_("array2DCloseFixture"), reporter.lastFailedFixture);
    EXPECT_EQ(_("array2DCloseSuite"), reporter.lastFailedSuite);
    EXPECT_EQ(_("filename"), reporter.lastFailedFile);
    EXPECT_EQ(line, reporter.lastFailedLine);
}

TEST(CheckArray2DCloseFailureBecauseOfExceptionContainsCorrectDetails)
{
    int line = 0;
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        UnitTestCpp::TestDetails testDetails(_("array2DCloseTest"), _("array2DCloseFixture"), _("array2DCloseSuite"), _("filename"), -1);
        ScopedCurrentTest scopedResults(testResults, &testDetails);

        const float data[2][2] = { {0, 1}, {2, 3} };
        EXPECT_ARRAY2D_NEAR(data, ThrowingObject2D(), 2, 2, 0.01f);   line = __LINE__;
    }

    EXPECT_EQ(_("array2DCloseTest"), reporter.lastFailedTest);
    EXPECT_EQ(_("array2DCloseFixture"), reporter.lastFailedFixture);
    EXPECT_EQ(_("array2DCloseSuite"), reporter.lastFailedSuite);
    EXPECT_EQ(_("filename"), reporter.lastFailedFile);
    EXPECT_EQ(line, reporter.lastFailedLine);
}

TEST(CheckArray2DCloseFailureIncludesTolerance)
{
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);

        float const data1[2][2] = { {0, 1}, {2, 3} };
        float const data2[2][2] = { {0, 1}, {3, 3} };
        EXPECT_ARRAY2D_NEAR(data1, data2, 2, 2, 0.01f);
    }

    EXPECT_TRUE(reporter.lastFailedMessage.find(_("0.01")) != string::npos);
}

TEST(CheckArray2DCloseFailsOnException)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);

        const float data[2][2] = { {0, 1}, {2, 3} };
        ThrowingObject2D obj;
        EXPECT_ARRAY2D_NEAR(data, obj, 2, 2, 0.01f);

        failure = (testResults.GetFailureCount() > 0);
    }

    EXPECT_TRUE(failure);
}

TEST(CheckArray2DCloseFailureOnExceptionIncludesCheckContents)
{
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);

        const float data[2][2] = { {0, 1}, {2, 3} };
        ThrowingObject2D obj;
        EXPECT_ARRAY2D_NEAR(data, obj, 2, 2, 0.01f);
    }

    EXPECT_TRUE(reporter.lastFailedMessage.find(_("data")) != string::npos);
    EXPECT_TRUE(reporter.lastFailedMessage.find(_("obj")) != string::npos);
}

float const* const* FunctionWithSideEffects3()
{
    ++g_sideEffect;
    static float const data1[] = {0,1};
    static float const data2[] = {2,3};
    static const float* const data[] = {data1, data2};
    return data;
}

TEST(CheckArray2DCloseDoesNotHaveSideEffectsWhenPassing)
{
    g_sideEffect = 0;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        const float data[2][2] = { {0, 1}, {2, 3} };
        EXPECT_ARRAY2D_NEAR(data, FunctionWithSideEffects3(), 2, 2, 0.01f);
    }
    EXPECT_EQ(1, g_sideEffect);
}

TEST(CheckArray2DCloseDoesNotHaveSideEffectsWhenFailing)
{
    g_sideEffect = 0;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        const float data[2][2] = { {0, 1}, {3, 3} };
        EXPECT_ARRAY2D_NEAR(data, FunctionWithSideEffects3(), 2, 2, 0.01f);
    }
    EXPECT_EQ(1, g_sideEffect);
}

} // TEST_SUITE

} // TEST_SUITE(unit_test_cpp)

} // namespace _Test

} // namespace UnitTestCpp
