#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

#include "Game.h"

/*
* changer le calcul du score de cellules des joueurs pour le mettre au runtime
*/


int main(int argc, char *argv[])
{
	Game game;
	
	while(!game.ShouldClose())
	{
	    game.Update();
	    //game.Render();
	}
	
	return EXIT_SUCCESS;
}