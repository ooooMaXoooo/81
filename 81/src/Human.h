#pragma once

#include "Player.h"

class Human : public Player
{
public :
	Human(std::shared_ptr<Board> board);

	void Play() override;

protected :
	uint8_t InputInNumber(std::string& input) const;
};