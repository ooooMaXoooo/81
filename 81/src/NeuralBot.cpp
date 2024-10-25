#include "NeuralBot.h"



NeuralBot::NeuralBot(std::shared_ptr<Board> board, uint16_t id, uint8_t nbPlayers)
    : Player {board, id},
    m_ntw(NeuralNetwork(83, 81, { 50, 50, 50 })),
    m_nbPlayer(nbPlayers)
{
}

void NeuralBot::Play()
{
    uint8_t pos = -1;

    // do stuff



    m_LastPos = pos;
    m_PlayedTiles.emplace_back(pos);
    m_Board->Update(pos, m_ID);
         
         
    Update_PositionScore_map(pos);
    Update_AreaScore(pos);
}