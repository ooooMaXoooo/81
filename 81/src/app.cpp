#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

#include <cstdlib>

#include "Game.h"
#include "GeneticTraining.h"


int main(int argc, char *argv[])
{
	srand(time(NULL));
	/*Game game{"2PP"};
	
	while(!game.ShouldClose())
	{
		game.Step();
	    //game.Update();
	    //game.Render();
	}*/

	std::cout << std::endl;
	AI::GeneticTraining training;
	training.train();
	
	return EXIT_SUCCESS;
}