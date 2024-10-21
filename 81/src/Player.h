#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <array>
#include <unordered_map>

#include "Board.h"

#define LOG_STARS(x) for(int i=0;i<x;++i) {Log('*');} Log(std::endl);

enum class PlayerType
{
    Human,
    Bot
};

struct Score_Head
{
    uint8_t head_pos;
    uint8_t score;

    Score_Head(uint8_t pos, uint8_t _score)
    {
        head_pos = pos;
        score = _score;
    }
};


class Player
{
private:
    PlayerType m_Type;

    int m_ID; // starting at 1
    //float m_Score = 0;

    int m_LastPos = -11;

    // tiles around the last played tile of the player
    std::vector<int> m_GreyTiles;

    // all the tiles that have been played
    static std::vector<int> m_PlayedTiles;

    // a pointer to the board -- shared with other players
    std::shared_ptr<Board> m_Board;

    // store the score and the head for each slot of the player
    std::unordered_map<uint8_t, std::shared_ptr<Score_Head>> m_PosScore_map;

public:
    Player(int id, PlayerType type, std::shared_ptr<Board> board);

    void SendGreyTiles();

    void ClearGreyTiles();

    bool Play();

    void SetPlayerType(PlayerType type) { m_Type = type; }

    float score();

private:
    int PlayAt_Bot(const std::vector<int>& map);

    int PlayAt_Human();


    int InputInNumber(std::string& input) const;

    void UpdateGreyTiles();

    void Update_PositionScore_map(uint8_t pos);
    void getNeighbors(std::vector<uint8_t>& neighbors, uint8_t pos);
    void mergeChains(const std::vector<uint8_t>& neighbors);


    uint8_t LinearScore() const;
    float AreaScore() const;

    /// <summary>
    /// convert the board into an array of cells wich are represented by an array of 9 integers
    /// </summary>
    /// <param name="board">: the board to slice</param>
    /// <param name="cells">: a pointer to an array of 9 cells (an array of 9 arrays of 9 integers)</param>
    void sliceBoard(const std::vector<int>& board, std::array<std::array<int, 9>, 9>* const cells) const;
    
    /// <summary>
    /// return true if the player owns the majority of slots in a given cell
    /// </summary>
    /// <param name="cell">: the cell to check</param>
    /// <returns>true if the player owns the cells</returns>
    bool OwnCell(const std::array<int, 9>& cell) const;

};

