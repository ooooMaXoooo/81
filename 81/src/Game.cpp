#include "Game.h"

Game::Game(int nbPlayer)
    : m_NbPlayer(nbPlayer)
{
    m_Board = std::make_shared<Board>(9);


    // initialize all players as humans
    m_Players.reserve(nbPlayer);
    for (int i = 0; i < nbPlayer; ++i)
    {
        m_Players.emplace_back(std::make_unique<Player>(i + 1, PlayerType::Human, m_Board));
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
    //CLEAR_SCREEN();
    LOG_STARS(30);
    std::cout << "\n\nscore de Player 1 : " << m_Players[0]->score() << '\n';
    std::cout << "score de Player 2 : " << m_Players[1]->score() << '\n';
    std::cout << std::endl;

    //remove GreyTiles of other players
    m_Players[m_Turn == 0 ? m_NbPlayer - 1 : m_Turn - 1]->ClearGreyTiles();

    //send greyTiles
    m_Players[m_Turn]->SendGreyTiles();

    // temporarly call render to see the board to make a choice
    Render();


    //check if the pos is correct <--> return value
    while(!m_Players[m_Turn]->Play());
    //m_Board->PlayAt(m_Players[m_Turn]->PlayAt(), m_Turn + 1);
    m_ShouldClose = m_Board->IsFinish();


    //next turn, nbPlayer - 1 because turn goes from 0 to nbPlayer - 1
    m_Turn = m_Turn == m_NbPlayer - 1 ? 0 : m_Turn + 1;
}