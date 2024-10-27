#include "NeuralNetwork.h"

#include <iostream>
#include <sstream>

#include <assert.h>

#include <algorithm>
#include <functional>
#include <random>




NeuralNetwork::NeuralNetwork(uint nbEntries, uint nbOutputs, std::vector<uint> dimHiddenLayers)
	: m_nbEntries(nbEntries), m_nbOutputs(nbOutputs), m_nbHiddenlayers(dimHiddenLayers.size()), m_dim_eachHiddenLayer(dimHiddenLayers)
{
	// reserve enough place for every vector.
	m_entries_weights.reserve(m_nbEntries);
	m_outputs.reserve(m_nbOutputs);
	m_hidden_neurons.reserve(m_nbHiddenlayers);
	fillVectorRNG(&m_outputsBias, m_nbOutputs);

	/*for (uint layer_indice = 0; layer_indice < m_nbHiddenlayers; layer_indice++)
	{
		m_hidden_neurons[layer_indice];
		m_hidden_neurons[layer_indice].reserve(dimHiddenLayers[layer_indice]);
	}*/

	// fill entries
	for (uint i = 0; i < m_nbEntries; i++)
	{
		std::vector<float> weights;
		fillVectorRNG(&weights, m_dim_eachHiddenLayer[0]);
		m_entries_weights.emplace_back(weights);
	}


	// fill all hidden layers
	for (uint layer_indice = 0; layer_indice < m_nbHiddenlayers; layer_indice++)
	{
		// fill one hidden layers

		std::vector<Neuron> neurons;
		neurons.reserve(dimHiddenLayers[layer_indice]);

		for (uint neuron_indice = 0; neuron_indice < dimHiddenLayers[layer_indice]; neuron_indice++)
		{
			std::vector<float> weights;

			fillVectorRNG(&weights,
				(layer_indice == m_nbHiddenlayers - 1) ? m_nbOutputs : dimHiddenLayers[layer_indice + 1]);

			neurons.emplace_back(weights, ((rand() % 60000) - 30000) / 10000);
		}

		m_hidden_neurons.emplace_back(neurons);
	}

	// fill outputs to avoid access inexistant memory in the output function
	fillVectorRNG(&m_outputs, m_nbOutputs);
	fillVectorRNG(&m_outputsBias, m_nbOutputs);
}

NeuralNetwork::NeuralNetwork(const NeuralNetwork& NN)
{
	m_nbEntries           =  NN.m_nbEntries;
	m_entries_weights     =  NN.m_entries_weights;

	m_nbOutputs           =  NN.m_nbOutputs;
  //m_outputs             =  NN.m_outputs;
	m_outputsBias         =  NN.m_outputsBias;

	m_nbHiddenlayers      =  NN.m_nbHiddenlayers;
	m_dim_eachHiddenLayer =  NN.m_dim_eachHiddenLayer;
	m_hidden_neurons      =  NN.m_hidden_neurons;
}

NeuralNetwork::NeuralNetwork(const char* filepath)
{
	loadFromFile(filepath);
}

std::vector<float> NeuralNetwork::output(const std::vector<uint> entries_values)
{

	for (uint neuron_indice = 0; neuron_indice < m_dim_eachHiddenLayer[0]; neuron_indice++)
	{
		// store the result of the weighted sum in the neuron
		m_hidden_neurons[0][neuron_indice].value = computeFirstLayerNeuronValue(entries_values, neuron_indice);
	}

	//std::cout << "\tFirst hidden layer computed\n";


	// forward propagation in hidden layers
	for (uint layer_indice = 1; layer_indice < m_nbHiddenlayers; layer_indice++)
	{
		for (uint neuron_indice = 0; neuron_indice < m_dim_eachHiddenLayer[layer_indice]; neuron_indice++)
		{
			// store the result of the weighted sum in the neuron
			m_hidden_neurons[layer_indice][neuron_indice].value = computeNeuronValue(layer_indice, neuron_indice);
		}
		//std::cout << '\t' << layer_indice + 1 << "th hidden layer computed\n";
	}

	//std::cout << "\tAll hidden layer computed\n";

	// compute the result in the last layer
	for (uint neuron_indice = 0; neuron_indice < m_nbOutputs; neuron_indice++)
	{
		// store the result of the weighted sum in the neuron
		m_outputs[neuron_indice] = computeLastLayerNeuronValue(neuron_indice);
	}


	return m_outputs;

}

