#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

#include <cstdlib>

#include "Game.h"
#include "GeneticTraining.h"


int main(int argc, char *argv[])
{
	srand((unsigned)time(0));
	Game game{"9BBBBBBBBB", "./res/NTW/neural_bot_17.ntw"};
	
	while(!game.ShouldClose())
	{
		game.Step();
	    //game.Update();
	    //game.Render();
	}
	
	/*
	std::cout << std::endl;
	AI::GeneticTraining training(500, 200, 10);
	training.train("./res/NTW/neural_bot_9.ntw");
	*/
	return EXIT_SUCCESS;
}