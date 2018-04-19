#include "unittest-cpp/UnitTestC++.h"

#include <fstream>
#include "neural-network/Neuron.h"
#include "neural-network/TestData.h"

using namespace std;

namespace NeuralNetwork {
namespace Test {

class NeuronTest : public UnitTestCpp::TestFixture
{
};

TEST_SUITE(neural_network) {

TEST_FIXTURE(NeuronTest, Constructor)
{
    Neuron<double> neuron;

    EXPECT_EQ(double { 0 }, neuron.Output());
}

TEST_FIXTURE(NeuronTest, ConstructorThreshold)
{
    Threshold<double> threshold;

    EXPECT_EQ(double { -1 }, threshold.Output());
}

} // TEST_SUITE(neural_network)

} // namespace Test
} // namespace NeuralNetwork