void NeuralNetwork::addNeuron(uint layer)
{
	float bias;
	std::vector<float> weights;

	bias = ((rand() % 60000) - 30000)/ 10000.;

	if (layer == m_nbHiddenlayers)
	{
		// create link with next layer
		fillVectorRNG(&weights, m_nbOutputs);
		/*weights.reserve(m_nbOutputs);
		for (int _ = 0; _ < m_nbOutputs; _++)
		{
			weights.emplace_back(((rand() % 60000) - 30000) / 10000);
		}*/

		//create link with the previous layer
		for (uint i = 0; i < m_dim_eachHiddenLayer[m_nbHiddenlayers - 2]; i++)
		{
			m_hidden_neurons[m_nbHiddenlayers - 2][i].weights.emplace_back(((rand() % 60000) - 30000) / 10000.);
		}
	}
	else
	{
		// create link to next layer
		fillVectorRNG(&weights, m_dim_eachHiddenLayer[layer + 1]);

		// create link to previous layer
		if (layer == 0)
		{
			for (uint i = 0; i < m_nbEntries; i++)
			{
				m_entries_weights[i].emplace_back(((rand() % 60000) - 30000) / 10000.);
			}
		}
		else
		{
			for (uint i = 0; i < m_nbEntries; i++)
			{
				m_hidden_neurons[layer - 1][i].weights.emplace_back(((rand() % 60000) - 30000) / 10000.);
			}
		}
	}

	m_hidden_neurons[m_dim_eachHiddenLayer[layer]].emplace_back(std::move(weights), bias);
	
		
	m_dim_eachHiddenLayer[layer] += 1;
}

void NeuralNetwork::removeNeuron(uint layer, uint position)
{
	// delete the neuron
	m_hidden_neurons[layer].erase(m_hidden_neurons[layer].begin() + position);

	// delete previous connections

	if (layer == 0)
	{
		for (uint i = 0; i < m_nbEntries; i++)
		{
			m_entries_weights[i].erase(m_entries_weights[i].begin() + position);
		}
	}
	else
	{
		for (uint i = 0; i < m_dim_eachHiddenLayer[layer - 1]; i++)
		{
			m_hidden_neurons[layer - 1][i].weights.erase(m_hidden_neurons[layer - 1][i].weights.begin() + position);
		}
	}


	m_dim_eachHiddenLayer[layer] -= 1;
}

Neuron NeuralNetwork::getNeuron(uint layer, uint neuron_position) const
{
	return m_hidden_neurons[layer][neuron_position];
}

void NeuralNetwork::changeNeuron(const Neuron& neuron, uint layer, uint neuron_position)
{
	const uint _SIZE_BEFORE = m_hidden_neurons[layer][neuron_position].weights.size();
	const uint _SIZE_AFTER = neuron.weights.size();

	if (_SIZE_AFTER > _SIZE_BEFORE)
	{
		const uint delta_size = _SIZE_AFTER - _SIZE_BEFORE;
		
		Neuron n(neuron);

		n.weights.erase(n.weights.cend() - delta_size, n.weights.cend());


		m_hidden_neurons[layer][neuron_position] = n;
	}
	else if (_SIZE_AFTER < _SIZE_BEFORE)
	{
		const uint delta_size = _SIZE_BEFORE - _SIZE_AFTER;
		Neuron n(neuron);

		n.weights.reserve(n.weights.capacity() + delta_size);

		for (uint i = 0; i < delta_size; i++)
		{
			n.weights.emplace_back(m_hidden_neurons[layer][neuron_position].weights[i]);
		}

		m_hidden_neurons[layer][neuron_position] = n;
	}
	else
	{
		m_hidden_neurons[layer][neuron_position] = neuron;
	}

}

void NeuralNetwork::changeWeight(uint layer, uint neuron_position, uint other_neuron_position, float value)
{
	m_hidden_neurons[layer][neuron_position].weights[other_neuron_position] = value;
}

float NeuralNetwork::getWeight(uint layer, uint neuron_position, uint weight_position)
{
	if (layer == 0) // entry layer
	{
		return m_entries_weights[neuron_position][weight_position];
	}
	else // hidden layer
	{
		return m_hidden_neurons[layer + 1][neuron_position].weights[weight_position];
	}
}

void NeuralNetwork::saveConfig(const char* filepath) const
{
	std::ofstream file(filepath);
	assert(file.is_open());
	
	const std::string config = getConfig();

	file << config;
}

