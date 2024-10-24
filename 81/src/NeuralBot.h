#pragma once

#include "Player.h"

class NeuralBot : public Player
{
public :
	NeuralBot(std::shared_ptr<Board> board);

	void Play();
};