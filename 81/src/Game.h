#pragma once


#include <stdexcept>
#include <string>
#include <memory>

#include "Board.h"
#include "Human.h"
#include "NeuralBot.h"


class Game
{
private:
    // all players
    std::vector<std::unique_ptr<Player>> m_Players;

    // the board on which we play
    std::shared_ptr<Board> m_Board;


    // select the player who is playing 
    // 0 for player 1
    // x-1 for player x
    int m_Turn = 0;

    // number of players (with bots)
    int m_NbPlayer = 2;

    // set to true to exit the app
    bool m_ShouldClose = false;


public:
    Game(const char* format = "2PB");

    ~Game();

    void Render() const { m_Board->Display(); }

    void Update();

    bool ShouldClose() const;
};