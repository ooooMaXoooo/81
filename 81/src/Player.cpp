#include "Player.h"

#include <string>


std::vector<int> Player::m_PlayedTiles;




Player::Player(int id, PlayerType type, std::shared_ptr<Board> board)
    : m_ID(id), m_Type(type), m_Board(board)
{
}

void Player::SendGreyTiles()
{
    //compute grey tiles
    UpdateGreyTiles();
    //send data to the board
    for (int i = 0; i < m_GreyTiles.size(); ++i)
    {
        m_Board->Update(m_GreyTiles[i], -1);
    }
}

void Player::ClearGreyTiles()
{
    const int size = m_GreyTiles.size();
    for (int i = 0; i < size; ++i)
    {
        m_Board->Update(m_GreyTiles[0], 0);
        m_GreyTiles.erase(m_GreyTiles.begin());
    }
}

bool Player::Play()
{
    int pos = -1;
    switch (m_Type)
    {
    case PlayerType::Human:
        pos = PlayAt_Human();
        break;

    case PlayerType::Bot:
        pos = PlayAt_Bot(m_Board->GetMap());
        break;

    default:
        break;
    }

    if (find(m_GreyTiles.begin(), m_GreyTiles.end(), pos) != m_GreyTiles.end())
    {
        std::cout << "\nErreur impossile de jouer a " << pos << std::endl;
        return false;
    }

    m_LastPos = pos;
    // TODO : place m_PlayedTiles.reserve() somewhere
    m_PlayedTiles.emplace_back(pos);
    // return if there is an error (in future)
    // TODO : handle potential errors
    m_Board->Update(pos, m_ID);

    return true;
}

int Player::PlayAt_Bot(const std::vector<int>& map)
{
    return 0;
}

int Player::PlayAt_Human()
{
    int pos = -1;
    std::string input = "";
    bool correct = false;
    do
    {
        pos = -1;
        pos = InputInNumber(input);

        // verifiy if the tile has already been played
        // check for the last pos
        correct = pos > -1 && pos < 81;

        if (!correct)
            std::cout << "Choose between 0 and 80" << std::endl;
    } while (!correct);

    return pos;
}

int Player::InputInNumber(std::string& input) const
{
    int pos = -1;
    std::cout << "[player " << m_ID << "] : plays at : ";
    std::cin >> input;

    try
    {
        pos = std::stoi(input);
    }
    catch (std::invalid_argument const& ex)
    {
        std::cerr << "invalid argument " << ex.what() << std::endl;
        pos = -1;
    }
    catch (std::out_of_range const& ex)
    {
        std::cerr << "out of range " << ex.what() << std::endl;
        pos = -1;
    }

    std::cout << "\n\n";
    return pos;
}

void Player::UpdateGreyTiles()
{
    int boardSize = m_Board->Size();

    // right side
    if ((m_LastPos + 1) % 9 == 0)
    {
        m_GreyTiles.emplace_back(m_LastPos - 1);
        m_GreyTiles.emplace_back(m_LastPos - boardSize);
        m_GreyTiles.emplace_back(m_LastPos + boardSize);
        m_GreyTiles.emplace_back(m_LastPos - boardSize - 1);
        m_GreyTiles.emplace_back(m_LastPos + boardSize - 1);
    }
    // left side
    else if (m_LastPos % 9 == 0)
    {
        m_GreyTiles.emplace_back(m_LastPos + 1);
        m_GreyTiles.emplace_back(m_LastPos - boardSize);
        m_GreyTiles.emplace_back(m_LastPos + boardSize);
        m_GreyTiles.emplace_back(m_LastPos - boardSize + 1);
        m_GreyTiles.emplace_back(m_LastPos + boardSize + 1);
    }
    // all pos are in [0; 80] --> don't need to check if it's greater than 0
    // bottom row
    else if (m_LastPos < boardSize) // maybe * 3 for size
    {
        m_GreyTiles.emplace_back(m_LastPos + 1);
        m_GreyTiles.emplace_back(m_LastPos - 1);
        m_GreyTiles.emplace_back(m_LastPos + boardSize);
        m_GreyTiles.emplace_back(m_LastPos + boardSize + 1);
        m_GreyTiles.emplace_back(m_LastPos + boardSize - 1);
    }
    // top row
    else if (m_LastPos > (boardSize * (boardSize - 1)))
    {
        m_GreyTiles.emplace_back(m_LastPos + 1);
        m_GreyTiles.emplace_back(m_LastPos - 1);
        m_GreyTiles.emplace_back(m_LastPos - boardSize);
        m_GreyTiles.emplace_back(m_LastPos - boardSize + 1);
        m_GreyTiles.emplace_back(m_LastPos - boardSize - 1);
    }
    // midle
    else
    {
        m_GreyTiles.emplace_back(m_LastPos - 1);
        m_GreyTiles.emplace_back(m_LastPos + 1);

        m_GreyTiles.emplace_back(m_LastPos - boardSize);
        m_GreyTiles.emplace_back(m_LastPos + boardSize);

        m_GreyTiles.emplace_back(m_LastPos - boardSize - 1);
        m_GreyTiles.emplace_back(m_LastPos - boardSize + 1);

        m_GreyTiles.emplace_back(m_LastPos + boardSize - 1);
        m_GreyTiles.emplace_back(m_LastPos + boardSize + 1);
    }

    // check for played tile
    int count = 0;
    const int size = m_GreyTiles.size();
    for (int i = 0; i < size; ++i)
    {
        if ((std::find(m_PlayedTiles.begin(), m_PlayedTiles.end(), m_GreyTiles[count]) != std::end(m_PlayedTiles)) || m_GreyTiles[count] < 0)
        {
            m_GreyTiles.erase(m_GreyTiles.begin() + count);
            continue;
        }
        ++count;
    }
}