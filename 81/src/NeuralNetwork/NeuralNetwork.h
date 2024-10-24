#pragma once

#include <vector>
#include <array>
#include <math.h>
#include <string>
#include <cstdlib>
#include <ctime>

typedef unsigned short uint;

struct Neuron
{
	std::vector<float> weights;
	float value;
	float bias;

	Neuron(const std::vector<float>& _weights, float _bias)
		: value{ 1 }, weights{ _weights }, bias{ _bias }
	{}
};

class NeuralNetwork
{
private :
	// dimensions
	std::vector<uint> m_dim_eachHiddenLayer;
	uint m_nbEntries;
	uint m_nbOutputs;
	uint m_nbHiddenlayers;

	// neurons arrays
	std::vector<std::vector<Neuron>> m_hidden_neurons;


	std::vector<std::vector<float>> m_entries_weights;
	std::vector<float> m_outputs;

	// bias
	std::vector<float> m_outputsBias;


public :

	NeuralNetwork(uint nbEntries, uint nbOutputs, std::vector<uint> dimHiddenLayers);
	NeuralNetwork(const NeuralNetwork& NN);
	NeuralNetwork(const char* filepath);

	/// <summary>
	/// return the 10 best outputs given by the network
	/// </summary>
	/// <param name="entries_values">the value of entries</param>
	/// <returns>the 10 best values</returns>
	std::array<float, 10> output(const std::vector<float> entries_values);

	void addNeuron(uint layer);
	void removeNeuron(uint layer, uint position);

	void changeWeight(uint layer, uint neuron_position, uint other_neuron_position, float value);

	uint getNbEntries() const { return m_nbEntries; }
	uint getNbHiddenlayers() const { return m_nbHiddenlayers; }
	std::vector<uint> getHiddenLayersDimension() const { return m_dim_eachHiddenLayer; }

	void saveConfig(const char* filepath) const;

private :
	float sigmoid(float x) const { return 1 / (1 + exp(-x)); }
	float ELU(float x) const { return x > 0 ? x : (exp(x) - 1); }

	float computeNeuronValue(uint layer, uint position) const;
	float computeFirstLayerNeuronValue(const std::vector<float> entries_values, uint position) const;
	float computeLastLayerNeuronValue(uint neuron_position) const;


	void fillVectorRNG(std::vector<float>* vec, uint size) const;

	std::string parseFile();
};