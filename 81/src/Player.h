#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <string>
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

    // the cell-related score for a player
    uint8_t m_AreaScore = 0;

    // the array to store which area we own
    std::array<bool, 9> m_AreasStorage;

    // a count of the number of players
    inline static uint8_t m_NbPlayers;

public:
    Player(int id, PlayerType type, std::shared_ptr<Board> board);
    ~Player();

    void SendGreyTiles();

    void ClearGreyTiles();

    void Play();

    void SetPlayerType(PlayerType type) { m_Type = type; }

    float score();


    uint8_t LinearScore() const;
    float AreaScore() const { return m_AreaScore * 0.5; }

private:
    int PlayAt_Bot(const std::vector<int>& map);

    int PlayAt_Human();


    int InputInNumber(std::string& input) const;

    void UpdateGreyTiles();

    void Update_PositionScore_map(uint8_t pos);
    void getNeighbors(std::vector<uint8_t>& neighbors, uint8_t pos);
    void mergeChains(const std::vector<uint8_t>& neighbors);




    void Update_AreaScore(uint8_t pos);
    
    /// <summary>
    /// return true if the player owns the majority of slots in a given cell ~~~ change formulation
    /// </summary>
    /// <param name="cell_row">: the cell's row</param>
    /// <param name="cell_col">: the cell's column</param>
    /// <returns>true if the player owns the cells</returns>
    bool OwnCell(uint8_t cell_row, uint8_t cell_col) const;

    /// <summary>
    /// give the row and the column for cell for a given position.
    /// </summary>
    /// <param name="pos"> : the position where the player played</param>
    /// <param name="row"> : the row of the cell. Passed by reference</param>
    /// <param name="col"> : the column of the cell. Passed by reference</param>
    void GetPosCell(uint8_t pos, uint8_t& row, uint8_t& col) const;

    uint8_t findHead(uint8_t pos) const;

    void convertPosNumber_to_Char(uint8_t pos, std::string* str_out) const;
    //void convertPosChar_to_Number() const;
};