void NeuralNetwork::loadFromFile(const char* filepath)
{
	std::ifstream file_stream(filepath);

	const auto& [v_d_each_HL, d_entries, d_outputs, d_HLs, HNs, in_Weights, out_Bias] = parseFile(file_stream);

	m_nbEntries = d_entries;
	m_entries_weights = in_Weights;

	m_nbOutputs = d_outputs;
	m_outputsBias = out_Bias;

	m_nbHiddenlayers = d_HLs;
	m_dim_eachHiddenLayer = v_d_each_HL;
	m_hidden_neurons = HNs;
}


float NeuralNetwork::computeNeuronValue(uint layer_indice, uint neuron_position) const
{
	// compute the weighted sum
	// we start at the first neuron in the first hidden Layer
	float weighted_sum = 0;
	uint nbNeurons{};
	uint previousLayer_neuronIndice = 0;

	nbNeurons = m_dim_eachHiddenLayer[layer_indice - 1];

	while (previousLayer_neuronIndice < nbNeurons)
	{
		weighted_sum += m_hidden_neurons[layer_indice - 1][previousLayer_neuronIndice].value * m_hidden_neurons[layer_indice - 1][previousLayer_neuronIndice].weights[neuron_position];
		previousLayer_neuronIndice++;
	}


	// add the bias
	weighted_sum += m_hidden_neurons[layer_indice - 1][neuron_position].bias;

	// activation function
	weighted_sum = ELU(weighted_sum);


	return weighted_sum;
}


float NeuralNetwork::computeFirstLayerNeuronValue(const std::vector<uint> entries_values, uint neuron_position) const
{
	// compute the weighted sum
	// we start at the first neuron in the first hidden Layer
	float weighted_sum = 0;
	uint neuron_on_entry_layer = 0;

	while (neuron_on_entry_layer < m_nbEntries)
	{
		weighted_sum += entries_values[neuron_on_entry_layer] * m_entries_weights[neuron_on_entry_layer][neuron_position];
		neuron_on_entry_layer++;
	}

	// add the bias
	weighted_sum += m_hidden_neurons[0][neuron_position].bias;

	// activation function
	weighted_sum = ELU(weighted_sum);

	return weighted_sum;
}


float NeuralNetwork::computeLastLayerNeuronValue(uint neuron_position) const
{
	// compute the weighted sum
	// we start at the first neuron in the last hidden Layer
	float weighted_sum = 0;
	uint nbNeurons{};
	uint previousLayer_neuronIndice = 0;

	nbNeurons = m_dim_eachHiddenLayer[m_nbHiddenlayers - 1];
	while (previousLayer_neuronIndice < nbNeurons)
	{
		weighted_sum += m_hidden_neurons[m_nbHiddenlayers - 1][previousLayer_neuronIndice].value * m_hidden_neurons[m_nbHiddenlayers - 1][previousLayer_neuronIndice].weights[0];
		previousLayer_neuronIndice++;
	}

	// add the bias
	weighted_sum += m_outputsBias[neuron_position];

	// activation function
	// we choose the sigmoid to interpret the result as the probability that the result is good
	// for the 81, it represent the probability that a certain position is good
	weighted_sum = sigmoid(weighted_sum);

	return weighted_sum;
}

void NeuralNetwork::fillVectorRNG(std::vector<float>* vec, uint size) const
{
	vec->reserve(size);
	for (uint i = 0; i < size; i++)
	{
		vec->emplace_back(((rand() % 60000) - 30000) / 10000.);
	}
}

