#pragma once
/*
	* population -- many generation
	* genome ( binary encoding maybe)
	* generation 0 is random
	* fitnessfunc ( 0 might be the worst)
	* cross-over : cut the genom at a random point and exchange the genome  --> produce 2 individuals (childs)
	*		may have more than 2 parents
	* elitism :
	*	copy n best individuals and copy them
	*
	* mutation :
	*	slightly random changes in a genome
	*
	*
	* • genetic representation of a solution									• a neural network		• OK
	* • a function to generate new solutions									• not finished			• OK ?
	* • a selection function to choose which individuals will reproduce			• not finished			• OK ?
	* • a fitness function														• not finished			• OK ?
	* • a cross-over function													• not finished			• OK ?
	* • a mutation function														• not finished			• OK ?
	* • saving system															• not finished			• almost done
	*/


#include "Player.h"

#include "NeuralNetwork/NeuralNetwork.h"

enum Mutations : char
{
	change_weight,
	add_neuron,
	remove_neuron
};

class NeuralBot : public Player
{
private :
	NeuralNetwork m_ntw;

	uint8_t m_nbPlayer;

public :
	NeuralBot(std::shared_ptr<Board> board, uint16_t id, uint8_t nbPlayers);
	NeuralBot(const NeuralBot& neuralBot);

	void Play();

	NeuralNetwork* getPtr_Ntw() { return &m_ntw; }
	
	void mutate();
};