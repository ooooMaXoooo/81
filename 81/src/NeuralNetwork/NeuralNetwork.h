#pragma once

#include <vector>
#include <array>
#include <math.h>
#include <string>
#include <cstdlib>
#include <ctime>

#include <fstream>

typedef unsigned short uint;


struct Neuron
{
	std::vector<float> weights;
	float value;
	float bias;

	Neuron(const std::vector<float>& _weights, float _bias)
		: value{ 1 }, weights{ _weights }, bias{ _bias }
	{}

	Neuron(const Neuron& n)
		: value(1), weights(n.weights), bias(n.bias)
	{}
};

struct NNtwCaracteristics
{
	// dimensions
	std::vector<uint> v_d_each_HL;  // dimension of every hidden layers
	uint d_entries;  // nb of entries
	uint d_outputs;  // nb of outputs
	uint d_HLs;      // nb of hidden layers

	// neurons arrays
	std::vector<std::vector<Neuron>> HNs;


	std::vector<std::vector<float>> in_Weights;

	// bias
	std::vector<float> out_Bias;
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



	std::vector<float> output(const std::vector<uint> entries_values);

	void addNeuron(uint layer);
	void removeNeuron(uint layer, uint position);
	Neuron getNeuron(uint layer, uint neuron_position) const;
	void changeNeuron(const Neuron& neuron, uint layer, uint neuron_position);

	void changeWeight(uint layer, uint neuron_position, uint other_neuron_position, float value);
	float getWeight(uint layer, uint neuron_position, uint weight_position);

	uint getNbEntries() const { return m_nbEntries; }
	uint getNbOutputs() const { return m_nbOutputs; }
	uint getNbHiddenlayers() const { return m_nbHiddenlayers; }
	std::vector<uint> getHiddenLayersDimension() const { return m_dim_eachHiddenLayer; }

	void saveConfig(const char* filepath) const;
	void loadFromFile(const char* filepath);

private :
	float sigmoid(float x) const { return 1 / (1 + exp(-0.01*x)); }
	float ELU(float x) const { return x > 0 ? x : (exp(x) - 1); }

	float computeNeuronValue(uint layer, uint position) const;
	float computeFirstLayerNeuronValue(const std::vector<uint> entries_values, uint position) const;
	float computeLastLayerNeuronValue(uint neuron_position) const;


	void fillVectorRNG(std::vector<float>* vec, uint size) const;

	NNtwCaracteristics parseFile(std::ifstream& fileStream);

	std::string&& getConfig() const;
};