#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "Board.h"

#define LOG_STARS(x) for(int i=0;i<x;++i) {Log('*');} Log(std::endl);

enum class PlayerType
{
    Human,
    Bot
};


class Player
{
private:
    PlayerType m_Type;

    int m_ID;
    int m_Score = 0;

    int m_LastPos = -11;

    // tiles around the last played tile of the player
    std::vector<int> m_GreyTiles;

    // all the tiles that have been played
    static std::vector<int> m_PlayedTiles;

    std::shared_ptr<Board> m_Board;

public:
    Player(int id, PlayerType type, std::shared_ptr<Board> board);

    void SendGreyTiles();

    void ClearGreyTiles();

    bool Play();

    void SetPlayerType(PlayerType type) { m_Type = type; }

private:
    int PlayAt_Bot(const std::vector<int>& map);

    int PlayAt_Human();


    int InputInNumber(std::string& input) const;

    void UpdateGreyTiles();
};

