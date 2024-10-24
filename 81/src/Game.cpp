#include "Game.h"

Game::Game(const char* format)
{
    // read the format
    m_NbPlayer = format[0] - '0'; // convert char number to integer


    m_Board = std::make_shared<Board>(9);


    // initialize all players as humans
    m_Players.reserve(m_NbPlayer);
    for (int i = 1; i < m_NbPlayer + 1; ++i)
    {
        if(format[i] == 'P')
            m_Players.emplace_back(std::make_unique<Human>(m_Board));
        else// format[i] == 'B'
            m_Players.emplace_back(std::make_unique<NeuralBot>(m_Board));
    }

    // make the second player as a bot
    //m_Players[1]->SetPlayerType(PlayerType::Bot);
}

Game::~Game()
{
    m_Board->Display();
    std::cout << "\nApp is closed" << std::endl;
}

void Game::Update()
{
    CLEAR_SCREEN();

    //remove GreyTiles of other players
    m_Players[m_Turn == 0 ? m_NbPlayer - 1 : m_Turn - 1]->ClearGreyTiles();

    //send greyTiles
    m_Players[m_Turn]->SendGreyTiles();

    // temporarly call render to see the board to make a choice
    Render();

    if (m_Board->IsFinish())
    {
        m_ShouldClose = true;
        CLEAR_SCREEN();
        return;
    }

    m_Players[m_Turn]->Play();

    //next turn, nbPlayer - 1 because turn goes from 0 to nbPlayer - 1
    m_Turn = m_Turn == m_NbPlayer - 1 ? 0 : m_Turn + 1;
}

bool Game::ShouldClose() const
{
    if (m_ShouldClose)
    {
        CLEAR_SCREEN();
        m_Board->Display();
        std::cout << "\n\nLinear Score de Player 1  :\t" << (int)m_Players[0]->LinearScore() << '\n';
        std::cout << "AreaScore de player 1     :\t" << m_Players[0]->AreaScore();
        std::cout << "\nTotal Score for J1        :\t" << m_Players[0]->score();

        std::cout << "\n\nLinear Score de Player 2  :\t" << (int)m_Players[1]->LinearScore() << '\n';
        std::cout << "AreaScore de player 2     :\t" << m_Players[1]->AreaScore();
        std::cout << "\nTotal Score for J2        :\t" << m_Players[1]->score() << "\n\n";
        std::cout << std::endl;
    }


    return m_ShouldClose;
}
