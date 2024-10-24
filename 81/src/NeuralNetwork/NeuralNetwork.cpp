#include "NeuralNetwork.h"

#include <iostream>
#include <assert.h>
#include <algorithm>
#include <functional>
#include <fstream>
#include <random>


NeuralNetwork::NeuralNetwork(uint nbEntries, uint nbOutputs, std::vector<uint> dimHiddenLayers)
	: m_nbEntries(nbEntries), m_nbOutputs(nbOutputs), m_nbHiddenlayers(dimHiddenLayers.size()), m_dim_eachHiddenLayer(dimHiddenLayers)
{
	// reserve enough place for every vector.
	m_entries_weights.reserve(m_nbEntries);
	m_outputs.reserve(m_nbOutputs);
	m_hidden_neurons.reserve(m_nbHiddenlayers);
	fillVectorRNG(&m_outputsBias, m_nbOutputs);

	for (uint layer_indice = 0; layer_indice < m_nbHiddenlayers; layer_indice++)
	{
		m_hidden_neurons[layer_indice].reserve(dimHiddenLayers[layer_indice]);
	}

	// fill entries
	for (uint i = 0; i < m_nbEntries; i++)
	{
		fillVectorRNG(&m_entries_weights[i], m_dim_eachHiddenLayer[0]);
	}


	// fill all hidden layers
	for (uint layer_indice = 0; layer_indice < m_nbHiddenlayers; layer_indice++)
	{
		// fill one hidden layers
		for (uint neuron_indice = 0; neuron_indice < dimHiddenLayers[layer_indice]; neuron_indice++)
		{
			std::vector<float> weights;
			fillVectorRNG(&weights,
				dimHiddenLayers[ (layer_indice == m_nbHiddenlayers - 1) ?
				m_nbOutputs
				: dimHiddenLayers[layer_indice + 1]]);

			m_hidden_neurons[layer_indice].emplace_back(weights, ((rand() % 60000) - 30000) / 10000);
		}
	}
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

std::array<float, 10> NeuralNetwork::output(const std::vector<float> entries_values)
{

	for (uint neuron_indice = 0; neuron_indice < m_nbEntries; neuron_indice++)
	{
		// store the result of the weighted sum in the neuron
		m_hidden_neurons[0][neuron_indice].value = computeFirstLayerNeuronValue(entries_values, neuron_indice);
	}




	// forward propagation in hidden layers
	for (uint layer_indice = 1; layer_indice < m_nbHiddenlayers; layer_indice)
	{
		for (uint neuron_indice = 0; neuron_indice < m_dim_eachHiddenLayer[layer_indice]; neuron_indice++)
		{
			// store the result of the weighted sum in the neuron
			m_hidden_neurons[layer_indice][neuron_indice].value = computeNeuronValue(layer_indice, neuron_indice);
		}
	}



	// compute the result in the last layer
	for (uint neuron_indice = 0; neuron_indice < m_nbOutputs; neuron_indice++)
	{
		// store the result of the weighted sum in the neuron
		m_outputs[neuron_indice] = computeLastLayerNeuronValue(neuron_indice);
	}



	// sort the outputs array
	std::sort(m_outputs.begin(), m_outputs.end(), std::greater<int>());

	std::array<float, 10> out;

	for (uint i = 0; i < 10; i++)
	{
		out[i] = m_outputs[i];
	}

	return out;

}

void NeuralNetwork::addNeuron(uint layer)
{
	float bias;
	std::vector<float> weights;

	bias = ((rand() % 60000) - 30000)/ 10000;

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
			m_hidden_neurons[m_nbHiddenlayers - 2][i].weights.emplace_back(((rand() % 60000) - 30000) / 10000);
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
				m_entries_weights[i].emplace_back(((rand() % 60000) - 30000) / 10000);
			}
		}
		else
		{
			for (uint i = 0; i < m_nbEntries; i++)
			{
				m_hidden_neurons[layer - 1][i].weights.emplace_back(((rand() % 60000) - 30000) / 10000);
			}
		}
	}

	m_hidden_neurons[m_dim_eachHiddenLayer[layer]].emplace_back(1, std::move(weights), bias);
	
		
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

void NeuralNetwork::changeWeight(uint layer, uint neuron_position, uint other_neuron_position, float value)
{
	m_hidden_neurons[layer][neuron_position].weights[other_neuron_position] = value;
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


float NeuralNetwork::computeFirstLayerNeuronValue(const std::vector<float> entries_values, uint neuron_position) const
{
	// compute the weighted sum
	// we start at the first neuron in the first hidden Layer
	float weighted_sum = 0;
	uint previousLayer_neuronIndice = 0;

	while (previousLayer_neuronIndice < m_nbEntries)
	{
		weighted_sum += entries_values[previousLayer_neuronIndice] * m_entries_weights[previousLayer_neuronIndice][neuron_position];
		previousLayer_neuronIndice++;
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
		vec->emplace_back(((rand() % 60000) - 30000) / 10000);
	}
}

