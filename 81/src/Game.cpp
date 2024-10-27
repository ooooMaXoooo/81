#include "Game.h"

Game::Game(const char* format, const char* file)
{
    // read the format
    m_NbPlayer = format[0] - '0'; // convert char number to integer


    m_Board = std::make_shared<Board>(9);

    // initialize all players as humans
    m_Players.reserve(m_NbPlayer);
    for (int i = 1; i < m_NbPlayer + 1; ++i)
    {
        if(format[i] == 'P')
            m_Players.emplace_back(std::make_unique<Human>(m_Board, i));
        else// format[i] == 'B'
        {
            /*
            if (file == "\0null")
            {
                m_Players.emplace_back(std::make_unique<NeuralBot>(m_Board, i, m_NbPlayer));
            }
            else
            {
                m_Players.emplace_back(std::make_unique<NeuralBot>(m_Board, i, m_NbPlayer, file));
            }*/
            
            if (i == 1)
            {
                m_Players.emplace_back(std::make_unique<NeuralBot>(m_Board, i, m_NbPlayer, file));
            }
            else
            {
                std::string filepath = "./res/NTW/neural_bot_" + std::to_string(i+5) + ".ntw";
                //m_Players.emplace_back(std::make_unique<NeuralBot>(m_Board, i, m_NbPlayer));
                m_Players.emplace_back(std::make_unique<NeuralBot>(m_Board, i, m_NbPlayer, filepath.c_str()));
            }
        }
    }
}

Game::~Game()
{
    m_Board->Display();
    std::cout << "\nApp is closed" << std::endl;
}

void Game::Update()
{
    if (m_Turn % m_NbPlayer == m_NbPlayer - 1)
    {
        CLEAR_SCREEN();
    }

    //remove GreyTiles of other players
    m_Players[m_Turn == 0 ? m_NbPlayer - 1 : m_Turn - 1]->ClearGreyTiles();

    //send greyTiles
    m_Players[m_Turn]->SendGreyTiles();

    // temporarly call render to see the board to make a choice
    //Render();
    //Play();    
}

bool Game::ShouldClose() const
{
    if (m_ShouldClose)
    {
        
        //CLEAR_SCREEN();
        m_Board->Display();
        for (int i = 0; i < m_NbPlayer; i++)
        {
            std::cout << "\n\nLinear Score de Player "<< i+1 << "  :\t" << (int)m_Players[i]->LinearScore() << '\n';
            std::cout << "AreaScore de player "<< i+1 << "     :\t" << m_Players[i]->AreaScore();
            std::cout << "\nTotal Score for J"<< i+1 << "        :\t" << m_Players[i]->score();
        }
        std::cout << "\n\n" << std::endl;
    }


    return m_ShouldClose;
}

void Game::restart()
{
    m_Board->reset();

    for (int i = 0; i < m_NbPlayer; i++)
    {
        m_Players[i]->reset(i + 1, m_Board);
    }

    m_ShouldClose = false;
    m_Turn = 0;
}

void Game::Play()
{
    if (m_Board->IsFinish())
    {
        m_ShouldClose = true;
        //CLEAR_SCREEN();
        return;
    }

    m_Players[m_Turn]->Play();

    //next turn, nbPlayer - 1 because turn goes from 0 to nbPlayer - 1
    m_Turn = m_Turn == m_NbPlayer - 1 ? 0 : m_Turn + 1;
}

void Game::Step()
{
    Update();
    if (m_Turn % m_NbPlayer == 0)
    {
        Render();
    }
    Play();
}


void Game::simulation_step()
{
    Update();
    Play();
}

std::vector<float> Game::scores() const
{
    std::vector<float> players_score;

    players_score.reserve(m_NbPlayer);
    for (int i = 0; i < m_NbPlayer; i++)
    {
        players_score.emplace_back(m_Players[i]->score());
    }

    return players_score;
}

void Game::changeBot(uint id, NeuralBot& neuralB)
{
    m_Players[id] = std::make_unique<NeuralBot>(neuralB);
    m_Players[id]->reset(id + 1, m_Board);
}
