#pragma once


#include <stdexcept>
#include <memory>

#include "Board.h"
#include "Player.h"


class Game
{
private:
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

    // all players
    std::vector<std::unique_ptr<Player>> m_Players;

public:
    Game(int nbPlayer = 2);

    ~Game();

    void Render() const { m_Board->Display(); }

    void Update();

    bool ShouldClose() const;
};