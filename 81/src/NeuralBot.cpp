#include "NeuralBot.h"



NeuralBot::NeuralBot(std::shared_ptr<Board> board)
    : Player {board}
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