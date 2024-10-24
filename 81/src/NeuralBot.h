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
	* • genetic representation of a solution
	* • a function to generate new solutions
	* • a selection function to choose which individuals will reproduce
	* • a fitness function
	* • a cross-over function
	* • a mutation function
	*/


#include "Player.h"

#include "NeuralNetwork/NeuralNetwork.h"

class NeuralBot : public Player
{
public :
	NeuralBot(std::shared_ptr<Board> board);

	void Play();

	
};