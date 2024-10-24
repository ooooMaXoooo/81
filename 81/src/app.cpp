#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

#include "Game.h"


int main(int argc, char *argv[])
{
	Game game{ "2PP" };
	
	while(!game.ShouldClose())
	{
	    game.Update();
	    //game.Render();
	}
	
	return EXIT_SUCCESS;
}