NNtwCaracteristics NeuralNetwork::parseFile(std::ifstream& fileStream)
{
	NNtwCaracteristics caracteristics;

	std::string line{};
	std::stringstream ss;


	// copy the file into the string stream
	assert(fileStream.is_open());
	while (std::getline(fileStream, line))
	{
		ss << line << '\n';
	}

	// handle the config
	// each dimensions
	ss >> caracteristics.d_entries;
	ss >> caracteristics.d_HLs;

	for (uint i = 0; i < caracteristics.d_HLs; i++)
	{
		ss >> caracteristics.v_d_each_HL[i];
	}
	ss >> caracteristics.d_outputs;

	// the entries' weights
	for (uint neuron_indice = 0; neuron_indice < caracteristics.d_entries; neuron_indice++)
	{
		for (uint weight_indice = 0; weight_indice < caracteristics.v_d_each_HL[0]; weight_indice++)
		{
			ss >> caracteristics.in_Weights[neuron_indice][weight_indice];
		}
	}

	// each hidden layer except the last one
	for (uint layer_indice = 0; layer_indice < caracteristics.d_HLs - 1; layer_indice++)
	{
		for (uint neuron_indice = 0; neuron_indice < caracteristics.v_d_each_HL[layer_indice]; neuron_indice++)
		{
			for (uint weight_indice = 0; weight_indice < caracteristics.v_d_each_HL[layer_indice + 1]; weight_indice++)
			{
				ss >> caracteristics.HNs[layer_indice][neuron_indice].weights[weight_indice];
			}
			ss >> caracteristics.HNs[layer_indice][neuron_indice].bias;
		}
	}

	// the last hidden layer
	for (uint neuron_indice = 0; neuron_indice < caracteristics.v_d_each_HL[caracteristics.d_HLs - 1]; neuron_indice++)
	{
		for (uint weight_indice = 0; weight_indice < caracteristics.d_outputs; weight_indice++)
		{
			ss >> caracteristics.HNs[caracteristics.d_HLs - 1][neuron_indice].weights[weight_indice];
		}
		ss >> caracteristics.HNs[caracteristics.d_HLs - 1][neuron_indice].bias;
	}

	// the outputs' bias
	for (uint neuron_indice = 0; neuron_indice < caracteristics.d_outputs; neuron_indice++)
	{
		ss >> caracteristics.out_Bias[neuron_indice];
	}


	return caracteristics;
}

std::string&& NeuralNetwork::getConfig() const
{
	std::string config = "";

	// first line is all numbers :
	//		• nb Entry;
	//		• nb HiddenLayer;
	//		• NB_HL 1;
	//		• ...
	//		• NB_HL max;
	//		• nb outputs\n

	config.append(std::to_string(m_nbEntries));
	config.append(" ");
	config.append(std::to_string(m_nbHiddenlayers));
	config.append(" ");
	for (uint i = 0; i < m_nbHiddenlayers; i++)
	{
		config.append(std::to_string(m_dim_eachHiddenLayer[i]));
		config.append(" ");
	}
	config.append(std::to_string(m_nbOutputs));
	config.append("\n");

	// Next line is entries' weights
	for (uint neuron_indice = 0; neuron_indice < m_nbEntries; neuron_indice++)
	{
		for (uint weight_indice = 0; weight_indice < m_dim_eachHiddenLayer[0]; weight_indice++)
		{
			config.append(std::to_string(m_entries_weights[neuron_indice][weight_indice]));
			config.append(" ");
		}
		config.append(" ");
	}
	config.append("\n");



	// next lines are hidden layers
	for (uint layer_indice = 0; layer_indice < m_nbHiddenlayers - 1; layer_indice++)
	{
		// we handle the last hidden layer appart
		for (uint neuron_indice = 0; neuron_indice < m_dim_eachHiddenLayer[layer_indice]; neuron_indice++)
		{
			for (uint weight_indice = 0; weight_indice < m_dim_eachHiddenLayer[layer_indice + 1]; weight_indice++)
			{
				config.append(std::to_string(m_hidden_neurons[layer_indice][neuron_indice].weights[weight_indice]));
				config.append(" ");
			}
			config.append(std::to_string(m_hidden_neurons[layer_indice][neuron_indice].bias));
			config.append(" ");
		}
		config.append("\n");
	}

	// last hidden layer
	for (uint neuron_indice = 0; neuron_indice < m_dim_eachHiddenLayer[m_nbHiddenlayers - 1]; neuron_indice++)
	{
		for (uint weight_indice = 0; weight_indice < m_nbOutputs; weight_indice++)
		{
			config.append(std::to_string(m_hidden_neurons[m_nbHiddenlayers - 1][neuron_indice].weights[weight_indice]));
			config.append(" ");
		}
		config.append(std::to_string(m_hidden_neurons[m_nbHiddenlayers - 1][neuron_indice].bias));
		config.append(" ");
	}
	config.append("\n");

	// outputs bias
	for (uint neuron_indice = 0; neuron_indice < m_nbOutputs; neuron_indice++)
	{
		config.append(std::to_string(m_outputsBias[neuron_indice]));
		config.append(" ");
	}

	return static_cast<std::string&&>(config);
}

