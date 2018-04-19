#pragma once

#include <memory>
#include <random>
#include <vector>
#include "osal/Unused.h"
#include "core/serialization/DeserializationImpl.h"
#include "core/serialization/SerializationImpl.h"

namespace NeuralNetwork {

template<typename ValueType>
class Neuron;

template<typename ValueType = double>
class Connection
{
public:
    Neuron<ValueType> * neuron;
    ValueType weightFactor;
    ValueType weightFactorAdjustment;
};

template<typename ValueType = double>
class Randomizer
{
public:
    Randomizer(size_t numInputs)
        : rd()
        , generator(rd())
        , distributionRangeHalfWidth( 2.4 / numInputs)
        , standardDeviation(distributionRangeHalfWidth * 2 / 6)
        , normalDistribution(0, standardDeviation)
    {}
    ValueType GetWeight()
    {
        return normalDistribution(generator);
    }
private:
    std::random_device rd;
    std::mt19937 generator;
    const ValueType distributionRangeHalfWidth;
    const ValueType standardDeviation;
    std::normal_distribution<ValueType> normalDistribution;
};

template<typename ValueType = double>
class WeightSerializer
{
public:
    WeightSerializer()
        : _weightFactors()
    {}
    WeightSerializer(const std::vector<ValueType> & weightFactors)
        : _weightFactors(weightFactors)
    {}
    bool Load(const std::string & text)
    {
        return false;
    }
    bool Save(std::string & text)
    {
        return false;
    }

private:
    std::vector<ValueType> _weightFactors;
};


template<typename ValueType = double>
class Neuron
{
public:
    using OutputFunction = std::function<ValueType(ValueType value)>;
    using ErrorGradientFunction = std::function<ValueType(ValueType expected, ValueType actual)>;
    static const OutputFunction Sigmoid;
    static const ErrorGradientFunction ErrorGradientSigmoid;
    static const OutputFunction MinusOne;

    Neuron(const OutputFunction & outputFunction = Sigmoid,
           const ErrorGradientFunction & errorGradientFunction = ErrorGradientSigmoid)
        : _inputs()
        , _output()
        , _outputFunction(outputFunction)
        , _errorGradientFunction(errorGradientFunction)
    {}

    Neuron(const std::vector<ValueType> & weightFactors,
           const OutputFunction & outputFunction = Sigmoid,
           const ErrorGradientFunction & errorGradientFunction = ErrorGradientSigmoid)
        : _inputs()
        , _output()
        , _outputFunction(outputFunction)
        , _errorGradientFunction(errorGradientFunction)
    {}

    Neuron(size_t numInputs,
           const OutputFunction & outputFunction = Sigmoid,
           const ErrorGradientFunction & errorGradientFunction = ErrorGradientSigmoid)
        : _inputs()
        , _output()
        , _outputFunction(outputFunction)
        , _errorGradientFunction(errorGradientFunction)
    {}

    void Process()
    {
        ValueType sum {};
        for (auto const & input: _inputs)
        {
            sum += (input.neuron->_output * input.weightFactor);
        }
        _output = _outputFunction(sum);
    }

    ValueType Output() const { return _output; }
    ValueType WeightedCorrection(const Connection<ValueType> & connection, ValueType expectedOutput) const
    {
        ValueType delta = _errorGradientFunction(expectedOutput, _output);
        return delta * connection.weightFactor;
    }
    void Learn(Connection<ValueType> & connection, ValueType delta, ValueType learningRate, ValueType momentum)
    {
        ValueType weightFactorAdjustment = learningRate * delta + connection.weightFactorAdjustment * momentum;
        ValueType adjustedWeight = connection.weightFactor + weightFactorAdjustment;
        connection.weightFactor = adjustedWeight;
        connection.weightFactorAdjustment = weightFactorAdjustment;
    }

protected:
    std::vector<Connection<ValueType>> _inputs;
    ValueType _output;
    const OutputFunction & _outputFunction;
    const ErrorGradientFunction & _errorGradientFunction;
};

template<typename ValueType>
const typename Neuron<ValueType>::OutputFunction Neuron<ValueType>::Sigmoid =
    [](ValueType x) -> ValueType
    { return static_cast<ValueType>(1.0 / (1.0 + exp(-x))); };
template<typename ValueType>
const typename Neuron<ValueType>::ErrorGradientFunction Neuron<ValueType>::ErrorGradientSigmoid =
    [](ValueType expected, ValueType actual) -> ValueType
    { return static_cast<ValueType>(expected * (1.0 - expected) * (actual - expected)); };
template<typename ValueType>
const typename Neuron<ValueType>::OutputFunction Neuron<ValueType>::MinusOne =
    [](ValueType UNUSED(x)) -> ValueType { return static_cast<ValueType>(-1.0); };

template<typename ValueType = double>
class Threshold : public Neuron<ValueType>
{
public:
    Threshold()
        : Neuron<ValueType>(Neuron<ValueType>::MinusOne)
    {
        Neuron<ValueType>::Process();
    }
};

template<typename ValueType = double>
class NeuronLayer
{
public:
    NeuronLayer(size_t size)
        : _neurons(size)
    {}

protected:
    std::vector<Neuron<ValueType>> _neurons;
};

template<typename ValueType = double>
class Network
{
public:
    Network(size_t inputLayerSize, size_t outputLayerSize, std::vector<size_t> layerSizes)
        : _inputLayer(inputLayerSize)
        , _outputLayer(outputLayerSize)
        , _layers()
    {
        for (auto size : layerSizes)
        {
            _layers.emplace_back(size);
        }
    }

protected:
    NeuronLayer<ValueType> _inputLayer;
    NeuronLayer<ValueType> _outputLayer;
    std::vector<NeuronLayer<ValueType>> _layers;
};

} // namespace NeuralNetwork

